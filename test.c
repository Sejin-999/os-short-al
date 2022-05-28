//제출용
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <math.h>


#define LEVEL 5
#define Queue_Max 100

typedef struct process // 프로세스 구조체
{
	int pid;
	int arr_t;
	int serv_t;
	int startFlag;
	int start_t;
	int end_t;
	int banhwan;
	int remain_time; // 남은 시간. 추가
	int waiting_time; // 대기 시간. 추가
	float average_banhwan;

	// RR에서의 실행시간 
	int Time;

}Process;

// 도착시간 순으로 정렬하는 함수
void p_sort(Process* p, int n) {
	Process tmp;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (p[j].arr_t > p[j + 1].arr_t) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
			else if (p[j].arr_t == p[j + 1].arr_t && p[j].pid > p[j + 1].pid) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
		}
	}
}

// id순으로 정렬하는 함수
void p_resort(Process* p, int n) {
	Process tmp;
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
		printf("PID: %d\t도착시간: %d\t서비스시간: %d\t종료시간: %d\t반환시간: %d\t정규화된 반환시간: %.5f\n\n"
			, p[i].pid, p[i].arr_t, p[i].serv_t, p[i].end_t, p[i].banhwan, p[i].average_banhwan);
	}
}

// SRT 알고리즘
void p_srt(Process* p, int n) {
	// 남은 프로세스 수, 최소 남은시간(처음을 제외한), 반복문 i
	// now는 시스템 돌아가는 시간
	int remain, min, now_p, i;
	int now = 0;

	now_p = 0;
	remain = n;
	now = p[0].arr_t;

	// 전에 주신 파일에서는 문제가 없었지만, 문제가 발생하여
	// 아래 for문 추가. 서비스시간 이용해서 남은시간 값 넣어준것.
	for (int i = 0; i < n; i++) {
		p[i].remain_time = p[i].serv_t;
	}
	
	while (remain > 0) {
		min = 200;
		// 남은 동작 시간 순으로 실행하는 부분
		for (i = 0; p[i].arr_t <= now && i < n; i++) {
			if (p[i].remain_time < min && p[i].remain_time > 0) {
				now_p = i;
				min = p[i].remain_time;
			}
		}
		now++;
		p[now_p].remain_time--;
		
		if (p[now_p].remain_time == 0) {
			p[now_p].waiting_time = now - p[now_p].arr_t - p[now_p].serv_t;
			p[now_p].end_t = now;
			p[now_p].banhwan = p[now_p].end_t - p[now_p].arr_t;
			p[now_p].average_banhwan = ((float)p[now_p].waiting_time + (float)p[now_p].serv_t) / (float)p[now_p].serv_t;

			//printf("%d번째 프로세스가 종료되었습니다.\n", n - remain + 1);
			printf("PID: %d\t도착시간: %d\t서비스시간: %d\t종료시간: %d\t반환시간: %d\t정규화된 반환시간: %.5f\n\n"
				, p[now_p].pid, p[now_p].arr_t, p[now_p].serv_t, p[now_p].end_t, p[now_p].banhwan, p[now_p].average_banhwan);
			
			remain--;
		}
	}
}

void SRT(Process* p, int n) {
	printf("\n\n\t\t<<SRT 알고리즘 >>\n\n\n");
	p_sort(p, n);
	p_srt(p, n);
	// id순으로 정렬할 필요 없어보임.
	// p_resort(p, n);
}

// input 텍스트 파일 읽어오기
int DataSet(Process queue[][Queue_Max], Process* processes, int* numOfData)
{


	char line[32] = { 0, };
	FILE* file = fopen("input.txt", "r");
	if (file == NULL)
		return -1;

	int _pid = -1, _arr_t = -1, _serv_t = -1; //여기에 담아서 pid arrv_t serv_t에 넣기
	char tmp = '\0'; //  콤마 받기 위한 변수

	int idxOfProcesses = 0;

	//input.txt에서 데이터를 읽어와 Process배열에 데이터를 순차적으로 저장
	while (1) {
		fscanf(file, "%d %c %d %c %d", &_pid, &tmp, &_arr_t, &tmp, &_serv_t);
		if (feof(file) || _pid == 0) //0일떄 종료
			break;

		processes[idxOfProcesses].pid = _pid;
		processes[idxOfProcesses].arr_t = _arr_t;
		processes[idxOfProcesses].serv_t = _serv_t;
		
		idxOfProcesses++;
	}
	*numOfData = idxOfProcesses; // 읽어온 프로세스의 갯수 저장
	//뒤에서 갯수를 다 사용하면 종료할것
	
	
	int idxOfQueue = 0;
	//순차적으로 저장한 Process배열내에 데이터 중 도착시간이 0 인 프로세스들은 Queue 0 단계에 집어넣는다.
	for (int i = 0; i < idxOfProcesses; ++i)
	{
		if (processes[i].arr_t == 0)
			queue[0][idxOfQueue++] = processes[i];
	}

	fclose(file);
	return idxOfQueue; // 큐에 들어온 프로세스 갯수 반환
}

