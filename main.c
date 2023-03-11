#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[]) {

	//Used to tell runtime
	double clockstart, clockend;

	//Number of threads for parallel multiplication
	int threadNum = 0;

	//Clock values for averaging
	double clock1 = 0;
	double clock2 = 0;
	double clock3 = 0;

	clockstart = rtclock(); // start clocking

	// start: stuff I want to clock

	//Runs whole method 3 times for average run time
	for(int run = 0; run < 3; run++){

		//If sequential
		if(argc == 3){
			if(*argv[1] == 'S'){
				mmm_freeup(*argv[2] - '0');
				mmm_init(*argv[2] - '0');
				mmm_seq();
			} else {
				printf("Incorrect input format ");
			}
		} else if(argc == 4){

			//Runs parallel matrix
			if(*argv[1] == 'P'){
				mmm_freeup(*argv[3] - '0');
				mmm_init(*argv[3] - '0');
				
				threadNum = (*argv[2] - '0');

				//Runs only if there aren't more threads than rows of matrix
				if(threadNum > (*argv[3] - '0')){
					printf("Incorrect input format");
				} else {

					//Variables for thread range
					int threadScope = (*argv[3] - '0') / threadNum;
					int start = 0;
					int end = threadScope;
					int args[] = {start, end};

					for(int threadCount = 0; threadCount < threadNum; threadCount++){
						
						//Thread id
						pthread_t threadId;

						//Splits the given matrix rows into even chunks based on the number of threads
						//If the number does not divide evenly, the last thread fills the gap to the end
						if((end + threadScope * 2) > (*argv[3] - '0')){
							if(start + threadScope * 3 > (*argv[3] - '0')){
								if(start + threadScope * 2 <= (*argv[3] - '0')){
									args[1] = (*argv[3] - '0');
								} else {
								break;
								}
								
							}
						}
						//Creates a new thread and runs it
						pthread_create(&threadId, NULL, mmm_par, args);

						//Joins thread back to main and increments for next thread
						pthread_join(threadId, NULL);
						start += threadScope;
						end += threadScope;
						args[0] = start;
						args[1] = end;
					}
				}
			} else {
				printf("Incorrect input format");
			}
		} else {
			printf("Incorrect input format");
		}

		clockend = rtclock(); // stop clocking

		//Stores run time of each run to average later
		if(run == 0){
			clock1 = clockend - clockstart;
		} else if(run == 1){
			clock2 = clockend - clockstart;
		} else{
			clock3 = clockend - clockstart;
		}
		printf("\nRun %i Time taken: %.6f sec\n",run, (clockend - clockstart));
	}
	

	// end: stuff I want to clock

	
	
	//Gets avarage run time
	double averageRun = (clock1 + clock2 + clock3) / 3;
	printf("\nAverage runtime = %f", averageRun);

	//Supposed to track overall speedup
	// int speedup = seqTime / pTime;
	// printf("Speedup: ", speedup);

	//Verifies the accuracy of the matrix to a sequential array (not fully functional, the matrices themselves are identical in actuality)
	double verify = mmm_verify();
	printf("\nVerifying... largest error between parallel and sequential matrix: (not functional method) %f\n", verify);

	return 0;
}
