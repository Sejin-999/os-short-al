#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESS   100
#define MAX_QUEUE  100



typedef int(*comp)(const void*, const void*);
//tail..........head
typedef struct process {
	int pid; //프로세스번호
	int ar_t; //도착시간
	int ser_t; //수행시간
	bool is_finished; //종료판단변수
	int remain_time; //수행후 남아있는 시간
	int waiting_time; //대기시간
	int end_time; //종료시간
	int put; //프로세스 실행 판단 변수(0or1)
	int p_return; // 반환시간
	float p_nomal_rt; // 정규화된 반환시간
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1; //프로세스 현재 번호

void p_sort(process* p, int n) {
	process tmp;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (p[j].ar_t > p[j + 1].ar_t) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
			else if (p[j].ar_t == p[j + 1].ar_t && p[j].pid > p[j + 1].pid) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
		}
	}
}

// id순으로 정렬하는 함수
void p_resort(process* p, int n) {
	process tmp;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (p[j].pid > p[j + 1].pid) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", p[i].pid, p[i].ar_t, p[i].ser_t,
			p[i].end_time, p[i].p_return, p[i].p_nomal_rt);
	}
	

}

void p_srt(process* p, int n) {
	/*
	// 남은 프로세스, 서비스 시간, 반복문 i
	int remain, sv_time, i;
	*/
	int remain, min, now_p, i;
	int now = 0;

	now_p = 0;
	remain = n;
	now = p[0].ar_t;

	while (remain > 0) {
		min = 200;
		// 남은 동작 시간 순으로 실행하는 부분
		for (i = 0; p[i].ar_t <= now && i < n; i++) {
			if (p[i].remain_time < min && p[i].remain_time > 0) {
				now_p = i;
				min = p[i].remain_time;
			}
		}
		now++;
		p[now_p].remain_time--;
		/*
		if (p[now_p].remain_time == 0) {
			p[now_p].waiting_time = now - p[now_p].ar_t - p[now_p].ser_t;
			p[now_p].end_time = now;
			p[now_p].p_return = p[now_p].end_time - p[now_p].ar_t;
			p[now_p].p_nomal_rt = ((float)p[now_p].waiting_time + (float)p[now_p].ser_t) / (float)p[now_p].ser_t;
			printf("%d, %d, %d, %d, %d, %.2f \n", p[now_p].pid, p[now_p].ar_t, p[now_p].ser_t, p[now_p].end_time, p[now_p].p_return, p[now_p].p_nomal_rt);
			remain--;
		}
		*/
		if (p[now_p].remain_time == 0) {
			p[now_p].waiting_time = now - p[now_p].ar_t - p[now_p].ser_t;
			p[now_p].end_time = now;
			p[now_p].p_return = p[now_p].end_time - p[now_p].ar_t;
			p[now_p].p_nomal_rt = ((float)p[now_p].waiting_time + (float)p[now_p].ser_t) / (float)p[now_p].ser_t;
			

			// 먼저 종료된 순으로 보여주는 부분
			// 
			//printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", p[now_p].pid, p[now_p].ar_t, p[now_p].ser_t,
			//	p[now_p].end_time, p[now_p].p_return, p[now_p].p_nomal_rt);
			remain--;
		}
	}
}

void SRT(process* p, int n) {
	p_sort(g_process, n);
	p_srt(g_process, n);
	p_resort(g_process, n);
}

//fcfs 정렬_순차정렬
int fcfs_sort(process* a, process* b)
{
	if (a->ar_t > b->ar_t)
		return 1;
	else if (a->ar_t < b->ar_t)
		return -1;
	else
		return 0;
}
//프로세스 종료 확인 함수
int is_all_finish()
{
	int i;
	for (i = 0; i <= pr_cnt; i++)
	{
		if (g_process[i].remain_time != 0)
			return 0;
	}
	return 1;
}
//프로세스를 큐 끝에 삽입 tail.. head 일때 tail에 신규
int prc_enqueue(process* prc)
{
	prc_queue[++q_tail] = prc;

	return 1;
}
//프로세스를 큐에서 제거 tail..head일때 0 1 2 3 4
int prc_dequeue()
{
	prc_queue[q_head] = NULL;
	q_head++;
	if (q_head > q_tail)
	{
		return 0;
	}
	return 1;
}


