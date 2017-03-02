#include "ps_list.h"
#include "lwt_dispatch.h"
#ifndef LWT
#define LWT
typedef void* (*lwt_fn_t)(void*);
typedef struct lwt_context lwt_context;

typedef enum lwt_status
{
	LWT_INFO_NTHD_RUNNABLE,
	LWT_INFO_NTHD_BLOCKED,
	LWT_INFO_NTHD_ZOMBIES
}lwt_info_t;

typedef struct ps_list ps_list;//node
typedef struct ps_list_head ps_list_head;//list
typedef struct TCB
{
	//resources
	unsigned long 	stack;
	lwt_context* 	context;
	//thread function
	lwt_fn_t 		fn;
	void*			arg;
	void*			return_val;
	//thread status
	lwt_info_t 		status;
	//listnode
	ps_list			plist;
}TCB,*lwt_t;

void lwt_schedule();

lwt_t TCB_initial(lwt_fn_t fn,void* arg,lwt_info_t status);

void lwt_die(void*);

#endif
