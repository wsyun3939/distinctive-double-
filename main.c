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

int main(void)
{
	clock_t start, end;
	clock_t max = 0;
	IntDequeue *stack = malloc(STACK * (sizeof *stack));
	Array_initialize(stack);
	int nblock = NBLOCK;
	int i, j, x, l;
	int k = 0;
	int sum = 0;
	int gap = 0;
	int max_gap = 0;
	double time = 0;
	int sum_either = 0;
	int LB_sum = 0;
	char filename[BUFFER];
	FILE *fp_csv = NULL;
	for (int a = NUMBER; a < NUMBER + 100 * TIER; a++)
	{
		FILE *fp = NULL;
		sprintf(filename, "../Benchmark/%d-%d-%d/%05d.txt", TIER, STACK, nblock, a);
		printf("%s\n", filename);

		//	読み込みモードでファイルを開く
		fp = fopen(filename, "r");

		start = clock();
		// スタック数と高さを読み込む　//
		fscanf(fp, "%d %d", &i, &j);
		for (i = 0; i < STACK; i++)
		{
			fscanf(fp, "%d ", &l);
			for (j = 0; j < l; j++)
			{
				fscanf(fp, "%d ", &x);
				EnqueRear(&stack[i], x);
			}
		}

		//*---LB1---*//
		int LB1 = lower_bound1(stack);
		printf("LB1:%d\n", LB1);
		// printf("time:%d\n", (double)(end - start) / CLOCKS_PER_SEC);

		qsort(stack, STACK, sizeof(IntDequeue), (int (*)(const void *, const void *))pricmp);
		printf("sort:\n");
		Array_print(stack);
		int depth = 0;
		int UB = UpperBound(stack, both);
		int UB_cur = LB1;
		int min_relocation = branch_and_bound(stack, UB, UB_cur, LB1, both);
		end = clock();
		if (end - start > max)
		{
			max = end - start;
		}
		time += end - start;
		sum += min_relocation;
		gap += min_relocation - LB1;
		if (min_relocation - LB1 > max_gap)
		{
			max_gap = min_relocation - LB1;
		}
		if (min_relocation == LB1)
		{
			k++;
		}
		printf("min_relocation:%d,difference:%d\n", min_relocation, min_relocation - LB1);
		fclose(fp);
		// if (a % 100 == 1)
		// {
		// 	sprintf(filename, "../Benchmark/%d-%d-%d.csv", TIER, STACK, nblock);
		// 	fp_csv = fopen(filename, "r");
		// }
		// fscanf(fp_csv, "%d ", &x);
		// if (x != min_relocation)
		// {
		// 	printf("missmatch\n");
		// 	getchar();
		// }
		Array_clear(stack);
		if (a % 100 == 0)
		{
			nblock++;
			fclose(fp_csv);
		}
	}

	Array_terminate(stack);
	printf("ave_relocation:%f,avg_gap:%f,max_gap:%f,avg_time:%f,max_time:%f\n", (double)sum / (100 * TIER), (double)gap / (100 * TIER), (double)max_gap, (double)time / (CLOCKS_PER_SEC * 100 * TIER), (double)max / CLOCKS_PER_SEC);
	return 0;
}
