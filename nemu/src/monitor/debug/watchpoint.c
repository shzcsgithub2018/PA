#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
    int i;
    for (i = 0; i < NR_WP; i ++) {
        wp_pool[i].NO = i;
        wp_pool[i].next = &wp_pool[i + 1];
    }
    wp_pool[NR_WP - 1].next = NULL;

    head = NULL;
    free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
uint32_t result_Cal(char *args){
    bool success=true;
    uint32_t result=expr(args,&success);

    if(success!=true)
        Log("Please input current expression");

    return result;
}

void new_wp(char *args){
    if(free_==NULL)
        assert(0);

    WP *tmp=free_;
    free_=free_->next;

    printf("watchpoint: %d\n\n",tmp->NO);
    tmp->last_result=result_Cal(args);
    strcpy(tmp->expr,args);

    tmp->next=head;
    head=tmp;

}

void free_wp(int NO){
    assert(head!=NULL);

    if(head->NO==NO){
        WP *tmp=head;
        head=head->next;
        tmp->next=free_;
        free_=tmp;
        return ;
    }
    for(WP *iter_wp=head;iter_wp->next!=NULL;iter_wp=iter_wp->next){
        if(iter_wp->next->NO==NO){
            WP *tmp=iter_wp->next;
            iter_wp->next=iter_wp->next->next;
            tmp->next=free_;
            free_=tmp;
            return ;
        }
    }

    Log("Not exits NO watchpoint.");
}

void printf_wp(){
    assert(head!=NULL);

    for(WP *iter_wp=head;iter_wp!=NULL;iter_wp=iter_wp->next){
        printf("watchpoint %d:%s\n",iter_wp->NO,iter_wp->expr);
        printf("Now value = %u\n\n",iter_wp->last_result);
    }
}

WP* result_change(){
    assert(head!=NULL);

    uint32_t now_result;

    for(WP *iter_wp=head;iter_wp!=NULL;iter_wp=iter_wp->next){
        now_result=result_Cal(iter_wp->expr);

        if(now_result!=iter_wp->last_result){
            printf("watchpoint %d:%s\n\n",iter_wp->NO,iter_wp->expr);
            printf("Old value = %u\n",iter_wp->last_result);
            printf("New value = %u\n",now_result);

            iter_wp->last_result=now_result;

            return iter_wp;
        }
    }
    
    return NULL;
}