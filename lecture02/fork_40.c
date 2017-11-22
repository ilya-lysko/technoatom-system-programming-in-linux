#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void f(int j) {
	printf("%d\n", j);
}

int main() {
	int n = 40, key = 666;
	pid_t pid;

	int semid;
    struct sembuf sem_wait[] = {
        {0, -1, 0},
        {0,  1, 0}
    };
 
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if(semid < 0) {
        printf("Mistake!");
        exit(1);
    }
 
    for(int i = 0; i < n; ++i) {
        pid = fork();
        if (pid == 0) {
            semop(semid, &sem_wait[0], 1);
        } else if (pid < 0) {
            printf("Mistake!");
            exit(1);
        } else {
            f(i + 1);
            semop(semid, &sem_wait[1], 1);
            exit(0);
        }
    }
 
    semctl(semid, IPC_RMID, 0);
 
    return 0;
}