#include "header.h"

/*
 *
 * Assumptions:
 * Arrival time of 1st process = 0
 * For FCFS, input is sorted on arrival time
 *
*/


int max(int a,int b){
	return (a>b)?a:b;
}

Process* MakeNode(char process_name[101],int process_arrival_time,int process_cpu_burst)
{
	Process* nptr=(Process*)malloc(sizeof(Process));
	if(nptr!=NULL)
	{
		strcpy(nptr->proc_name,process_name);
		nptr->proc_arrival_time = process_arrival_time;
		nptr->proc_cpu_burst = process_cpu_burst;
		nptr->next = NULL;
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

		printf("\tQuestion 1\tFCFS ALGORITHM\n");

		while(fscanf(fp,"%s",process_name) != EOF)
		{
			fscanf(fp,"%d%d",&process_arrival_time,&process_cpu_burst);
			node = MakeNode(process_name,process_arrival_time,process_cpu_burst);
			head_ptr=InsertInLL(head_ptr,node);
			num_processes++;
		}

		//Assuming sorted on arrival time for FCFS algorithm


		int wait_time[num_processes];
		int service[num_processes];
		int turnaround[num_processes];
		double penalty_ratio[num_processes];

		cp_head_ptr=head_ptr;
		int tot_wait=0;
		int tot_turn=0;
		//calculate wait_time
		//wait_time for process 0 is 0
		int i=0;
		if(cp_head_ptr != NULL){
			wait_time[0] = 0;
			service[0] = 0;
			i++;
			while(cp_head_ptr->next != NULL)
			{
				service[i] = service[i-1] + cp_head_ptr->proc_cpu_burst;
				wait_time[i] = service[i] - cp_head_ptr->next->proc_arrival_time;
				if(wait_time[i] < 0)
					wait_time[i] = 0;
				tot_wait += wait_time[i];
				i++;
				cp_head_ptr = cp_head_ptr->next;
			}
		}
		
		cp_head_ptr=head_ptr;//RESET head pointer
		i=0;
		if(cp_head_ptr != NULL){
			turnaround[0] = cp_head_ptr->proc_cpu_burst;
			penalty_ratio[0] = (1.0*turnaround[0])/(cp_head_ptr->proc_cpu_burst);
			tot_turn = turnaround[0];
			i++;
			cp_head_ptr=cp_head_ptr->next;
			while(cp_head_ptr != NULL)
			{
				turnaround[i] = wait_time[i] + cp_head_ptr->proc_cpu_burst;
				penalty_ratio[i] = (1.0*turnaround[i])/(cp_head_ptr->proc_cpu_burst);
				cp_head_ptr = cp_head_ptr->next;
				tot_turn += turnaround[i];
				i++;
			}
		}

		printf("Process Name\tWait Time\tTurnaround\tPenalty Ratio\n");
		cp_head_ptr = head_ptr;
		i=0;
		while(cp_head_ptr != NULL)
		{
			printf("%s\t\t%d\t\t%d\t\t%.2lf\n",cp_head_ptr->proc_name,wait_time[i],turnaround[i],penalty_ratio[i]);
			i++;
			cp_head_ptr = cp_head_ptr->next;
		}


		int tot_time_taken = 0;

		cp_head_ptr = head_ptr;
		while(cp_head_ptr != NULL)
		{
			if(cp_head_ptr->next != NULL)
				tot_time_taken += max(cp_head_ptr->proc_cpu_burst,cp_head_ptr->next->proc_arrival_time);
			else
				tot_time_taken += cp_head_ptr->proc_cpu_burst;
			cp_head_ptr = cp_head_ptr->next;
		}



		printf("\nStatistics:\nAverage Waiting Time = %.2lf\nAverage Turnaround Time = %.2lf\nSystem Throughput = %.2lf\n",(1.0*tot_wait)/num_processes,(1.0*tot_turn)/num_processes,(1.0*num_processes)/tot_time_taken);




		free_mem(head_ptr);
		fclose(fp);

	}
	return 0;

}

