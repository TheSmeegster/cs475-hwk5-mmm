#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[]) {
	double clockstart, clockend;
	int threadNum = 0;

	clockstart = rtclock(); // start clocking

	// start: stuff I want to clock

	if(argc == 3){
		if(*argv[1] == 'S'){
			mmm_freeup(*argv[2] - '0');
			mmm_init(*argv[2] - '0');
			mmm_seq();
		} else {
			printf("Incorrect input format ");
		}
	} else if(argc == 4){
		if(*argv[1] == 'P'){
			mmm_freeup(*argv[3] - '0');
			mmm_init(*argv[3] - '0');
			
			threadNum = (*argv[2] - '0');

			if(threadNum > (*argv[3] - '0')){
				printf("Incorrect input format");
			} else {
				int threadScope = (*argv[3] - '0') / threadNum;
				int start = 0;
				int end = threadScope;
				int args[] = {start, end};
				for(int threadCount = 0; threadCount < threadNum; threadCount++){

					pthread_t threadId;

					if((end + threadScope * 2) > (*argv[3] - '0')){
						if(start + threadScope * 3 > (*argv[3] - '0')){
							if(start + threadScope * 2 <= (*argv[3] - '0')){
								args[1] = (*argv[3] - '0');
							} else {
							   break;
							}
							
						} else {
							printf("%i and %i and %i \n", threadScope, start + threadScope * 2, (*argv[3] - '0'));
						}
					}

					pthread_create(&threadId, NULL, mmm_par, args);

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

	// end: stuff I want to clock

	clockend = rtclock(); // stop clocking
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}
