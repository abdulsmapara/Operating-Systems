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

		int current_time = 0;	//critical time of system
		int i=0;
		int wait[num_processes];
		bool all_done = false;
		while(!all_done){
			all_done = true;
			//While not all processes are done
			cp_head_ptr = head_ptr;
			i=0;
			while(cp_head_ptr != NULL){
				if(cp_head_ptr->proc_arrival_time <= current_time)
				{
					if(cp_head_ptr->proc_arrival_time <= quantum)
					{
						if(cp_head_ptr->proc_cpu_burst > 0){
							all_done = false;
							if(cp_head_ptr->proc_cpu_burst > quantum)
							{
								//Run the process
								current_time += quantum;
								cp_head_ptr->proc_cpu_burst -= quantum;
								cp_head_ptr->proc_arrival_time += quantum;
								InsertInLL(sequence,MakeNode(cp_head_ptr->proc_name,cp_head_ptr->proc_arrival_time,cp_head_ptr->proc_cpu_burst,false,-1));
							}
							else{
								//Last time
								current_time += cp_head_ptr->proc_cpu_burst;
								cp_head_ptr->exit_time = current_time;
								wait[i] = current_time - cp_head_ptr->copy_burst_time - cp_head_ptr->cp_proc_arrival_time;
								
								cp_head_ptr->proc_cpu_burst = 0;
								InsertInLL(sequence,MakeNode(cp_head_ptr->proc_name,cp_head_ptr->proc_arrival_time,cp_head_ptr->proc_cpu_burst,true,cp_head_ptr->exit_time));
							}
						}
					}
					else{// if(cp_head_ptr->proc_arrival_time > quantum){
						Process* again_ptr = head_ptr;
						i=0;
						while(again_ptr != NULL)
						{
							if (again_ptr->proc_arrival_time < cp_head_ptr->proc_arrival_time) { 
									if (again_ptr->proc_cpu_burst > 0) { 
										all_done=false;
										if (again_ptr->proc_cpu_burst > quantum) { 
											current_time += quantum; 
											again_ptr->proc_cpu_burst -= quantum; 
											again_ptr->proc_arrival_time += quantum; 
											InsertInLL(sequence,MakeNode(again_ptr->proc_name,again_ptr->proc_arrival_time,again_ptr->proc_cpu_burst,false,-1));
										} 
										else { 
											current_time += again_ptr->proc_cpu_burst;
											again_ptr->exit_time = current_time;
											wait[i] = current_time - again_ptr->copy_burst_time - again_ptr->cp_proc_arrival_time;
											again_ptr->proc_cpu_burst = 0;
											InsertInLL(sequence,MakeNode(again_ptr->proc_name,again_ptr->proc_arrival_time,again_ptr->proc_cpu_burst,true,again_ptr->exit_time));
										} 
									} 
								}
								i++; 
								again_ptr=again_ptr->next;
						}
						if (cp_head_ptr->proc_cpu_burst > 0) { 
							all_done = false;

							if (cp_head_ptr->proc_cpu_burst > quantum) { 
							current_time += quantum;
								cp_head_ptr->proc_cpu_burst -= quantum;
								cp_head_ptr->proc_arrival_time += quantum;
								InsertInLL(sequence,MakeNode(cp_head_ptr->proc_name,cp_head_ptr->proc_arrival_time,cp_head_ptr->proc_cpu_burst,false,-1));
							}
							else{
								//Last time
								current_time += cp_head_ptr->proc_cpu_burst;
								cp_head_ptr->exit_time = current_time;
								wait[i] = current_time - cp_head_ptr->copy_burst_time - cp_head_ptr->cp_proc_arrival_time;
								
								cp_head_ptr->proc_cpu_burst = 0;
								InsertInLL(sequence,MakeNode(cp_head_ptr->proc_name,cp_head_ptr->proc_arrival_time,cp_head_ptr->proc_cpu_burst,true,cp_head_ptr->exit_time));
							}
					}

				}


			}
			if(cp_head_ptr->proc_arrival_time > current_time) { 
					current_time++;

			}else{

				i++;
				cp_head_ptr=cp_head_ptr->next;
			}

		}
	}
		Process* read = sequence;
		while(read){
			printf("%s  ",read->proc_name);
		}


		printf("\n");
	}

	return 0;
}