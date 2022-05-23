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
	int end_point;
	int recoverycnt;
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1; //���μ��� ���� ��ȣ

//RR �˰���
void RR(int cnt, process s[]) { //���μ��� ���� cnt
	//printf("%d \n",g_process[1].remain_time);
	
	int t = 0, i =0 , end_cnt;
	int flag = 1;
	int time_q = 1, time = 0 ,q = 1 ;

	int exit = 0; //���� ���μ��� ����
	int average_wait = 0;
	double average_banhwan = 0;

	for (i = 0; i < cnt; i++) {
		g_process[i].end_point = 1;
	}

	end_cnt = cnt;

	while (flag) {
		for (i = 0; i < cnt; i++) {
			printf("\n_______service Time : %d_________\n\n",time+1); // ����
		

			if (g_process[i].remain_time > 0 &&g_process[i].ar_t <= time) {
				if (g_process[i].pid == 2 || g_process[i].pid == 4) {
					if (time >5 && time<19) {
						printf("��\n");
						
						//4 start
						 if (g_process[i].pid == 4) {
							if (g_process[1].remain_time == 0 && (g_process[1].end_point == 1)) {
								g_process[1].end_time = time;
								g_process[1].end_point = 0;
								end_cnt -= 1;
								printf("1���Ϸ�");
							}
							else {
								printf("========ID : %d========\n", g_process[1].pid);
								g_process[1].remain_time -= 1;
								printf("������ �����ִ� �ð� �� %d\n", g_process[1].remain_time);
								time++;
								if (g_process[1].remain_time < 0) {
									printf("ȸ��");
									g_process[i].recoverycnt += 1;
								}
							}
						}
						//4 end
						//2 start
						 else if ((g_process[i].pid == 2)) {
							if (g_process[3].remain_time == 0 && (g_process[3].end_point == 1)) {
								g_process[3].end_time = time;
								g_process[3].end_point = 0;
								end_cnt -= 1;
								printf("4���Ϸ�");
							}
							else {
								printf("========ID : %d========\n", g_process[3].pid);
								g_process[3].remain_time -= 1;
								printf("������ �����ִ� �ð� %d\n", g_process[3].remain_time);
								time++;
							}
						}
					//2end

					}
					else {
						printf("========ID : %d========\n", g_process[i].pid);
						printf("������ �����ִ� �ð� %d\n", g_process[i].remain_time);
						g_process[i].remain_time -= 1;
						printf("������ �����ִ� �ð� %d\n", g_process[i].remain_time);
						time++;
					}

				}
				else {
					printf("========ID : %d========\n", g_process[i].pid);
					printf("������ �����ִ� �ð� %d\n", g_process[i].remain_time);
					g_process[i].remain_time -= 1;
					printf("������ �����ִ� �ð� %d\n", g_process[i].remain_time);
					time++;
				}

				

				/*ch*/
				if ((g_process[i].remain_time == 0) && (g_process[i].end_point == 1)) {
					printf("Process ID : %d �Ϸ�\n", g_process[i].pid);
					g_process[i].end_time = time;
					g_process[i].end_point = 0;
					end_cnt -= 1;
				}
				//chend


			}

			else if (end_cnt == 0) {
				flag = 0;
			}

		}

	}

	for (i = 0; i < cnt; i++) {
		if (g_process[i].recoverycnt > 0) {
			g_process[i].end_time = g_process[i].end_time - g_process[i].recoverycnt;
		}
	}

	for (i = 0; i < cnt; i++) {
		printf("%5d%15d%17d%15d%15d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t,
		g_process[i].end_time , g_process[i].end_time-g_process[i].ar_t);
	}

	printf("\n��� ���ð�: %d\n", average_wait / cnt);
	printf("\n��� ��ȯ�ð�: %.2f\n", average_banhwan / cnt);
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


	printf("\n<RR �����ٸ�>\n\n");
	printf("���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
	printf("=======================================================================================================\n");
	RR(count, copy_sys);
	printf("=======================================================================================================\n");


}