//큐에 프로세스 삽입하는 함수
int EnQueue(Process queue[Queue_Max], Process process)
{
	for (int i = 0; i < Queue_Max; ++i)
	{
		if (queue[i].pid == 0)	//pid= 0은 초기화값...값이없는지 판단
		{
			queue[i] = process;
			return 1;
		}
	}
	return 0;
}
// 프로세스 종료시 해당 프로세스 삭제
void DeQueue(Process queue[][Queue_Max], int level, int idx) //level: 준비큐 idx: 큐의 인덱스
{
	memset(&queue[level][idx], 0, sizeof(queue[level][idx]));//0으로 함수 초기화

	for (int i = idx; i < Queue_Max - 1; ++i)
		queue[level][i] = queue[level][i + 1];
}
// 정보 출력
void PrintProcess(Process process, int* numOfData)
{
	printf("PID: %d\t도착시간: %d\t서비스시간: %d\t종료시간: %d\t반환시간: %d\t정규화된 반환시간: %.5f\n\n"
		, process.pid, process.arr_t, process.serv_t, process.end_t, process.banhwan, process.average_banhwan);
}

//시간순으로 준비큐0에 삽입
void Arr_enQ(Process queue[][Queue_Max], Process processes[Queue_Max], int time, int* num)
{
	for (int i = 0; i < Queue_Max; ++i)
	{
		if (processes[i].arr_t == time) // 도착 시간과 현재 시간이 같으면 삽입
		{
			int result = EnQueue(queue[0], processes[i]);
			if (result == 1)
				(*num)++;
		}
	}
}



//계산후  종료시간   반환시간   정규화된 반환시간
void ProcessEnd(Process process, int curTime, int* numOfData)
{
	process.end_t = curTime; // 종료시간
	process.banhwan = curTime - process.arr_t; // 반환시간
	process.average_banhwan = (float)process.banhwan / process.serv_t; // 정규화된 반환시간

	PrintProcess(process, numOfData); // 프로세스가 종료될때마다 6가지 내용 출력 (id, 도착시간, 서비스시간 등등)
}

//(q=1)인 피트백 
void Feedback(Process queue[][Queue_Max], Process processes[Queue_Max], int* EnqCount, int* PCount)// 프로세스의 갯수
{
	int time = 0;
	int FinishFlag = 0; // 프로세스가 끝났는지 확인
	// 이 부분 DataSet함수에 있었는데 여기로 옮겼습니다.
	printf("\t\t<<Feedback 알고리즘 q=1(준비큐=5단계)>>\n\n\n");
	while (1)
	{
		if (*PCount == 0) // 남은 프로세스의 갯수가 0 이면 종료
			return;
		FinishFlag = 0;

		for (int level = 0; level < LEVEL; ++level) // 준비큐5개
		{
			for (int pro_n = 0; pro_n < Queue_Max; ++pro_n) // 큐에 저장될수 있는 프로세스의 갯수만큼 반복 최대 100개
			{
				if (queue[level][pro_n].pid != 0) // 프로세스의 id가 0이 아니라면
				{

					int qTime = 1;

					{
						if (queue[level][pro_n].startFlag == 0)
						{
							queue[level][pro_n].startFlag = 1;
							queue[level][pro_n].start_t = time; // 프로세스 시작시간 저장
						}
						queue[level][pro_n].Time++; // 프로세스의 실제 실행시간 1씩 증가

						if (queue[level][pro_n].Time == queue[level][pro_n].serv_t)	//서비스 시간 끝나면 정보 저장
						{
							ProcessEnd(queue[level][pro_n], time + 1, PCount); //프로세스가 끝난 후 처리(정보 계산, 출력 )      (Process process, int curTime, int* numOfData)
							FinishFlag = 1; // 프로세스 끝
							time++;
							Arr_enQ(queue, processes, time, EnqCount); // 현재 시간에 들어오는 프로세스 검사 및 삽입
							(*EnqCount)--; // 큐에 있는 프로세스 갯수 하나 감소
							DeQueue(queue, level, pro_n); // 종료된 프로세스 제거
							(*PCount)--; // 남은 프로세스 갯수 1 감소
							break;
						}
						time++;
						Arr_enQ(queue, processes, time, EnqCount); // 현재 시간에 들어오는 프로세스 검사 및 삽입
					}
					if (FinishFlag == 0) // 시간할당량(q=1) 동안 끝내지 못했을 경우
					{
						if (*EnqCount > 1)
						{
							if (level != 4) //  레벨 0부터 시작 ...4가 마지막 준비큐          >>마지막 준비큐 단계가 아닐경우엔 다음 준비큐로 삽입하고 현재 큐에서 삭제  
							{
								EnQueue(queue[level + 1], queue[level][pro_n]);
								DeQueue(queue, level, pro_n);
							}
						}
						FinishFlag = 1;
					}
				}
				if (FinishFlag)
					break;
			}
			if (FinishFlag)
				break;
		}
	}
}

// 메인함수
int main(void)
{
	Process process[Queue_Max] = { 0, };
	Process queue[LEVEL][Queue_Max] = { 0, };
	Process SRT_p[Queue_Max] = { 0, };
	Process SRT_q[LEVEL][Queue_Max] = { 0, };

	int DataProcess_count = -1;
	int SRT_c = -1;
	int numOfDataInQueue = DataSet(queue, process, &DataProcess_count); // 데이터 읽어온 후 0번 큐에 들어온 프로세스 갯수 저장
	
	Feedback(queue, process, &numOfDataInQueue, &DataProcess_count); // 피드백 큐 시작

	DataSet(SRT_q, SRT_p, &SRT_c);
	SRT(SRT_p, SRT_c);
	
	return 0;
}