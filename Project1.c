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
	struct Link *next;
};

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new link at the end  */
void insert(struct Link** start, int in_pid, int in_burst, int in_priority)
{
	/* allocate link memory */
	struct Link* nlink = (struct Link*) malloc(sizeof(struct Link));

	struct Link * last = *start;  //for puting into last postion

	/*put in the data*/
	nlink->burst = in_burst;
	nlink->pid = in_pid;
	nlink->priority = in_priority;

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
			last = last->next;
		}

		/*Change the next of last link */
		last->next = nlink;
	}
	return;
}

/**
 * A simple size method for my linked list
 */
int size(struct Link *link)
{
	int size = 0;
	while (link != NULL)
	{
		size++;
		link = link->next;
	}
	return size;
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
// This function prints contents of linked list starting from begging to end
void printLinks(struct Link *link)
{
	//TODO maybe???? i dont htink so
	while (link != NULL)
	{
		printf("%d %d %d\n", link->pid, link->burst, link->priority);
		link = link->next;
	}
}


/**
 * nonVolantary
 * TODO this calculation is (N-backtoback)-p
 */
void nonVolantary(struct Link *link, int p, int N)
{
	/**This is for the printed amount**/
	int nonVol = 0;
	int count = 0;

	for(int i = 0; i < size(link)-1; i++)
	{
		if(getLink(link, i)->pid == getLink(link, i+1)->pid)
		{
			count++;
		}
	}

	nonVol = (N-count)-p;

	printf("%d\n", nonVol);
}


/**
 * throughput
 * This calculation is the total p/total burst
 */
void throughput(struct Link *link, int p)
{
	/**the throughput calculating variable**/
	double tput = 0.00;

	/**The total burst variable**/
	int burst = 0;

	/**
	 * Traverese through the whole linkeList
	 * this is to add the total burst
	 */
	while (link != NULL)
	{
		burst += link->burst;
		link = link->next;
	}

	tput = (double)p/(double)burst;

	printf("%.02f\n", tput);
}


/**
 * turnaround(head, p)
 * Completion Time - Arrival Time
 */
void turnwaitresp(struct Link *link, int p)
{

	/**
	 * average
	 * total burst time
	 */
	double average = 0.0;
	double waverage = 0.0;
	double raverage = 0.0;
	int totalBurst = 0;
	int tsize = size(link);
	int turn[p+1];
	int wait[p+1];
	int ret[p+1];
	int arrival = 0;
	int sum = 0;
	int wsum = 0;
	int rsum = 0;
	int flag = 0;
	int flags[p+1];
	for(int i = 1; i < p+1;i++)
	{
		flags[i] = 0;
	}
	ret[0] = 0;

	int PID[tsize];
	int burst[tsize];
	int priority[tsize];


	for(int i = 0; i < tsize; i++)
	{
		struct Link *link1 = getLink(link, i);
		PID[i] = link1->pid;
		burst[i] = link1->burst;
		priority[i] = link1->priority;


		/**adding current burst to total burst**/
		totalBurst += burst[i];

		/**for loop to loop throught the rest of the linkedList**/
		for(int j = i+1; j < tsize; j++)
		{
			/**if its the last occurance of our pid calculate**/
			if(PID[i] != getLink(link, j)->pid)
			{
				flag = 1;
			}
			else
			{
				flag = 0;
				flags[getLink(link, j)->pid] = 1;
			}
		}

		if(flag == 1)
		{
			if(flags[i] == 1)
			{
				turn[PID[i]] = totalBurst;
				wait[PID[i]] = arrival;
				ret[PID[i]] = totalBurst - arrival;
				continue;
			}

			turn[PID[i]] = totalBurst;
			wait[PID[i]] = arrival;
			ret[PID[i]] = totalBurst - arrival;
			arrival = totalBurst;
		}
		if(flag == 0)
		{
			if(link1->next == NULL)
			{
				turn[PID[i]] = totalBurst;
				wait[PID[i]] = arrival;
				ret[PID[i]] = totalBurst - arrival;
			}
			continue;
		}
	}

	for(int i = 1; i < p+1; i++)
	{

		sum += turn[i];
		wsum += wait[i];
	}
	for(int i = 0; i < p+1; i++)
	{
		rsum += ret[i];
	}


	average = (double)sum/(double)p;
	waverage = (double)wsum/(double)p;
	raverage = (double)rsum/(double)p;
	printf("%.02f\n", average);
	printf("%.02f\n", waverage);
	printf("%.02f\n", raverage);
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

	/**
	 * CPU Utilization
	 */
	double CPU_Utilization = 100.00;

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

	while (fscanf(input, "%d", &PID) != EOF)
	{
		fscanf(input, "%d", &burst);
		fscanf(input, "%d", &priority);

		insert(&head, PID, burst, priority);
	}

	printf("%d\n", p);
	nonVolantary(head, p, N);
	printf("%.02f\n", CPU_Utilization);
	throughput(head, p);
	turnwaitresp(head, p);


	return 0;
}
