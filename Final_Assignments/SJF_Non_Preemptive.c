#include "header.h"
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
		nptr->proc_arrival_time = process_arrival_time;
		nptr->proc_cpu_burst = process_cpu_burst;
		nptr->next = NULL;
		nptr->flag = flag;
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

		printf("PROCESSES:   ");
		while(count < num_processes)
		{
			//choose minimum from available
			cp_head_ptr = head_ptr;
			Process* proc_choosen = NULL;
			while(cp_head_ptr != NULL)
			{
				if(cp_head_ptr->flag == false && curr_time >= cp_head_ptr->proc_arrival_time)
				{
					if(proc_choosen == NULL || cp_head_ptr->proc_cpu_burst < proc_choosen->proc_cpu_burst)
					{
						proc_choosen = cp_head_ptr;						
					}
				}
				cp_head_ptr = cp_head_ptr->next;

			}
			if(proc_choosen != NULL){
				proc_choosen->flag = true;
				InsertInReadyQueue(&queue,MakeNode(proc_choosen->proc_name,proc_choosen->proc_arrival_time,proc_choosen->proc_cpu_burst,proc_choosen->flag));
				printf("%s\t",proc_choosen->proc_name);
				count++;
				curr_time += proc_choosen->proc_cpu_burst;
			}
			else{
				curr_time++;
			}
		}
		printf("\n");
		int wait_time[num_processes];
		int service[num_processes];
		int turnaround[num_processes];
		double penalty_ratio[num_processes];
		int tot_turn;
		Process* current = GetFront(&queue);
		int i=0;
		int tot_wait=0;
		int tot_time_taken=0;
		assert(current!=NULL);
		//Initialize
		//Wait time i is for process i in the queue
		wait_time[0] = 0;
		service[0] = 0;
		turnaround[0]=current->proc_cpu_burst;
		tot_turn=turnaround[0];
		penalty_ratio[0] = (1.0*turnaround[0])/(current->proc_cpu_burst);
		
		i++;
		while(current->next != NULL){
			service[i] = service[i-1] + current->proc_cpu_burst;
			wait_time[i] = service[i] - current->next->proc_arrival_time;
			if(wait_time[i] < 0)
					wait_time[i] = 0;
			turnaround[i] = wait_time[i] + current->next->proc_cpu_burst;
			tot_turn += turnaround[i];
			penalty_ratio[i] = (1.0*turnaround[i])/(current->next->proc_cpu_burst);
			tot_time_taken += max(current->proc_cpu_burst,current->next->proc_arrival_time);
			//printf("%d ",wait_time[i]);
			tot_wait += wait_time[i];
			i++;
			current = GetFront(&queue);
		}
		if(current)
			tot_time_taken += (current->proc_cpu_burst);
		printf("Wait Time:   ");
		for (int i = 0; i < num_processes; ++i)
		{
			printf("%d\t",wait_time[i]);
		}
		printf("\n");
		printf("Turnaround: ");
		for (int i = 0; i < num_processes; ++i)
		{
			printf("%d\t",turnaround[i]);
		}
		printf("\npenaltyratio ");
		for (int i = 0; i < num_processes; ++i)
		{
			avg_penalty += penalty_ratio[i];
			printf("%.2lf   ",penalty_ratio[i]);
		}
		printf("\n\n");
		printf("\nStatistics:\nAverage Waiting Time = %.2lf\nAverage Turnaround Time = %.2lf\nSystem Throughput = %.2lf\n",(1.0*tot_wait)/num_processes,(1.0*tot_turn)/num_processes,(1.0*num_processes)/tot_time_taken);
		printf("Average Penalty Ratio = %.2lf\n",avg_penalty/(num_processes));

	}

	return 0;
}