//RR 알고리즘
void RR(int cnt, process s[]) { //프로세스 개수 cnt
	int t = 0;
	int time_q = 1, time = 0;
	int max_wait_index = 0; //가장 오래기다린 프로세스 Index
	int clock_inter = 1; //colck 인터럽트 발생 여부 판단 변수

	int exit = 0; //종료 프로세스 개수
	int average_wait = 0;
	double average_banhwan = 0;
	//파일 출력 정의
	FILE* file2 = fopen("output.txt", "w");

	while (1) {
		for (int i = 0; i < cnt; i++) {
			//현재 시간 = 도착시간 -> 프로세스 실행판단 변수 1
			if (g_process[i].ar_t == time) g_process[i].put = 1;

			//프로세스 생성 && 클럭인터럽트 발생 
			if (g_process[i].put == 1 && clock_inter == 1) {

				//ser_t != 0 && 가장 오래 기다린 프로세스의 대기시간보다 큰 대기시간을 가질때
				if (g_process[i].ser_t != 0 && g_process[i].waiting_time >= g_process[max_wait_index].waiting_time) {
					//max_wait_index의 인덱스 값 저장
					max_wait_index = i;
				}
			}
		}
		//실행 후 대기시간 0으로 초기화
		g_process[max_wait_index].waiting_time = 0;

		//실행 했으므로 수행 시간 -1
		g_process[max_wait_index].ser_t--;


		for (int i = 0; i < cnt; i++) {
			//대기중인 프로세스&& 현재 비 실행 프로세스일 경우
			if (i != max_wait_index && g_process[i].put == 1) {
				//대기시간 +1
				g_process[i].waiting_time++;
			}
		}
		//가장 오래 기다린 프로세스의 수행시간이 0 && 4초 이상 실행 시 중단
		if (g_process[max_wait_index].ser_t != 0) {
			clock_inter = 0;
		}
		else {
			//수행시간이 0일 때 프로세스 실행 종료
			if (g_process[max_wait_index].ser_t == 0) {
				g_process[t].put = 0;

				//프로세스 정보 출력
				printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[t].pid, s[t].ar_t, s[t].ser_t,
					time + 1, time + 1 - s[t].ar_t, (time + 1 - s[t].ar_t) / (double)s[t].ser_t);
				fprintf(file2, "프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
				fprintf(file2, "%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[t].pid, s[t].ar_t, s[t].ser_t,
					time + 1, time + 1 - s[t].ar_t, (time + 1 - s[t].ar_t) / (double)s[t].ser_t);

				exit++;

				average_wait += (time + 1 - s[t].ar_t - s[t].ser_t);
				average_banhwan += ((time + 1 - s[t].ar_t) / (double)s[t].ser_t);
				t++;
			}
			//q=1와 비교할 시간 값 0으로 초기화

			//인터럽트 발생 여부 변수 1로 변경
			clock_inter = 1;
		}

		//시간 +1   
		time++;

		//프로세스 개수 = 종료된 프로세스 개수 ->while문 종료
		if (exit == cnt) break;
	}


	printf("\n평균 대기시간: %d\n", average_wait / cnt);
	printf("\n평균 반환시간: %.2f\n", average_banhwan / cnt);
	fprintf(file2, "\n평균 대기시간: %d\n", average_wait / cnt);
	fprintf(file2, "\n평균 반환시간: %.2f\n", average_banhwan / cnt);
}
int main()
{
	FILE* file = NULL;
	char c[MAX_PROCESS];
	char* a, * b = NULL;

	process copy_sys[MAX_QUEUE] = { 0, };

	int count = 0, i;
	int max_wait_index = 0, one_q = 1, exit = 0;

	memset(g_process, 0, sizeof(g_process));

	//파일 읽어오기
	file = fopen("input.txt", "r");
	if (file == NULL) {
		printf("파일이 존재하지 않습니다.");
		return -1;
	}
	while (fgets(c, MAX_PROCESS, file) != NULL) {
		if (c[0] == '\n') continue;

		a = strtok(c, ",");
		if (*a == '0') {
			break;
		}
		g_process[count].pid = atoi(a);

		a = strtok(NULL, ",");
		g_process[count].ar_t = atoi(a);
		a = strtok(NULL, ",");
		g_process[count].remain_time = g_process[count].ser_t = atoi(a);
		g_process[count].waiting_time = 0;
		count++;

	}
	fclose(file); //파일 닫기
	printf("input.txt\n");
	//input.txt 파일 정보 출력
	for (i = 0; i < count; i++) {
		copy_sys[i] = g_process[i];
		//PID, 도착시간, 수행시간
		printf("%d %d %d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
	}
	puts("===============================================");


	//printf("\n<RR 스케줄링>\n\n");
	printf("\n<SRT 스케줄링>\n\n");
	printf("프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
	printf("=======================================================================================================\n");

	// SRT는 이 부분만 실행시켜주면 됨.
	//----------------------------
	
	SRT(g_process, count);
	
	//----------------------------
	 
	//RR(count, copy_sys);
	printf("=======================================================================================================\n");


}