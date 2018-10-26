#include "header2.h"


const int quantum=5;
//const int context_switch=0;
Process* MakeNode(char process_name[101],int process_arrival_time,int process_cpu_burst, bool flag,int exit_time_process)
{
	Process* nptr=(Process*)malloc(sizeof(Process));
	if(nptr!=NULL)
	{
		strcpy(nptr->proc_name,process_name);
		nptr->cp_proc_arrival_time = nptr->proc_arrival_time = process_arrival_time;
		nptr->proc_cpu_burst = process_cpu_burst;
		nptr->next = NULL;
		nptr->flag = flag;
		nptr->exit_time = exit_time_process;
		nptr->copy_burst_time = process_cpu_burst;
	}
	return nptr;
}
Process* InsertInLL(Process* head_ptr,Process* node)
{
	Process* cp_head_ptr=head_ptr;
	if(head_ptr == NULL)
	{
		head_ptr = node;
	}
	else
	{
		while(cp_head_ptr->next != NULL)
		{
			cp_head_ptr = cp_head_ptr->next;
		}
		cp_head_ptr->next = node;
		node->next=NULL;
	}
	return head_ptr;
}
void InitializeReadyQueue(Queue* queue)
{
	queue->front = queue->rear = NULL;
}
int empty(Queue* queue){

	return (queue->front == NULL && queue->rear==NULL);
}
void InsertInReadyQueue(Queue* queue,Process* proc)
{
	if(queue->front == NULL && queue->rear == NULL)
	{
		queue->front = queue->rear = proc;
		proc->next = NULL;
	}
	else
	{
		queue->rear->next = proc;
		proc->next = NULL;
		queue->rear = proc;
	}
}
Process* GetFront(Queue* queue)
{
	Process* ptr=NULL,*ret=NULL;
	
	if(queue->front != NULL){
		ptr = (queue->front);
		
		(queue->front) = (queue->front)->next;
		ret = MakeNode(ptr->proc_name,ptr->proc_arrival_time,ptr->proc_cpu_burst,ptr->flag,-1);
		ret->next=ptr->next;
		if(ptr)
			free(ptr);
		if(queue->front == NULL){
			queue->rear = NULL;
		}
	}
	return ret;
}
int main(int argc, char const *argv[])
{
	if(argc < 2){
		fprintf(stderr, "Please specify filename\n");
		exit(1);
	}
	Process* head_ptr=NULL;
	Process* cp_head_ptr=NULL;
	Process* node=NULL;
	char process_name[101];
	int process_arrival_time;
	int process_cpu_burst;
	bool flag;
	int exit_time_process;
	FILE* fp=fopen(argv[1],"r");
	int num_processes = 0;
	Process* sequence=NULL;
	if(fp != NULL){

		printf("\tQuestion 1.3\tRound Robin ALGORITHM\nBy nature-Prremptive\n\n");

		while(fscanf(fp,"%s",process_name) != EOF)
		{
			fscanf(fp,"%d%d",&process_arrival_time,&process_cpu_burst);
			node = MakeNode(process_name,process_arrival_time,process_cpu_burst,false,-1);
			head_ptr=InsertInLL(head_ptr,node);
			num_processes++;
		}
		Queue ready_queue,*ptr_ready_queue;
		ptr_ready_queue = &ready_queue;
		int current_time = 0;
		InitializeReadyQueue(ptr_ready_queue);
		InsertInReadyQueue(ptr_ready_queue,MakeNode(head_ptr->proc_name,head_ptr->proc_arrival_time,head_ptr->proc_cpu_burst,head_ptr->flag,-1));
		
		cp_head_ptr = head_ptr->next;
		int count=0;
		printf("Order of execution \n");
		int wait_time = 0;
		while(count != num_processes){
			//Take p1 and execute it

			Process* curr_process = GetFront(ptr_ready_queue);
			if(curr_process){
				printf("%s   ",curr_process->proc_name);
				if(curr_process->proc_cpu_burst < quantum){
					current_time += curr_process->proc_cpu_burst;
					curr_process->proc_cpu_burst = 0;
				}else{
					curr_process->proc_cpu_burst -= quantum;
					current_time += quantum;
				}
			}

			while(cp_head_ptr != NULL && cp_head_ptr->proc_arrival_time <= current_time){
				InsertInReadyQueue(ptr_ready_queue,MakeNode(cp_head_ptr->proc_name,cp_head_ptr->proc_arrival_time,cp_head_ptr->proc_cpu_burst,cp_head_ptr->flag,-1));
				cp_head_ptr = cp_head_ptr->next;
			}
			if(curr_process){
				if(curr_process->proc_cpu_burst > 0){
					InsertInReadyQueue(ptr_ready_queue,MakeNode(curr_process->proc_name,curr_process->proc_arrival_time,curr_process->proc_cpu_burst,curr_process->flag,-1));
				}else{
					curr_process->exit_time = current_time;
					Process* another=head_ptr;
					while(another && strcmp(another->proc_name ,curr_process->proc_name)!=0){
						another=another->next;
					}
					if(another){
						another->exit_time = current_time;
						another->flag = true;
					}
					count++;
				}
			}
			if(empty(ptr_ready_queue) && count != num_processes){
					current_time++;	
			}		
		}
		printf("\n\n");
		//print
		cp_head_ptr = head_ptr;
		int tot_wait=0;
		int tot_turn=0;

		printf("Process\tTurnaround\tWait time\n");
		while(cp_head_ptr != NULL)
		{
			//printf("%d\n",cp_head_ptr->proc_arrival_time);
			int turnaround = cp_head_ptr->exit_time-cp_head_ptr->proc_arrival_time;
			assert(cp_head_ptr->proc_cpu_burst == cp_head_ptr->copy_burst_time);
			wait_time=turnaround - cp_head_ptr->proc_cpu_burst;
			tot_wait += wait_time;
			tot_turn += turnaround;
			printf("%s\t%d\t\t%d%.2lf\t",cp_head_ptr->proc_name,turnaround,wait_time,(1.0*turnaround)/(cp_head_ptr->proc_cpu_burst));
			printf("\n");
			cp_head_ptr = cp_head_ptr -> next;
		}
		printf("Average Wait time: %.2lf\nAverage turnaround time: %.2lf\nSystem Throughput: %.2lf\n\n",(1.0*tot_wait)/num_processes,(1.0*tot_turn)/num_processes,(num_processes*1.0)/current_time);

		//printf("Current time %d \n",current_time);
	}

	printf("\n");
	return 0;
}