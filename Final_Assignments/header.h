#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
typedef enum{false=0,true} bool;

typedef struct ProcessesData
{
	char proc_name[101];
	int proc_arrival_time;
	int proc_cpu_burst;
	struct ProcessesData* next;//linked list
	bool flag;
}Process;


typedef struct QueueDS
{
	Process* front;
	Process* rear;
}Queue;

#endif