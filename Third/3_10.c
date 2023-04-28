#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int lock = 1;

void listener(int sig) {
    lock = 0;
    printf("SIGINT");
}

int main() {
    	int counter = 1; // max_set_size = 100;
	srand(time(NULL));
	signal(SIGINT, listener);
	int *shared_mem = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	if (shared_mem == -1){
		perror("Ошибка подключения сегмента");
	}
    	int *max_num = (shared_mem + sizeof(int));
	int *min_num = (shared_mem + 2 * sizeof(int));
	int *sum_num = (shared_mem + 3 * sizeof(int));
	int *avg_num = (shared_mem + 4 * sizeof(int));

    	while (lock) 
	{
		signal(SIGINT, listener);
        	int max_set_size = rand() % 1000;
		int n = rand() % max_set_size + 1;
        	int *set = shared_mem;
		
        	pid_t pid = fork();
		if (pid == 0) { // first child
        	    signal(SIGINT, listener);
		    int maximum = set[0];
       		    int minimum = set[0];
        	    int sum = set[0];
		    for (int i = 1; i < n; i++) {
        	        if (set[i] > maximum) {
        	            maximum = set[i];
        	        }
        	        if (set[i] < minimum) {
        	            minimum = set[i];
        	        }
			sum += set[i];
        	    }
		    int avg = sum / n;
        	    *max_num = maximum;
        	    *min_num = minimum;
		    *sum_num = sum;
		    *avg_num = avg;
        	    exit(0);
        	} 
		else { // parent
			signal(SIGINT, listener);
			for (int i = 0; i < n; i++) {
                    		set[i] = rand() % 100000;
                	}
        		wait(0);    	    
			printf("%d: Max: %d, Min: %d, Sum: %d, Avg: %d\n", counter, *max_num, *min_num, *sum_num, *avg_num);
        	    	counter++;
        	}
	}

	if (munmap (NULL, sizeof(int)) == -1){
    		perror("Ошибка удаления объекта разделяемой памяти");
	}
	    return 0;
}
