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
	int pid; //���μ�����ȣ
	int ar_t; //�����ð�
	int ser_t; //����ð�
	bool is_finished; //�����Ǵܺ���
	int remain_time; //������ �����ִ� �ð�
	int waiting_time; //���ð�
	int end_time; //����ð�
	int put; //���μ��� ���� �Ǵ� ����(0or1)
	int p_return; // ��ȯ�ð�
	float p_nomal_rt; // ����ȭ�� ��ȯ�ð�
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1; //���μ��� ���� ��ȣ

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

// id������ �����ϴ� �Լ�
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
	// ���� ���μ���, ���� �ð�, �ݺ��� i
	int remain, sv_time, i;
	*/
	int remain, min, now_p, i;
	int now = 0;

	now_p = 0;
	remain = n;
	now = p[0].ar_t;

	while (remain > 0) {
		min = 200;
		// ���� ���� �ð� ������ �����ϴ� �κ�
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
			

			// ���� ����� ������ �����ִ� �κ�
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

//fcfs ����_��������
int fcfs_sort(process* a, process* b)
{
	if (a->ar_t > b->ar_t)
		return 1;
	else if (a->ar_t < b->ar_t)
		return -1;
	else
		return 0;
}
//���μ��� ���� Ȯ�� �Լ�
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
//���μ����� ť ���� ���� tail.. head �϶� tail�� �ű�
int prc_enqueue(process* prc)
{
	prc_queue[++q_tail] = prc;

	return 1;
}
//���μ����� ť���� ���� tail..head�϶� 0 1 2 3 4
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


//RR �˰���
void RR(int cnt, process s[]) { //���μ��� ���� cnt
	int t = 0;
	int time_q = 1, time = 0;
	int max_wait_index = 0; //���� ������ٸ� ���μ��� Index
	int clock_inter = 1; //colck ���ͷ�Ʈ �߻� ���� �Ǵ� ����

	int exit = 0; //���� ���μ��� ����
	int average_wait = 0;
	double average_banhwan = 0;
	//���� ��� ����
	FILE* file2 = fopen("output.txt", "w");

	while (1) {
		for (int i = 0; i < cnt; i++) {
			//���� �ð� = �����ð� -> ���μ��� �����Ǵ� ���� 1
			if (g_process[i].ar_t == time) g_process[i].put = 1;

			//���μ��� ���� && Ŭ�����ͷ�Ʈ �߻� 
			if (g_process[i].put == 1 && clock_inter == 1) {

				//ser_t != 0 && ���� ���� ��ٸ� ���μ����� ���ð����� ū ���ð��� ������
				if (g_process[i].ser_t != 0 && g_process[i].waiting_time >= g_process[max_wait_index].waiting_time) {
					//max_wait_index�� �ε��� �� ����
					max_wait_index = i;
				}
			}
		}
		//���� �� ���ð� 0���� �ʱ�ȭ
		g_process[max_wait_index].waiting_time = 0;

		//���� �����Ƿ� ���� �ð� -1
		g_process[max_wait_index].ser_t--;


		for (int i = 0; i < cnt; i++) {
			//������� ���μ���&& ���� �� ���� ���μ����� ���
			if (i != max_wait_index && g_process[i].put == 1) {
				//���ð� +1
				g_process[i].waiting_time++;
			}
		}
		//���� ���� ��ٸ� ���μ����� ����ð��� 0 && 4�� �̻� ���� �� �ߴ�
		if (g_process[max_wait_index].ser_t != 0) {
			clock_inter = 0;
		}
		else {
			//����ð��� 0�� �� ���μ��� ���� ����
			if (g_process[max_wait_index].ser_t == 0) {
				g_process[t].put = 0;

				//���μ��� ���� ���
				printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[t].pid, s[t].ar_t, s[t].ser_t,
					time + 1, time + 1 - s[t].ar_t, (time + 1 - s[t].ar_t) / (double)s[t].ser_t);
				fprintf(file2, "���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
				fprintf(file2, "%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[t].pid, s[t].ar_t, s[t].ser_t,
					time + 1, time + 1 - s[t].ar_t, (time + 1 - s[t].ar_t) / (double)s[t].ser_t);

				exit++;

				average_wait += (time + 1 - s[t].ar_t - s[t].ser_t);
				average_banhwan += ((time + 1 - s[t].ar_t) / (double)s[t].ser_t);
				t++;
			}
			//q=1�� ���� �ð� �� 0���� �ʱ�ȭ

			//���ͷ�Ʈ �߻� ���� ���� 1�� ����
			clock_inter = 1;
		}

		//�ð� +1   
		time++;

		//���μ��� ���� = ����� ���μ��� ���� ->while�� ����
		if (exit == cnt) break;
	}


	printf("\n��� ���ð�: %d\n", average_wait / cnt);
	printf("\n��� ��ȯ�ð�: %.2f\n", average_banhwan / cnt);
	fprintf(file2, "\n��� ���ð�: %d\n", average_wait / cnt);
	fprintf(file2, "\n��� ��ȯ�ð�: %.2f\n", average_banhwan / cnt);
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

	//���� �о����
	file = fopen("input.txt", "r");
	if (file == NULL) {
		printf("������ �������� �ʽ��ϴ�.");
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
	fclose(file); //���� �ݱ�
	printf("input.txt\n");
	//input.txt ���� ���� ���
	for (i = 0; i < count; i++) {
		copy_sys[i] = g_process[i];
		//PID, �����ð�, ����ð�
		printf("%d %d %d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
	}
	puts("===============================================");


	//printf("\n<RR �����ٸ�>\n\n");
	printf("\n<SRT �����ٸ�>\n\n");
	printf("���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
	printf("=======================================================================================================\n");

	// SRT�� �� �κи� ��������ָ� ��.
	//----------------------------
	
	SRT(g_process, count);
	
	//----------------------------
	 
	//RR(count, copy_sys);
	printf("=======================================================================================================\n");


}