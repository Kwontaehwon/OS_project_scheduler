#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

struct Process {
    int p_id;
    int arrivetime; //file input
    int bursttime; //file input
    int waittime;
    int remaintime;
};

int input_data(struct Process *p, int max){
    // 텍스트 파일을 읽어 main의 프로세스 구조체 포인터 배열에 저장하는 함수
    FILE *fd = fopen("process.txt", "r");
    char data[255];
    char *pdata;
    int i = 1;
    int arrivetime, bursttime;

    while (!feof(fd)) {
        fgets(data, sizeof(data), fd);
        pdata = strtok(data, " ");
        for(int j = 0; j < 2; j++){
            if(j == 0){
                arrivetime = atoi(pdata);
                // printf("arrivaltime: %d    ", arrivetime);
            }
            else{
                bursttime = atoi(pdata);
                // printf("bursttime: %d    ", bursttime);
            }
            pdata = strtok(NULL, " ");
        }
        p[i-1].p_id = i;
        p[i-1].arrivetime = arrivetime;
        p[i-1].bursttime = bursttime;
        p[i-1].remaintime = p[i-1].bursttime;
        i++;
    }
    fclose(fd);
    return i-1; // i개의 프로세스 (id : 1 ~ i)
}

int main(){
    struct Process p[MAX];
    int num_process = input_data(&p, MAX);

    FILE *fd = fopen("pytxt.txt", "w");
    fprintf(fd,"%d\n",num_process);

    int origin_numprocess = num_process;
    int currenttime = 0;
    int timeslice;
    printf("timeslice input\n");
    scanf("%d", &timeslice);
    int excute[1000] = {0,};
    struct Process ready[MAX];
    ready[0].p_id = 0;
    struct Process end[MAX];
    int endprocessnum = 0;
    int readynum = 0;
    int now_excute = 0;
    int process_excutetime=0;
    int processlist[100] = {0,};
    while(num_process > 0){
        
        for(int i=0;i<origin_numprocess;i++){ // 들어온 process를 ready 배열에 넣어준다.
            if(p[i].arrivetime == currenttime){
                memcpy(&ready[readynum], &p[i], sizeof(struct Process));
                readynum++;
            }
        }

        if(now_excute == 0){ //실행중인 프로세스 없음
            if(ready[0].p_id != 0){ //ready에 있는 첫번째 프로세서 실행
                now_excute = 1;
                if(processlist[ready[0].p_id] == 0){
                    processlist[ready[0].p_id] = 1;
                    p[ready[0].p_id - 1].waittime = currenttime;
                }
            } else {
                excute[currenttime] = 0;
            }
        }
        
        if(now_excute == 1){ //실행중인 프로세스 있을때
            if(processlist[ready[0].p_id] == 0){
                processlist[ready[0].p_id] = 1;
                p[ready[0].p_id - 1].waittime = currenttime;
            }
            excute[currenttime] = ready[0].p_id;
            ready[0].remaintime--;
            process_excutetime++;
            if(ready[0].remaintime == 0){
                printf("Process %d (%d, %d)\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);
                fprintf(fd,"%d %d %d\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);
                //printf("Process %d is finshed at %d\n", ready[0].p_id, currenttime);
                memcpy(&end[endprocessnum],&ready[0],sizeof(struct Process));
                endprocessnum++;
                if(readynum > 1){
                    for(int i=0;i<readynum-1;i++){
                    memcpy(&ready[i],&ready[i+1],sizeof(struct Process));
                    }
                    ready[readynum-1].p_id = 0;
                    ready[readynum-1].arrivetime = 0;
                    ready[readynum-1].bursttime = 0;
                    ready[readynum-1].remaintime = 0;
                    ready[readynum-1].waittime = 0;
                } else {
                    ready[0].p_id = 0; 
                    ready[0].arrivetime = 0; 
                    ready[0].bursttime = 0; 
                    ready[0].remaintime = 0; 
                    ready[0].waittime = 0; 
                }
                readynum--;
                num_process--;
                process_excutetime = 0;
            }

            if(timeslice == process_excutetime){
                //printf("Process %d execution finshed at %d\n",ready[0].p_id, currenttime); // Time Quantum안에 끝나지 않고 다음 프로세스로 넘어가는 경우
                printf("Process %d (%d, %d)\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);
                fprintf(fd,"%d %d %d\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);

                if(readynum > 1){
                    struct Process temp = ready[0];
                    for(int i=0;i<readynum-1;i++){
                        ready[i] = ready[i+1];
                    }
                    ready[readynum-1] = temp;
                }
                process_excutetime = 0;
            }
        }
        currenttime++;
    }

    
    for(int i=0;i<origin_numprocess;i++){
        printf("p_id %d wait time : %d\n",p[i].p_id,p[i].waittime);
    }
    for(int i=0;i<currenttime;i++){
        printf("%d ", excute[i]);
    }
    
    printf("Exit : %d\n", currenttime);
    fprintf(fd,"%d",currenttime);
    fclose(fd);
    printf("\n");
    return 0;
}