#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define MAX_SIZE 99
#define MAX_TIME_SIZE 30
#define MAX_SERVICETIME_SIZE 10

struct queqe {
	int proId;
	int arrTime;
	int serviceTime;
	int checkCom;
};

void main() {
	struct queqe z[MAX_SIZE];
	int a, b, c, rs = 1;
	int flag = 1, flag2 = 1;
	int q = 1;

	while (flag) {
		printf("Id");
		scanf("%d", &a);
		if (a != 0) {
			printf("arr");
			scanf("%d", &b);
			printf("ser");
			scanf("%d", &c);
			z[rs].proId = a;
			z[rs].arrTime = b;
			z[rs].serviceTime = c;
			z[rs].checkCom = 1;
			rs++;
		}
		else {
			flag = 0;
		}
	}
	for (int i = 1; i <= rs - 1; i++) {
		printf("%d  %d  %d\n", z[i].proId, z[i].arrTime, z[i].serviceTime);
	}

	printf("\nRR\n");
	int comfile = rs - 1;

	while (flag2) {
		int cnt = 0;
		int j = 1;
		int sel5;

		

		// Service Time q-> Servie start
		for (j = j; j < rs; j++) {

			// comfile start
			//printf("comfile %d\n", comfile);

			if (z[j].checkCom == 0) {
				printf("나오는데? %d\n",comfile);
				comfile -= 1;
			}

			/*
			for (int k = 1; k < rs; k++) {
				if (comfile == 0) {
					flag2 = 0;
				}
				else if (z[k].checkCom == 0) {
					comfile = comfile - 1;
				}
				else {
					comfile = rs - 1;
					break;
				}

				printf("comfile %d", comfile);
			}*/
			// comfile end

			if (z[j].serviceTime == 0) {
				printf("Process ID : %d 는 완료했습니다\n", z[j].proId);
				z[j].checkCom = 0;
			}
			else if (z[j].serviceTime < 0) {
				printf("ERROR 다시확인하시오\n");
				break;
			}
			else {
				printf("ID : %d\n", z[j].proId);
				printf("남은 서비스 시간 : %d\n", z[j].serviceTime);
				z[j].serviceTime -= q;
				printf("실행 후 남은 서비스 시간 : %d\n", z[j].serviceTime);
			}

		}
		// Service Time q-> Servie END}
		

		
		printf("계속하려면 1");
		scanf("%d", &sel5);
		if (sel5 == 1) {
			continue;
		}
		else {
			flag2 = 0;
		}
		
	}


}




/*
int a,b,c,flag=1;

while(flag){
	scanf("%d", &a);
	if (a != 0) {
		scanf("%d", &b);
		scanf("%d", &c);
		int save[MAX_ID_SIZE] = a, b, c;
	}
	else {
		break;
	}

}
printf("%d  %d  %d",);
*/

/*
		if (cnt == rs - 1) {
			flag2 = 0;
			for (int i = 1; i <= rs - 1; i++) {
				// 만약 service time 이 0 이되면 cnt 스택을 쌓고 이 스택+1 rs와 같아지면 Break 하면 되겠군
				printf("%d  %d  %d\n", z[i].proId, z[i].arrTime, z[i].serviceTime);
			}
		}*/