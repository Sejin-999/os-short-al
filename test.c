//�����
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <math.h>


#define LEVEL 5
#define Queue_Max 100

typedef struct process // ���μ��� ����ü
{
	int pid;
	int arr_t;
	int serv_t;
	int startFlag;
	int start_t;
	int end_t;
	int banhwan;
	int remain_time; // ���� �ð�. �߰�
	int waiting_time; // ��� �ð�. �߰�
	float average_banhwan;

	// RR������ ����ð� 
	int Time;

}Process;

// �����ð� ������ �����ϴ� �Լ�
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

// id������ �����ϴ� �Լ�
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
		printf("PID: %d\t�����ð�: %d\t���񽺽ð�: %d\t����ð�: %d\t��ȯ�ð�: %d\t����ȭ�� ��ȯ�ð�: %.5f\n\n"
			, p[i].pid, p[i].arr_t, p[i].serv_t, p[i].end_t, p[i].banhwan, p[i].average_banhwan);
	}
}

// SRT �˰���
void p_srt(Process* p, int n) {
	// ���� ���μ��� ��, �ּ� �����ð�(ó���� ������), �ݺ��� i
	// now�� �ý��� ���ư��� �ð�
	int remain, min, now_p, i;
	int now = 0;

	now_p = 0;
	remain = n;
	now = p[0].arr_t;

	// ���� �ֽ� ���Ͽ����� ������ ��������, ������ �߻��Ͽ�
	// �Ʒ� for�� �߰�. ���񽺽ð� �̿��ؼ� �����ð� �� �־��ذ�.
	for (int i = 0; i < n; i++) {
		p[i].remain_time = p[i].serv_t;
	}
	
	while (remain > 0) {
		min = 200;
		// ���� ���� �ð� ������ �����ϴ� �κ�
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

			//printf("%d��° ���μ����� ����Ǿ����ϴ�.\n", n - remain + 1);
			printf("PID: %d\t�����ð�: %d\t���񽺽ð�: %d\t����ð�: %d\t��ȯ�ð�: %d\t����ȭ�� ��ȯ�ð�: %.5f\n\n"
				, p[now_p].pid, p[now_p].arr_t, p[now_p].serv_t, p[now_p].end_t, p[now_p].banhwan, p[now_p].average_banhwan);
			
			remain--;
		}
	}
}

void SRT(Process* p, int n) {
	printf("\n\n\t\t<<SRT �˰��� >>\n\n\n");
	p_sort(p, n);
	p_srt(p, n);
	// id������ ������ �ʿ� �����.
	// p_resort(p, n);
}

// input �ؽ�Ʈ ���� �о����
int DataSet(Process queue[][Queue_Max], Process* processes, int* numOfData)
{


	char line[32] = { 0, };
	FILE* file = fopen("input.txt", "r");
	if (file == NULL)
		return -1;

	int _pid = -1, _arr_t = -1, _serv_t = -1; //���⿡ ��Ƽ� pid arrv_t serv_t�� �ֱ�
	char tmp = '\0'; //  �޸� �ޱ� ���� ����

	int idxOfProcesses = 0;

	//input.txt���� �����͸� �о�� Process�迭�� �����͸� ���������� ����
	while (1) {
		fscanf(file, "%d %c %d %c %d", &_pid, &tmp, &_arr_t, &tmp, &_serv_t);
		if (feof(file) || _pid == 0) //0�ϋ� ����
			break;

		processes[idxOfProcesses].pid = _pid;
		processes[idxOfProcesses].arr_t = _arr_t;
		processes[idxOfProcesses].serv_t = _serv_t;
		
		idxOfProcesses++;
	}
	*numOfData = idxOfProcesses; // �о�� ���μ����� ���� ����
	//�ڿ��� ������ �� ����ϸ� �����Ұ�
	
	
	int idxOfQueue = 0;
	//���������� ������ Process�迭���� ������ �� �����ð��� 0 �� ���μ������� Queue 0 �ܰ迡 ����ִ´�.
	for (int i = 0; i < idxOfProcesses; ++i)
	{
		if (processes[i].arr_t == 0)
			queue[0][idxOfQueue++] = processes[i];
	}

	fclose(file);
	return idxOfQueue; // ť�� ���� ���μ��� ���� ��ȯ
}

//ť�� ���μ��� �����ϴ� �Լ�
int EnQueue(Process queue[Queue_Max], Process process)
{
	for (int i = 0; i < Queue_Max; ++i)
	{
		if (queue[i].pid == 0)	//pid= 0�� �ʱ�ȭ��...���̾����� �Ǵ�
		{
			queue[i] = process;
			return 1;
		}
	}
	return 0;
}
// ���μ��� ����� �ش� ���μ��� ����
void DeQueue(Process queue[][Queue_Max], int level, int idx) //level: �غ�ť idx: ť�� �ε���
{
	memset(&queue[level][idx], 0, sizeof(queue[level][idx]));//0���� �Լ� �ʱ�ȭ

	for (int i = idx; i < Queue_Max - 1; ++i)
		queue[level][i] = queue[level][i + 1];
}
// ���� ���
void PrintProcess(Process process, int* numOfData)
{
	printf("PID: %d\t�����ð�: %d\t���񽺽ð�: %d\t����ð�: %d\t��ȯ�ð�: %d\t����ȭ�� ��ȯ�ð�: %.5f\n\n"
		, process.pid, process.arr_t, process.serv_t, process.end_t, process.banhwan, process.average_banhwan);
}

