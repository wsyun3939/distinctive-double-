#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

#ifdef EITHER
int branch_and_bound(IntDequeue *q, int UB, int UB_cur, int LB, direction Dir)
{
	static int num_ret;
	static int depth = 0;
	static int SecondPosition = 0;
	static int MinRelocation = 0;
	static int LB_temp = 0;
	static int BlockSpace = 0;
	if (UB == UB_cur)
	{
		depth = 0;
		return MinRelocation = UB;
	}
	int i = 0;
	int j = 0;
	int PriorityEdge = 0;
	int TableSize = 0;
	direction dir = Dir;
	direction DirNext = Dir;
	IntDequeue *q_temp = NULL;
	LB_idx *BlockingTable = NULL;
	switch (dir)
	{
	case both:
		qsort(q + 1, STACK - 1, sizeof(IntDequeue), (int (*)(const void *, const void *))pricmp);
		break;
	default:
		depth++;

#if TEST == 0
		printf("Block relocation(depth=%d)\n", depth);
#endif

		LB--;
		printf("LB=%d\n", LB);
		if (dir == lower)
		{
			PriorityEdge = q[0].que[q[0].front];
			SecondPosition = (q[0].front + 1) % q[0].max;
			printf("%d\n", SecondPosition);
		}
		else if (dir == upper)
		{
			PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
			SecondPosition = (q[0].front + q[0].num - 2) % q[0].max;
		}
		if (SecondPosition == q[0].min_idx)
		{
			DirNext = both;
			if (PriorityEdge == q[0].que[q[0].min_idx] + 1)
			{
				q_temp = malloc(STACK * (sizeof *q_temp));
				Array_initialize(q_temp);
				Array_copy(q_temp, q);
				Deque(&q_temp[0], &num_ret, dir);

#if TEST == 0
				printf("Priority Retrieval:%d\n", num_ret);
#endif

				Deque(&q_temp[0], &num_ret, dir);

#if TEST == 0
				printf("Number Retrieval:%d\n", num_ret);
#endif

				insert_media(q_temp, 0);
				if (branch_and_bound(q_temp, UB, UB_cur, LB, both))
				{
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}

#if TEST == 0
				printf("Lower termination.\n");
				Array_print(q_temp);
#endif

				depth--;
				Array_terminate(q_temp);
				free(q_temp);
				return 0;
			}
		}
		BlockingTable = CreateBlockingTable(q, dir, &TableSize);
		// printBlockingTable(BlockingTable, TableSize);
		if (DirNext != both)
		{
			Deque(&q[0], &num_ret, dir);
			for (i = TableSize - 1; i >= 0; i--)
			{
				if (BlockingTable[i].blocking == 0)
				{
					Enque(&q[BlockingTable[i].idx], PriorityEdge, BlockingTable[i].dir);

#if TEST == 0
					Array_print(q);
#endif

					if (branch_and_bound(q, UB, UB_cur, LB, DirNext))
					{
						free(BlockingTable);
						return MinRelocation;
					}
					Deque(&q[BlockingTable[i].idx], &num_ret, BlockingTable[i].dir);
				}
			}
			LB++;
			if (LB + depth <= UB_cur)
			{
				for (i = TableSize - 1; i >= 0; i--)
				{
					if (BlockingTable[i].blocking == 1)
					{
						Enque(&q[BlockingTable[i].idx], PriorityEdge, BlockingTable[i].dir);

#if TEST == 0
						Array_print(q);
#endif
						if (branch_and_bound(q, UB, UB_cur, LB, DirNext))
						{
							free(BlockingTable);
							return MinRelocation;
						}
					}
					Deque(&q[BlockingTable[i].idx], &num_ret, BlockingTable[i].dir);
				}
			}
		}
		else
		{
			q_temp = malloc(STACK * (sizeof *q_temp));
			Array_initialize(q_temp);
			Array_copy(q_temp, q);
			Deque(&q_temp[0], &num_ret, dir);
			for (i = TableSize - 1; i >= 0; i--)
			{
				if (BlockingTable[i].blocking == 1)
					continue;
				Enque(&q_temp[BlockingTable[i].idx], PriorityEdge, BlockingTable[i].dir);

#if TEST == 0
				Array_print(q_temp);
#endif

				if (branch_and_bound(q_temp, UB, UB_cur, LB, DirNext))
				{
					free(BlockingTable);
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}
				Array_copy(q_temp, q);

#if TEST == 0
				Array_print(q_temp);
#endif

				Deque(&q_temp[BlockingTable[i].idx], &num_ret, dir);
			}
			LB++;
			if (LB + depth <= UB_cur)
			{
				for (i = TableSize - 1; i >= 0; i--)
				{
					if (BlockingTable[i].blocking == 0)
						continue;
					Enque(&q_temp[BlockingTable[i].idx], PriorityEdge, BlockingTable[i].dir);

#if TEST == 0
					Array_print(q_temp);
#endif

					if (branch_and_bound(q_temp, UB, UB_cur, LB, DirNext))
					{
						free(BlockingTable);
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					Array_copy(q_temp, q);

#if TEST == 0
					Array_print(q_temp);
#endif

					Deque(&q_temp[BlockingTable[i].idx], &num_ret, dir);
				}
				Array_terminate(q_temp);
				free(q_temp);
			}
		}
		free(BlockingTable);
		depth--;
		return 0;
	}

	while ((q[0].front == q[0].min_idx) || ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx))
	{
		if (q[0].front == q[0].min_idx)
		{
			DequeFront(&q[0], &num_ret);

#if TEST == 0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif
		}
		else if ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx)
		{
			DequeRear(&q[0], &num_ret);

#if TEST == 0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif
		}
		if (insert_sort(q))
		{
			depth = 0;
			return MinRelocation = UB_cur;
		}

#if TEST == 0
		Array_print(q);
#endif
	}
	BlockSpace = 0;
	for (i = 1; i < STACK; i++)
		BlockSpace += TIER - q[i].num;
	dir = q[0].dir;
	if (dir == both)
		dir = q[0].que[q[0].front] < q[0].que[(q[0].front + q[0].num - 1) % q[0].max] ? lower : upper;
	switch (dir)
	{
	case lower:
		if (nblocking_lower(&q[0]) > BlockSpace)
			;
		else
		{
			if (LB + depth == UB_cur - 1)
			{
				j = UpperBound(q, dir) + depth;
				if (j < UB)
					UB = j;
			}
			if (branch_and_bound(q, UB, UB_cur, LB, lower))
			{
				return MinRelocation;
			}
		}

#if TEST == 0
		Array_print(q);
#endif

		if (nblocking_upper(&q[0]) > BlockSpace)
			;
		else
		{
			LB_temp = LB + min_relocation(&q[0], upper) - q[0].LB;
			if (LB_temp + depth > UB_cur)
				;
			else
			{
				if (LB_temp + depth == UB_cur - 1)
				{
					j = UpperBound(q, dir) + depth;
					if (j < UB)
						UB = j;
				}
				if (branch_and_bound(q, UB, UB_cur, LB_temp, upper))
				{
					return MinRelocation;
				}
			}

#if TEST == 0
			Array_print(q);
#endif
		}
		break;
	case upper:
		if (nblocking_upper(&q[0]) > BlockSpace)
			;
		else
		{
			if (LB + depth == UB_cur - 1)
			{
				j = UpperBound(q, dir) + depth;
				if (j < UB)
					UB = j;
			}
			if (branch_and_bound(q, UB, UB_cur, LB, upper))
			{
				return MinRelocation;
			}
		}

#if TEST == 0
		Array_print(q);
#endif

		if (nblocking_lower(&q[0]) > BlockSpace)
			;
		else
		{
			LB_temp = LB + min_relocation(&q[0], lower) - q[0].LB;
			if (LB_temp + depth > UB_cur)
				;
			else
			{
				if (LB_temp + depth == UB_cur - 1)
				{
					j = UpperBound(q, dir) + depth;
					if (j < UB)
						UB = j;
				}
				if (branch_and_bound(q, UB, UB_cur, LB_temp, lower))
				{
					return MinRelocation;
				}
			}
		}

#if TEST == 0
		Array_print(q);
#endif

		break;
	}
	if (depth == 0)
	{
		UB_cur++;

#if TEST == 0
		printf("UB++\n");
#endif

		if (branch_and_bound(q, UB, UB_cur, LB, both))
		{
			return MinRelocation;
		}
		return -1;
	}
	else
	{
		return 0;
	}
}
#endif
