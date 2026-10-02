#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX 20
 
typedef struct {
    int pid; //process id
    int arrival; // time it takes to arrive
    int burst; // total CPU time
    int remaining; // time left
    int start; //starting time ran
    int end; // time finished
} test;
 
int main(int argc, char *argv[]) {

    //input file
    if (argc < 3) {
        printf("Usage: %s input_file [FCFS|RR|SJF] [time_quantum]\n", argv[0]);
        return 1;
    }
 
    char *algo = argv[2];
    int q = 0;
    if (argc > 3) {
        q = atoi(argv[3]);
    }
    if (strcmp(algo, "RR") == 0 && q <= 0) {
        printf("RR needs a time quantum\n");
        return 1;
    }
 //open input file 
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Cant open file %s\n", argv[1]);
        return 1;
    }
    //tasks needed
    int l;
    fscanf(fp, "%d", &l);
    test t[100];
    for (int i = 0; i < l; i++) {
        fscanf(fp, "%d %d %d", &t[i].pid, &t[i].arrival, &t[i].burst);
        t[i].remaining = t[i].burst;
        t[i].start = -1;
    }
    fclose(fp);
 
    int queue[MAX];
    int qn = 0;
    int current = -1;
    int used = 0;
    int done = 0;
 
    printf("%s:\n", algo);
 
    for (int time = 0; done < l; ++time) {
 
       
        for (int i = 0; i < l; i++) {
            if (t[i].arrival == time) {
                queue[qn++] = i;
            }
        }
 
        //RR go back to queue
        if (current != -1 && strcmp(algo, "RR") == 0 && used == q) {
            queue[qn++] = current;
            current = -1;
        }
 
        
        if (current == -1 && qn > 0) {
            int pick = 0;   
 //SJF shortest burst
            if (strcmp(algo, "SJF") == 0) {
                for (int i = 1; i < qn; ++i) {
                    if (t[queue[i]].burst < t[queue[pick]].burst) {
                        pick = i;
                    }
                }
            }
 
            current = queue[pick];
 
            
            for (int i = pick; i < qn - 1; ++i) {
                queue[i] = queue[i + 1];
            }
            qn--;
            used = 0;
 
            if (t[current].start == -1) {
                t[current].start = time;
            }
        }
 
       //CPU is idle
        if (current == -1) {
            printf("Time %d: Idle\n", time);
            continue;
        }
 
        
        printf("Time %d: PID %d running\n", time, t[current].pid);
        used++;
        //see if finished
        if (--t[current].remaining == 0) {
            t[current].end = time + 1;
            printf("Time %d: PID %d finished\n", time + 1, t[current].pid);
            done++;
            current = -1;
        }
    }
 //printing table
    printf("\nPID  Arrival  Start  End  Running  Waiting\n");
    double total = 0;
    for (int i = 0; i < l; i++) {
        int wait = t[i].end - t[i].arrival - t[i].burst;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               t[i].pid, t[i].arrival, t[i].start, t[i].end, t[i].burst, wait);
        total += wait;
    }
    //average
    printf("\nAverage Waiting Time: %g\n", total / l);
 
    return 0;
}