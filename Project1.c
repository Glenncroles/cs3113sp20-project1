#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * format specifiers (%.02f)
 */

/**
 * The linkedList
 * for the pid burst and priority
 **/
struct Link
{
	int pid;
	int burst;
	int priority;
	int duplicate;
	int lastDup;
	int arrival;
	struct Link *next;
};

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new link at the end  */
void insert(struct Link** start, int in_pid, int in_burst, int in_priority)
{
	/* allocate link memory */
	struct Link* nlink = (struct Link*) malloc(sizeof(struct Link));

	//struct Link * link = *start;

	struct Link * last = *start;  //for puting into last postion

	/*put in the data*/
	nlink->burst = in_burst;
	nlink->pid = in_pid;
	nlink->priority = in_priority;
	nlink->duplicate = 0;
	nlink->lastDup = 0;
	nlink->arrival = 0;

	/* This new link is going to be the last link, so make next of
          it as NULL*/
	nlink->next = NULL;


	/* If the Linked List is empty, then make the new link as head */
	if (*start == NULL)
	{
		*start = nlink;
		return;
	}
	else
	{
		/*Else traverse till the last link */
		while (last->next != NULL)
		{
			if(nlink->pid == last->pid)
			{
				nlink->lastDup = 1;
				nlink->duplicate = 1;
				last->duplicate = 1;

				if(last->lastDup == 1)
				{
					last->lastDup = 0;
				}


			}
			if(last->next->next == NULL)
			{
				if(nlink->pid == last->next->pid)
				{
					nlink->duplicate = 1;
					nlink->lastDup = 1;
					last->next->duplicate = 1;
					last->next->lastDup = 0;
				}


			}

			last = last->next;
		}

		/*Change the next of last link */
		last->next = nlink;
	}
	return;
}


// This function prints contents of linked list starting from begging to end
void printLinks(struct Link *link)
{
	while (link != NULL)
	{
		printf("%d->%d->%d->%d\n", link->pid, link->duplicate, link->lastDup, link->arrival);
		link = link->next;
	}
}

void arrival(struct Link *link, int N)
{
	int pid[N];
	int pos = 0;
	int flag = 0;
	while(link->next != NULL)
	{
		if(link->duplicate == 1)
		{
			for(int i = 0; i < pos; i++)
			{
				if(pid[i] == link->pid)
				{
					flag = 1;
					break;
				}
			}
			link->arrival = 1;
			pid[pos] = link->pid;
			pos++;
		}
		else
		{
			link->arrival = 1;
		}
		if(flag == 1)
		{
			link->arrival = 0;
		}
		link = link->next;
	}
}


/**
 * getting the link of a certain posistion
 */
struct Link *getLink(struct Link *link, int index)
{
	int counter = 0;

	while(link != NULL)
	{
		if(counter == index)
		{
			return link;
		}
		counter++;
		link = link->next;
	}

	return link;

}

/**
 * turnaround(head, p)
 * Completion Time - Arrival Time
 */
void turnwaitresp(struct Link *link, int p, int N)
{
	/**
	 * average
	 * total burst time
	 */
	double average = 0.0;
	double waverage = 0.0;
	double raverage = 0.0;
	int totalBurst = 0;
	int nonVol = 0;
	int count = 0;
	//int tsize = N;
	int arrival = 0;
	int sum = 0;
	int wsum = 0;
	int rsum = 0;
	//int flag = 0;
	//int flagssss[N];
	//int* flags = flagss(link,p,N,flagssss);



	for(int i = 0; i < N; i++)
	{
		struct Link *link1 = getLink(link, i);
		arrival = 0;

		/**adding current burst to total burst**/
		totalBurst += link1->burst;


		if(link1->duplicate == 0)
		{
			arrival = totalBurst;
			sum += totalBurst;
			wsum += arrival;
			rsum += totalBurst - arrival;

		}
		else
		{
			if(link1->arrival == 1)
			{
				arrival = totalBurst;
			}
			if(link1->lastDup == 1)
			{
				sum += totalBurst;
				wsum += arrival;
				rsum += totalBurst - arrival;
				continue;
			}
			if(link1->next == NULL)
			{
				sum += totalBurst;
				wsum += arrival;
				rsum += totalBurst - arrival;
			}

			continue;
		}

		if(link1->next != NULL)
		{
			if(link1->pid == link1->next->pid)
			{
				count++;
			}
		}

	}



	nonVol = N-count-p;
	if(nonVol < 0)
		printf("0\n");
	else
		printf("%d\n", nonVol);


	printf("100.00\n");

	double tput = (double)p/(double)totalBurst;

	printf("%.02f\n", tput);

	average = (double)sum/(double)p;
	waverage = (double)wsum/(double)p;
	raverage = (double)rsum/(double)p;

	printf("%.02f\n", average);
	printf("%.02f\n", waverage);
	if(waverage == 0)
	{
		printf("%.02f\n", 0.00);
	}
	else
	{
		printf("%.02f\n", raverage);
	}
}


int main(int argc, char** argv)
{
	/**
	 * P is the number of Processes available to run
	 * p is number of execution elements available
	 * 		number of threads
	 * 		1<= p <= 32
	 * N is number of instructions in the lines to fallow
	 * 	  	1<= N <= 2^15
	 **/
	int P = 0;
	int p = 0;
	int N = 0;

	/**
	 * PID
	 * Burst
	 * Priority
	 */
	int PID = 0;
	int burst = 0;
	int priority = 0;


	/* Start with the empty list */
	struct Link* head = NULL;

	//bringing in the file this is for testing
	FILE* input = fopen(argv[1], "r");

	/**
	 * reading in the file
	 * putting the pid burst and priority into the linked list
	 */
	fscanf(input, "%d", &P);
	fscanf(input, "%d", &p);
	fscanf(input, "%d", &N);

	while ((fgetc(input)) != EOF)
	{
		fscanf(input, "%d", &PID);
		fscanf(input, "%d", &burst);
		fscanf(input, "%d", &priority);

		insert(&head, PID, burst, priority);
	}
	arrival(head,N);
	//printLinks(head);

	printf("%d\n", p);
	turnwaitresp(head, p, N);

	return 0;
}
