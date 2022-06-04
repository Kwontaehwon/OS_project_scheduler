#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    int p_id;
    int arrivetime; //파일에서 입력받는값
    int bursttime; //파일에서 입력받는값
    int waittime;
    int remaintime;
};

int input_data(struct Process* p){
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
            }
            else{
                bursttime = atoi(pdata);
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

void SJFscheduler() {
    struct Process p[100]; //프로세스 리스트
    int num_process = input_data(p); // 파일에서 입력받기 입력받은 프로세스 의 수
    int origin_numprocess = num_process;
    int currenttime = 0;
    int excute[1000] = {0,};
    struct Process ready[100]; //레디배열
    ready[0].p_id = 0;
    int readynum = 0; //레디배열에 있는 프로세스 수
    int processlist[100] = {0,};

    FILE *fd = fopen("pytxt.txt", "w");
    fprintf(fd,"%d\n",num_process);

    while(num_process > 0){
        for(int i=0; i<origin_numprocess; i++){ // 들어온 process를 ready 배열에 넣어준다.
            if(p[i].arrivetime == currenttime){
                memcpy(&ready[readynum], &p[i], sizeof(struct Process));
                readynum++;
                for(int i=0; i<readynum-1; i++){
                     for(int j=i+1; j<readynum; j++){
                        if(ready[i].remaintime > ready[j].remaintime){
                            struct Process temp = ready[i];
                            ready[i] = ready[j];
                            ready[j] = temp;
                        }
                    }
                }
            }
        }
        if(readynum > 0) {
            if(processlist[ready[0].p_id] == 0){ // 실행했던 프로세스 리스트
                processlist[ready[0].p_id] = 1;
                p[ready[0].p_id - 1].waittime = currenttime;
            }
            excute[currenttime] = ready[0].p_id;
            ready[0].remaintime--;
            fprintf(fd,"%d %d %d\n",ready[0].p_id, currenttime, 1);
            if(ready[0].remaintime == 0){
                for(int i=0; i<readynum-1; i++) {
                    memcpy(&ready[i],&ready[i+1],sizeof(struct Process));
                }
                readynum--;
                num_process--;
            }
        } else {
            excute[currenttime] = 0;
        }
        currenttime++;
    }

    
    for(int i=0;i<origin_numprocess;i++){
        printf("p_id %d waiting time : %d\n",p[i].p_id,p[i].waittime);
    }
    for(int i=0;i<currenttime;i++){
        printf("%d ", excute[i]);
    }
    
    printf("Exit : %d\n", currenttime);
    printf("\n");
    fclose(fd);
}

void RRscheduler() {
    struct Process p[100]; //프로세스 리스트
    int num_process = input_data(p); // 파일에서 입력받기 입력받은 프로세스 의 수

    FILE *fd = fopen("pytxt.txt", "w");
    fprintf(fd,"%d\n",num_process);

    int origin_numprocess = num_process;
    int currenttime = 0;
    int timeslice;
    printf("timeslice input\n");
    scanf("%d", &timeslice);
    int excute[1000] = {0,};
    struct Process ready[100]; //레디배열
    ready[0].p_id = 0;
    int readynum = 0; //레디배열에 있는 프로세스 수
    int now_excute = 0; // 실행중인지 확인
    int process_excutetime = 0; //각 프로세스에서 실행중인 시간
    int processlist[100] = {0,}; //프로세스 목록

    while(num_process > 0){
        for(int i=0;i<origin_numprocess;i++){ // 들어온 process를 ready 배열에 넣어준다.
            if(p[i].arrivetime == currenttime){
                memcpy(&ready[readynum], &p[i], sizeof(struct Process));
                readynum++;
            }
        }

        if(now_excute == 0){ //실행중인 프로세스 없음
            if(ready[0].p_id != 0){ //ready에 있는 첫번째 프로세서 실행
                now_excute = 1; //프로세스 실행중으로 바꾸기
                if(processlist[ready[0].p_id] == 0){
                    processlist[ready[0].p_id] = 1;
                    p[ready[0].p_id - 1].waittime = currenttime;
                }
            } else {
                excute[currenttime] = 0;
            }
        }
        
        if(now_excute == 1){ //실행중인 프로세스 있을때
            if(processlist[ready[0].p_id] == 0){ // 실행했던 프로세스 리스트
                processlist[ready[0].p_id] = 1;
                p[ready[0].p_id - 1].waittime = currenttime;
            }
            excute[currenttime] = ready[0].p_id; 
            ready[0].remaintime--;
            process_excutetime++;
            if(ready[0].remaintime == 0){ //레디배열에 첫번째 프로세스의 남은시간이 없을때
                printf("Process %d (%d, %d)\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);
                fprintf(fd,"%d %d %d\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);
                if(readynum > 1){ //레디배열에 대기중인 프로세스가 있을때
                    for(int i=0;i<readynum-1;i++){
                    memcpy(&ready[i],&ready[i+1],sizeof(struct Process)); // 레디배열 한칸씩 앞으로 당기기
                    }
                    ready[readynum-1].p_id = 0;  //레디배열 대기수 하나 줄이기 
                    ready[readynum-1].arrivetime = 0;
                    ready[readynum-1].bursttime = 0;
                    ready[readynum-1].remaintime = 0;
                    ready[readynum-1].waittime = 0;
                } else { //레디배열에 대기중인 프로세스가 없을때
                    ready[0].p_id = 0;  //레디배열 비워주기
                    ready[0].arrivetime = 0; 
                    ready[0].bursttime = 0; 
                    ready[0].remaintime = 0; 
                    ready[0].waittime = 0; 
                }
                readynum--;
                num_process--;
                process_excutetime = 0;
            }

            if(timeslice == process_excutetime){ // 실행중인 프로세스가 타임슬라이스만큼 실행했을때
                //printf("Process %d execution finshed at %d\n",ready[0].p_id, currenttime); // Time Quantum안에 끝나지 않고 다음 프로세스로 넘어가는 경우
                printf("Process %d (%d, %d)\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);
                fprintf(fd,"%d %d %d\n",ready[0].p_id, currenttime-process_excutetime+1 , process_excutetime);

                if(readynum > 1){ //레디배열에 대기 중인 프로세스가 있을때
                    struct Process temp = ready[0];
                    for(int i=0; i<readynum-1; i++){
                        ready[i] = ready[i+1]; //레디배열 앞으로 한칸씩 당기기
                    }
                    ready[readynum-1] = temp; //실행하던 프로세스를 레디배열 맨마지막으로 보내기
                }
                process_excutetime = 0;
            }
        }
        currenttime++;
    }

    
    for(int i=0;i<origin_numprocess;i++){
        printf("p_id %d waiting time : %d\n",p[i].p_id,p[i].waittime);
    }
    for(int i=0;i<currenttime;i++){
        printf("%d ", excute[i]);
    }
    
    printf("Exit : %d\n", currenttime);
    fprintf(fd,"%d",currenttime);
    fclose(fd);
    printf("\n");
}

int main(){
    int input;
    printf("------- TEAM 4 Schedular -------\n");
    printf("--------------------------------\n");
    printf("1. RR scheduler\n");
    printf("2. SJF scheduler\n");
    printf("--------------------------------\n");
    scanf("%d",&input);
    if(input == 1) {
        RRscheduler();
    } else if(input == 2) {
        SJFscheduler();
    } else {
        printf("INPUT 1 or 2\n");
    }
    return 0;
}