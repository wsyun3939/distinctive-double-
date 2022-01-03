#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

#define BUFFER 256
/*変数の定義
Stack:スタックの最大個数
Tier:積み上げられるブロックの最大値
Nblocking_lower:priorityの下側に位置するブロック数
Nblocking_upper:priorityの上側に位置するブロック数
stack[i]:i番目のスタック
Nblocking_upper_again:上側から移動させた後もブロッキングブロックとなる個数
Nblocking_lower_again:下側から移動させた後もブロッキングブロックとなる個数

*/

int main(void) {
	clock_t start = clock();
	IntDequeue* stack = malloc(STACK*(sizeof *stack));
	IntDequeue* stack0 = malloc(STACK*(sizeof *stack0));
	Array_initialize(stack);
	Array_initialize(stack0);
	int nblock = NBLOCK;
	int i, j, x, l;
	int k = 0;
	int sum = 0;
	double time=0;
	double time0=0;
	int sum_either = 0;
	int LB_sum = 0;
	char filename[BUFFER];
	FILE *fp_csv = NULL;
		for (int a = NUMBER; a < NUMBER+100*TIER; a++) {
			FILE * fp = NULL;
			sprintf(filename, "/home/lab02/s-watanabe/デスクトップ/BlockRelocationProblem/distinctive-double--1/Benchmark/%d-%d-%d/%05d.txt", TIER, STACK, nblock, a);
			printf("%s\n", filename);

			//	読み込みモードでファイルを開く
			fp=fopen(filename, "r");

			clock_t start = clock();
			// スタック数と高さを読み込む　//
			fscanf(fp, "%d %d", &i, &j);
			for (i = 0; i < STACK; i++) {
				fscanf(fp, "%d ", &l);
				for (j = 0; j < l; j++) {
					fscanf(fp, "%d ", &x);
					EnqueRear(&stack[i], x);
				}
			}

			
			//*---LB1---*//
			int LB1 = lower_bound1(stack);
			clock_t end = clock();
			printf("LB1:%d\n", LB1);
			printf("time:%d\n", (double)(end - start) / CLOCKS_PER_SEC);
			time+=(double)(end - start) / CLOCKS_PER_SEC;

			clock_t start0 = clock();
			// スタック数と高さを読み込む　//
			fscanf(fp, "%d %d", &i, &j);
			for (i = 0; i < STACK; i++) {
				fscanf(fp, "%d ", &l);
				for (j = 0; j < l; j++) {
					fscanf(fp, "%d ", &x);
					pre_EnqueRear(&stack0[i], x);
				}
			}
			//*---LB1---*//
			int LB0 = lower_bound1(stack0);
			clock_t end0 = clock();
			printf("LB0:%d\n", LB1);
			printf("time0:%d\n", (double)(end0 - start0) / CLOCKS_PER_SEC);
			time+=(double)(end0 - start0) / CLOCKS_PER_SEC;
/*
			qsort(stack, STACK, sizeof(IntDequeue), (int(*)(const void *, const void *))pricmp);
			printf("sort:\n");
			Array_print(stack);
			int depth = 0;
			int UB=UpperBound(stack,both);
			int UB_cur = LB1;
			int min_relocation = branch_and_bound(stack, UB, UB_cur, LB1, both, NULL, 0);
			//int min_relocation = enumerate_relocation(stack, depth,both);
			sum += min_relocation;
			if (min_relocation == LB1) {
				k++;
			}
			printf("min_relocation:%d,difference:%d\n", min_relocation, min_relocation - LB1); 
			fclose(fp);
			if (a % 100 == 1) {
				sprintf(filename, "C:/Users/wsyun/source/repos/Block relocation problem/Benchmark/%d-%d-%d.csv", TIER, STACK, nblock);
				fp_csv=fopen(filename, "r");
			}
			fscanf(fp_csv, "%d ", &x);
			if (x != min_relocation) {
				printf("missmatch\n");
				getchar();
			}
			*/
		Array_clear(stack);
		if (a % 100 == 0) {
				nblock++;
				fclose(fp_csv);
			}
		}
	
	Array_terminate(stack);
	printf("pre_time:%f,time:%f\n",time0/(100*TIER),time/(100*TIER));
	// printf("time:%f,match:%d,ave_relocation:%f\n", (double)(end - start) / CLOCKS_PER_SEC, k,(double)sum/(100*TIER));
	return 0;
}