//�ð������� �غ�ť0�� ����
void Arr_enQ(Process queue[][Queue_Max], Process processes[Queue_Max], int time, int* num)
{
	for (int i = 0; i < Queue_Max; ++i)
	{
		if (processes[i].arr_t == time) // ���� �ð��� ���� �ð��� ������ ����
		{
			int result = EnQueue(queue[0], processes[i]);
			if (result == 1)
				(*num)++;
		}
	}
}



//�����  ����ð�   ��ȯ�ð�   ����ȭ�� ��ȯ�ð�
void ProcessEnd(Process process, int curTime, int* numOfData)
{
	process.end_t = curTime; // ����ð�
	process.banhwan = curTime - process.arr_t; // ��ȯ�ð�
	process.average_banhwan = (float)process.banhwan / process.serv_t; // ����ȭ�� ��ȯ�ð�

	PrintProcess(process, numOfData); // ���μ����� ����ɶ����� 6���� ���� ��� (id, �����ð�, ���񽺽ð� ���)
}

//(q=1)�� ��Ʈ�� 
void Feedback(Process queue[][Queue_Max], Process processes[Queue_Max], int* EnqCount, int* PCount)// ���μ����� ����
{
	int time = 0;
	int FinishFlag = 0; // ���μ����� �������� Ȯ��
	// �� �κ� DataSet�Լ��� �־��µ� ����� �Ű���ϴ�.
	printf("\t\t<<Feedback �˰��� q=1(�غ�ť=5�ܰ�)>>\n\n\n");
	while (1)
	{
		if (*PCount == 0) // ���� ���μ����� ������ 0 �̸� ����
			return;
		FinishFlag = 0;

		for (int level = 0; level < LEVEL; ++level) // �غ�ť5��
		{
			for (int pro_n = 0; pro_n < Queue_Max; ++pro_n) // ť�� ����ɼ� �ִ� ���μ����� ������ŭ �ݺ� �ִ� 100��
			{
				if (queue[level][pro_n].pid != 0) // ���μ����� id�� 0�� �ƴ϶��
				{

					int qTime = 1;

					{
						if (queue[level][pro_n].startFlag == 0)
						{
							queue[level][pro_n].startFlag = 1;
							queue[level][pro_n].start_t = time; // ���μ��� ���۽ð� ����
						}
						queue[level][pro_n].Time++; // ���μ����� ���� ����ð� 1�� ����

						if (queue[level][pro_n].Time == queue[level][pro_n].serv_t)	//���� �ð� ������ ���� ����
						{
							ProcessEnd(queue[level][pro_n], time + 1, PCount); //���μ����� ���� �� ó��(���� ���, ��� )      (Process process, int curTime, int* numOfData)
							FinishFlag = 1; // ���μ��� ��
							time++;
							Arr_enQ(queue, processes, time, EnqCount); // ���� �ð��� ������ ���μ��� �˻� �� ����
							(*EnqCount)--; // ť�� �ִ� ���μ��� ���� �ϳ� ����
							DeQueue(queue, level, pro_n); // ����� ���μ��� ����
							(*PCount)--; // ���� ���μ��� ���� 1 ����
							break;
						}
						time++;
						Arr_enQ(queue, processes, time, EnqCount); // ���� �ð��� ������ ���μ��� �˻� �� ����
					}
					if (FinishFlag == 0) // �ð��Ҵ緮(q=1) ���� ������ ������ ���
					{
						if (*EnqCount > 1)
						{
							if (level != 4) //  ���� 0���� ���� ...4�� ������ �غ�ť          >>������ �غ�ť �ܰ谡 �ƴҰ�쿣 ���� �غ�ť�� �����ϰ� ���� ť���� ����  
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

// �����Լ�
int main(void)
{
	Process process[Queue_Max] = { 0, };
	Process queue[LEVEL][Queue_Max] = { 0, };
	Process SRT_p[Queue_Max] = { 0, };
	Process SRT_q[LEVEL][Queue_Max] = { 0, };

	int DataProcess_count = -1;
	int SRT_c = -1;
	int numOfDataInQueue = DataSet(queue, process, &DataProcess_count); // ������ �о�� �� 0�� ť�� ���� ���μ��� ���� ����
	
	Feedback(queue, process, &numOfDataInQueue, &DataProcess_count); // �ǵ�� ť ����

	DataSet(SRT_q, SRT_p, &SRT_c);
	SRT(SRT_p, SRT_c);
	
	return 0;
}