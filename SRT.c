#include <stdio.h>
#include <stdlib.h>

// 구조체 정의
// 프로세스 id, 도착시간, 서비스 시간, 종료 시간, 반환시간, 대기시간, 남은 동작시간
// 정규화된 반환 시간
// 순으로 정의
typedef struct process {
    int p_id, p_arr, p_sv, p_end, p_return, p_wait, p_remain;
    float p_nomal_rt;
}process;

void p_sort(process* p, int n);
void p_srt(process* p, int n);

int main() {
    int n = 4;
    process p[4];

    p[0].p_id = 1;
    p[0].p_arr = 0;
    p[0].p_sv = 6;
    p[0].p_end = 0;
    p[0].p_return = 0;
    p[0].p_nomal_rt = 0;
    p[0].p_remain = p[0].p_sv;

    p[1].p_id = 2;
    p[1].p_arr = 1;
    p[1].p_sv = 4;
    p[1].p_end = 0;
    p[1].p_return = 0;
    p[1].p_nomal_rt = 0;
    p[1].p_remain = p[1].p_sv;

    p[2].p_id = 3;
    p[2].p_arr = 2;
    p[2].p_sv = 2;
    p[2].p_end = 0;
    p[2].p_return = 0;
    p[2].p_nomal_rt = 0;
    p[2].p_remain = p[2].p_sv;

    p[3].p_id = 4;
    p[3].p_arr = 3;
    p[3].p_sv = 2;
    p[3].p_end = 0;
    p[3].p_return = 0;
    p[3].p_nomal_rt = 0;
    p[3].p_remain = p[3].p_sv;

    p_sort(p, n);
    p_srt(p, n);
    /*
    printf("%d, %d, %d, %d \n", p[0].p_wait, p[1].p_wait, p[2].p_wait, p[3].p_wait);
    printf("%d, %d, %d, %d \n", p[0].p_end, p[1].p_end, p[2].p_end, p[3].p_end);
    printf("%d, %d, %d, %d \n", p[0].p_return, p[1].p_return, p[2].p_return, p[3].p_return);
    printf("%.2f, %.2f, %.2f, %.2f \n", p[0].p_nomal_rt, p[1].p_nomal_rt, p[2].p_nomal_rt, p[3].p_nomal_rt);

    for (int i = 0; i < n; i++) {
       printf("%d, %d, %d, %d, %d, %.2f \n\n", p[i].p_id, p[i].p_arr, p[i].p_sv, p[i].p_end, p[i].p_return, p[i].p_nomal_rt);
    }
    */
}

// 도착 시간 순으로 정렬하는 함수
void p_sort(process* p, int n) {
    process tmp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (p[j].p_arr > p[j + 1].p_arr) {
                tmp = p[j + 1];
                p[j + 1] = p[j];
                p[j] = tmp;
            }
            else if (p[j].p_arr == p[j + 1].p_arr && p[j].p_id > p[j + 1].p_id) {
                tmp = p[j + 1];
                p[j + 1] = p[j];
                p[j] = tmp;
            }
        }
    }
}

void p_srt(process* p, int n) {
    /*
    // 남은 프로세스, 서비스 시간, 반복문 i
    int remain, sv_time, i;
    */
    int remain, min, now_p, i, temp[150];
    int now, wt = 0;

    remain = n;
    now = p[0].p_arr;
    while (remain > 0) {
        min = 200;
        // 여기가 남은시간 구분해주는 곳
        for (i = 0; p[i].p_arr <= now && i < n; i++) {
            if (p[i].p_remain < min && p[i].p_remain > 0) {
                now_p = i;
                min = p[i].p_remain;
            }
        }
        now++;
        p[now_p].p_remain--;

        if (p[now_p].p_remain == 0) {
            p[now_p].p_wait = now - p[now_p].p_arr - p[now_p].p_sv;
            p[now_p].p_end = now;
            p[now_p].p_return = p[now_p].p_end - p[now_p].p_arr;
            p[now_p].p_nomal_rt = ((float)p[now_p].p_wait + (float)p[now_p].p_sv) / (float)p[now_p].p_sv;
            printf("%d, %d, %d, %d, %d, %.2f \n", p[now_p].p_id, p[now_p].p_arr, p[now_p].p_sv, p[now_p].p_end, p[now_p].p_return, p[now_p].p_nomal_rt);
            remain--;
        }
    }
}