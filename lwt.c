#include <stdio.h>
#include <stdlib.h>
#include "lwt.h"
#define STACKSIZE 8192
//data typdes
//global
lwt_t 			current_TCB;
lwt_context* 	main_context;
ps_list_head 	__list_h_runnable;

//threads
void* foo1(void* arg)
{
	printf("foo1:%d\n",(int)(arg));
	return arg + 1;
	//lwt_yield(NULL);
}

void* foo2(void* arg)
{
	printf("foo2:%d\n",(int)(arg));
	return arg - 1;
	//lwt_yield(NULL);
}


//assist
void trampoline()
{
	current_TCB->return_val = current_TCB->fn(current_TCB->arg);
	//__lwt_dispatch(next_TCB->context,current_TCB->context);
	lwt_die(current_TCB->return_val);
}

unsigned long stackget()
{
	return (unsigned long)(malloc(STACKSIZE) + STACKSIZE);
}

//initial
lwt_t TCB_initial(lwt_fn_t fn,void* arg,lwt_info_t status)
{
	lwt_t pTCB 			= (lwt_t)malloc(sizeof(TCB));
	
	pTCB->stack 		= stackget();
	pTCB->fn 			= fn;
	pTCB->arg			= arg;
	pTCB->status 		= status;
	pTCB->context 		= (lwt_context*)malloc(sizeof(lwt_context));
	
	return pTCB;
}

//thread library
lwt_t lwt_create(lwt_fn_t fn, void* data)
{
	lwt_t pTCB 	= TCB_initial(fn,data,LWT_INFO_NTHD_RUNNABLE);
	ps_list_head_append_d(&__list_h_runnable,pTCB);
}

int lwt_yield(lwt_t pTCB)
{
	if(pTCB = NULL)
		lwt_schedule();
	
	if(pTCB->status == LWT_INFO_NTHD_RUNNABLE)//?????
		ps_list_head_append_d(&__list_h_runnable,current_TCB);
	ps_list_head_add_d(&__list_h_runnable,pTCB);
	lwt_schedule();
}

void lwt_die(void* return_val)
{
	//__lwt_dispatch(next_TCB->context,current_TCB->context);
	lwt_yield(NULL);
}

void lwt_schedule()
{
	lwt_t current_TCB = ps_list_head_first_d(&__list_h_runnable,TCB);
	current_TCB->context->ip = (unsigned long)trampoline;

	if(main_context->sp == 0 && main_context->ip == 0)
		printf("11111");
		__lwt_dispatch(main_context,current_TCB->context);

	if(!ps_list_head_empty(&__list_h_runnable))
	{
		ps_list_rem_d(current_TCB);
		__lwt_dispatch(current_TCB->context,current_TCB->context);
	}

	__lwt_dispatch(current_TCB->context,main_context);
	return;
}

void prepare()
{
	ps_list_head_init(&__list_h_runnable);
	current_TCB 		= TCB_initial(NULL,NULL,LWT_INFO_NTHD_RUNNABLE);
	main_context 		= (lwt_context*)malloc(sizeof(lwt_context)); 
	main_context->sp 	= 0;
	main_context->ip	= 0;
}

int main()
{
	prepare();
	printf("11111");	
	lwt_t thread1 = lwt_create(foo1,(void*)1);
	lwt_t thread2 = lwt_create(foo2,(void*)1);
	printf("11111");	
	lwt_yield(thread1);
	return 0;
}