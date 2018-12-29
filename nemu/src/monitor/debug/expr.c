#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
    TK_NOTYPE = 256,TK_NUM,TK_NUM_HEX,TK_RES, TK_EQ,TK_NEQ,TK_AND,TK_POINT

    /* TODO: Add more token types */

};

static struct rule {
    char *regex;
    int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE},            			// spaces
    {"\\+", '+'},                 			// plus
	{"-", '-'},								// decrease
	{"\\*", '*'},							// multiply
	{"/", '/'},								// divide
	{"\\(",'('},							// Left parenthesis
	{"\\)",')'},							// Right parenthesis
	{"\\$(e(([a-d]x)|([bsi]p)|([ds]i)))", TK_RES},//regester
	{"0x[0-9a-fA-F]{1,8}", TK_NUM_HEX},		// Hexadecimal integer
	{"([1-9][0-9]{1,31})|[0-9]",TK_NUM},	// Decimal integer
    {"==", TK_EQ},                			// equal
	{"!=", TK_NEQ},							// not equal
	{"&&", TK_AND},							//And operation
	{"\\*", TK_POINT}						// point
	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i ++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token {
    int type;
    char str[32];
} Token;

Token tokens[10000];
int nr_token;

static bool make_token(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i ++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                        i, rules[i].regex, position, substr_len, substr_len, substr_start);
                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */

                switch (rules[i].token_type) {
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case TK_EQ:
					case TK_NEQ:
					case TK_AND:
					case TK_POINT:
						tokens[nr_token++].type=rules[i].token_type;
						break;
					case TK_NUM:
					case TK_RES:
					case TK_NUM_HEX:
					{
						tokens[nr_token].type=TK_NUM;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token++].str[substr_len]='\0';
						break;
					}
					case TK_NOTYPE:
						break;
                    default: TODO();
                }
                break;
            }
        }

        if (i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;	
}

bool check_parentheses(Token *p,Token *q,bool *success){
	int count=0;
	int sign=0;
	for(Token *iter_p=p;iter_p<q;iter_p++){
		if(iter_p->type=='(')
			count++;
		else if(iter_p->type==')')
			count--;
		if(count==0)
			sign=1;//not exit a pair of parentheses at head and front
	}
	if(q->type==')')
		count--;

	if(sign==0 && count==0 && (p->type=='(' && q->type==')'))
		return true;
	else if(count!=0)
		*success=false;
	return false;
}
   
Token* pos_mop(Token *p,Token *q,bool *success){
	int is_inP=0,sign=0; 
	Token *pos_mod=NULL;
	for(Token *iter_p=p;iter_p<=q;iter_p++){
		if(iter_p->type=='('){
			is_inP++;
			continue;
		}
		if(iter_p->type==')'){
			is_inP--;
			continue;
		}
		if(is_inP!=0)continue;

		if(iter_p->type!='+' && iter_p->type!='-' 
			&& iter_p->type!='*' && iter_p->type!='/')
			continue;

		if (iter_p->type=='+' || iter_p->type=='-'){
			pos_mod=iter_p;
			sign=1;
		}
		else if(sign==0 && 
			(iter_p->type=='*' || iter_p->type=='/'))
			pos_mod=iter_p;
	}
	if(pos_mod==NULL)
		*success=false;
	return pos_mod;
}

uint32_t eval(Token *p,Token *q,bool *success){
	if(*success==false)
		return 0;
  	if (p > q) {
    	/* Bad expression */
		*success=false;
 	}
  	else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
		if(p->type==TK_NUM)
			return (uint32_t)atoi(p->str);
		else
			*success=false;
    }
  	else if (check_parentheses(p, q,success) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    	return eval(p + 1, q - 1,success);
  	}
  	else {
		Token *op = pos_mop(p,q,success);
		//Log("p=%s q=%s\n",p->str,q->str);
		uint32_t val1 = eval(p, op - 1,success);
		uint32_t val2 = eval(op + 1, q,success);

		switch (op->type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': {
				if(val2==0){
					Log("Division by zero,the result is wrong!\n");
					return 1;
				}
				return val1 / val2;
			}
			default: return 0;
    	}
	}
	return 0;
}

uint32_t expr(char *e, bool *success) {
	Log("Hello\n");
    if (!make_token(e)) {
        *success = false;
        return 0;
    }
	// Log("nr_token=%d\n",nr_token);
	// for(int i=0;i<nr_token;i++){
	// 	Log("%c\t",tokens[i].type);
	// 	Log("%s\n",tokens[i].str);
	// }
    /* TODO: Insert codes to evaluate the expression. */
	for (int i = 0; i < nr_token; i ++) {
		if(tokens[i].type != '*' || ( i!=0 &&( tokens[i].type == ')' 
										   	|| tokens[i].type == TK_NUM
										   	|| tokens[i].type == TK_NUM_HEX 
										   	|| tokens[i].type == TK_RES
											) 
									)
			)
			continue;
		else
    		tokens[i].type = TK_POINT;
	}

	return eval(tokens,tokens+nr_token-1,success);
}
