#include "header2.h"
/*
 * Assumptions:
 * Input is sorted on Arrival Time
*/ 
int max(int a,int b){
	return (a>b)?a:b;
}
Process* MakeNode(char process_name[101],int process_arrival_time,int process_cpu_burst, bool flag)
{
	Process* nptr=(Process*)malloc(sizeof(Process));
	if(nptr!=NULL)
	{
		strcpy(nptr->proc_name,process_name);
		nptr->cp_proc_arrival_time = nptr->proc_arrival_time = process_arrival_time;
		nptr->copy_burst_time = nptr->proc_cpu_burst = process_cpu_burst;
		nptr->next = NULL;
		nptr->flag = flag;
		nptr->exit_time=-1;
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



void free_mem(Process* ptr){

	Process* del=NULL;
	while(ptr != NULL){

		del = ptr;
		ptr = ptr->next;
		free(del);
	}

}
Process* divide(Process* head_ptr)
{
	//fast-slow algorithm
	Process* fast,*slow;
	fast=head_ptr;
	slow=head_ptr;
	fast = (head_ptr->next)->next;
	while(fast != NULL)
	{
		fast = fast -> next;
		slow = slow -> next;
		if(fast){
			fast = fast->next;
		}
	}
	Process* save = slow->next;
	slow->next = NULL;
	return save;
}
Process* Merge(Process* head1,Process* head2)
{
	Process* ret_head=NULL;
	Process* ret=NULL;
	if(head1==NULL)
	{
		ret_head = head2;	
	}
	else if(head2 == NULL)
	{
		ret_head = head1;
	}
	else{
		if(head1->proc_cpu_burst <= head2->proc_cpu_burst)
		{
			ret = ret_head = head1;
			head1 = head1->next;
		}
		else
		{
			ret = ret_head = head2;
			head2 = head2->next;
		}
	}
	while(head1 && head2)
	{
		if(head1->proc_cpu_burst <= head2->proc_cpu_burst)
		{
			ret->next = head1;
			ret = ret->next;
			head1 = head1->next;
		}
		else
		{
			ret->next = head2;
			ret = ret->next;
			head2 = head2->next;
		}
	}
	if(head1){
		ret->next=head1;
	}else{
		ret->next=head2;
	}
	return ret_head;
}
Process* MergeSort(Process* head_ptr)
{
	Process* ret_head=NULL;
	if((head_ptr) == NULL || (head_ptr)->next == NULL)
	{
		ret_head=head_ptr;
	}
	else
	{
		Process* head2 = divide(head_ptr);
		head_ptr=MergeSort(head_ptr);
		head2=MergeSort(head2);
		ret_head=Merge(head_ptr,head2);
		
	}
	return ret_head;
}


void InitializeReadyQueue(Queue* queue)
{
	queue->front = queue->rear = NULL;
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
		ret = MakeNode(ptr->proc_name,ptr->proc_arrival_time,ptr->proc_cpu_burst,ptr->flag);
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
		fprintf(stderr,"Please specify file name\n");
		exit(1);
	}
	Process* head_ptr=NULL;
	Process* cp_head_ptr=NULL;
	Process* node=NULL;
	char process_name[101];
	int process_arrival_time;
	int process_cpu_burst;

	FILE* fp=fopen(argv[1],"r");
	int num_processes = 0;
	if(fp != NULL){

		printf("\tQuestion 2\tSJF ALGORITHM\n");

		while(fscanf(fp,"%s",process_name) != EOF)
		{
			fscanf(fp,"%d%d",&process_arrival_time,&process_cpu_burst);
			node = MakeNode(process_name,process_arrival_time,process_cpu_burst,false);
			head_ptr=InsertInLL(head_ptr,node);
			num_processes++;
		}
		
		Queue queue;
		InitializeReadyQueue(&queue);
		cp_head_ptr = head_ptr;
		int curr_time = 0;
		int count=0;
		cp_head_ptr = head_ptr;
		//Among available processes, choose the least one
		double avg_penalty=0.0;

		printf("PROCESSES:(each name indicates run of 1 unit of time)\n");
		bool flag=false;
		while(!flag)
		{
			flag = true;
			//choose minimum from available
			cp_head_ptr = head_ptr;
			Process* proc_choosen = NULL;
			while(cp_head_ptr != NULL)
			{
				if(cp_head_ptr->proc_cpu_burst > 0){
					flag = false;
					if(curr_time >= cp_head_ptr->proc_arrival_time)
					{
						if(proc_choosen == NULL || cp_head_ptr->proc_cpu_burst < proc_choosen->proc_cpu_burst)
						{
							proc_choosen = cp_head_ptr;						
						}
					}
				}
				cp_head_ptr = cp_head_ptr->next;

			}

			if(proc_choosen != NULL){
				proc_choosen ->proc_cpu_burst-= 1;

				curr_time ++;
				if(proc_choosen->proc_cpu_burst == 0){
					proc_choosen->exit_time = curr_time;
					proc_choosen->flag = true;	//not required but still

				}
				else{
					;//InsertInReadyQueue(&queue,MakeNode(proc_choosen->proc_name,proc_choosen->proc_arrival_time,proc_choosen->proc_cpu_burst,proc_choosen->flag));
				}

			}
			else{
				curr_time++;
			}
		}
		printf("\n");
		
		printf("PROCESS\tWait time\tTurnaround\tPenalty Ratio\n");
		cp_head_ptr=head_ptr;
		double avg_wait=0.0,avg_turn=0.0,system_throughput=0.0;
		while(cp_head_ptr != NULL)
		{
			int wait_time=cp_head_ptr->exit_time - cp_head_ptr->proc_arrival_time - cp_head_ptr->copy_burst_time;
			if(wait_time < 0)
				wait_time = 0;
			int turnaround = wait_time + cp_head_ptr->copy_burst_time;
			avg_wait += wait_time;
			avg_turn += turnaround;

			double penalty = (1.0*turnaround)/(cp_head_ptr->copy_burst_time);
			printf("%s\t %d\t\t%d\t\t%.2lf\n",cp_head_ptr->proc_name,wait_time,turnaround,penalty);
			cp_head_ptr = cp_head_ptr->next;
		}
		
		printf("\nAverage Wait time: %.2lf\nAverage turnaround time: %.2lf\nSystem Throughput: %.2lf\n",(avg_wait*1.0)/num_processes,(avg_turn*1.0)/num_processes,(1.0*num_processes)/(curr_time-1));

	}

	return 0;
}

