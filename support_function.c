#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

int insert_sort(IntDequeue *q)
{
	IntDequeue q_temp = q[0];
	int i;
	if (IsEmpty(&q[0]))
	{
		for (i = 1; i < STACK; i++)
		{
			if (IsEmpty(&q[i]))
			{
				break;
			}
			q[i - 1] = q[i];
		}
		q[i - 1] = q_temp;
		if (i == 1)
			return 1;
		return 0;
	}
	else
	{
		for (i = 1; i < STACK; i++)
		{
			if (IsEmpty(&q[i]))
			{
				break;
			}
			if (q_temp.que[q_temp.min_idx] < q[i].que[q[i].min_idx])
			{
				break;
			}
			q[i - 1] = q[i];
		}
		q[i - 1] = q_temp;
		return 0;
	}
}

int insert_media(IntDequeue *q, int stack_idx)
{
	IntDequeue q_temp = q[stack_idx];
	int i = 0;
	if (stack_idx != STACK - 1)
	{
		if (IsEmpty(&q_temp))
		{
			for (i = stack_idx + 1; i < STACK; i++)
			{
				q[i - 1] = q[i];
			}
			q[i - 1] = q_temp;
		}
		else
		{
			for (i = stack_idx + 1; i < STACK; i++)
			{
				if (q_temp.que[q_temp.min_idx] < q[i].que[q[i].min_idx] || IsEmpty(&q[i]))
				{
					break;
				}
				q[i - 1] = q[i];
			}
			q[i - 1] = q_temp;
		}
		if (i != stack_idx + 1)
			return i - 1;
	}
	if (stack_idx != 0)
	{
		if (IsEmpty(&q_temp))
			;
		else
		{
			for (i = stack_idx - 1; i >= 0; i--)
			{
				if (IsEmpty(&q[i]))
					;
				else if (q_temp.que[q_temp.min_idx] > q[i].que[q[i].min_idx])
				{
					break;
				}
				q[i + 1] = q[i];
			}
			q[i + 1] = q_temp;
		}
		return i + 1;
	}
	return 0;
}

//*---優先順位の高くなったスタックを挿入ソート---*//
void insert_front(IntDequeue *q, int stack_idx)
{
	IntDequeue q_temp = q[stack_idx];
	int i = 0;
	if (stack_idx == 0)
		return;
	else
	{
		for (i = stack_idx - 1; i >= 0; i--)
		{
			if (IsEmpty(&q[i]))
				;
			else if (q_temp.que[q_temp.min_idx] > q[i].que[q[i].min_idx])
			{
				break;
			}
			q[i + 1] = q[i];
		}
		q[i + 1] = q_temp;
		return;
	}
}

//*---ブロックを取り出す方向を決める---*//
direction retrieval_direction(IntDequeue *q, int *LB)
{
	int Nblocking_upper = nblocking_upper(q);
	int Nblocking_lower = nblocking_lower(q);
	direction dir;
	if (q->num < 6)
	{
		dir = Nblocking_upper < Nblocking_lower ? upper : Nblocking_upper > Nblocking_lower ? lower
																							: both;
		if (dir == both)
			*LB = Nblocking_lower;
		else
			*LB = min_relocation(q, dir);
	}
	else
	{
		IntDequeue *q_temp = NULL;
		if (Nblocking_lower < Nblocking_upper)
		{
			if ((Nblocking_lower + 1 + num_expo(Nblocking_upper)) < Nblocking_upper)
			{
				dir = lower;
				*LB = min_relocation(q, dir);
			}
			else
			{
				q_temp = malloc(sizeof(*q_temp));
				Initialize(q_temp);
				Copy(q_temp, q);
				//*---上側から取り出した場合の積み替え最小回数---*//
				Nblocking_upper = min_relocation(q, upper);

				//*---下側から取り出した場合の積み替え最小回数---*//
				Nblocking_lower = min_relocation(q_temp, lower);

				Terminate(q_temp);
				free(q_temp);
				dir = Nblocking_upper < Nblocking_lower ? upper : Nblocking_upper > Nblocking_lower ? lower
																									: both;
				if (dir == upper)
					*LB = Nblocking_upper;
				else
					*LB = Nblocking_lower;
			}
		}
		else
		{
			if ((Nblocking_upper + 1 + num_expo(Nblocking_lower)) < Nblocking_lower)
			{
				dir = upper;
				*LB = min_relocation(q, dir);
			}
			else
			{
				q_temp = malloc(sizeof(*q_temp));
				Initialize(q_temp);
				Copy(q_temp, q);
				//*---上側から取り出した場合の積み替え最小回数---*//
				Nblocking_upper = min_relocation(q, upper);

				//*---下側から取り出した場合の積み替え最小回数---*//
				Nblocking_lower = min_relocation(q_temp, lower);

				Terminate(q_temp);
				free(q_temp);
				dir = Nblocking_upper < Nblocking_lower ? upper : Nblocking_upper > Nblocking_lower ? lower
																									: both;
				if (dir == upper)
					*LB = Nblocking_upper;
				else
					*LB = Nblocking_lower;
			}
		}
	}
	return dir;
}

direction pre_retrieval_direction(IntDequeue *q, int *LB)
{
	int Nblocking_upper = nblocking_upper(q);
	int Nblocking_lower = nblocking_lower(q);
	direction dir;
	if (q->num <= 2)
	{
		dir = both;
		*LB = 0;
	}
	else
	{
		IntDequeue *q_temp = NULL;
		if (Nblocking_lower < Nblocking_upper)
		{
			q_temp = malloc(sizeof(*q_temp));
			Initialize(q_temp);
			Copy(q_temp, q);
			//*---上側から取り出した場合の積み替え最小回数---*//
			Nblocking_upper = min_relocation(q, upper);

			//*---下側から取り出した場合の積み替え最小回数---*//
			Nblocking_lower = min_relocation(q_temp, lower);

			Terminate(q_temp);
			free(q_temp);
			dir = Nblocking_upper < Nblocking_lower ? upper : Nblocking_upper > Nblocking_lower ? lower
																								: both;
			if (dir == upper)
				*LB = Nblocking_upper;
			else
				*LB = Nblocking_lower;
		}
		else
		{
			q_temp = malloc(sizeof(*q_temp));
			Initialize(q_temp);
			Copy(q_temp, q);
			//*---上側から取り出した場合の積み替え最小回数---*//
			Nblocking_upper = min_relocation(q, upper);

			//*---下側から取り出した場合の積み替え最小回数---*//
			Nblocking_lower = min_relocation(q_temp, lower);

			Terminate(q_temp);
			free(q_temp);
			dir = Nblocking_upper < Nblocking_lower ? upper : Nblocking_upper > Nblocking_lower ? lower
																								: both;
			if (dir == upper)
				*LB = Nblocking_upper;
			else
				*LB = Nblocking_lower;
		}
	}
	return dir;
}

//*---下界値優先探索のための比較関数---*//
int LBcmp(const LB_idx *n1, const LB_idx *n2)
{
	return n1->blocking < n2->blocking ? -1 : n1->blocking > n2->blocking ? 1
																		  : 0;
}

int UpperBound(const IntDequeue *q, direction dir)
{
	IntDequeue *q_temp = malloc(STACK * (sizeof *q_temp));
	Array_initialize(q_temp);
	Array_copy(q_temp, q);
	int num_ret;
	int i = 0;
	int j = 0;
	int UB = 0;
	int PriorityEdge = 0;
	int NumBlocking = 0;
	direction DirNext = dir;
	LB_idx *BlockingTable = NULL;
	while (!(IsEmpty(&q_temp[0])))
	{
		j = 0;
		switch (DirNext)
		{
		case both:
			while ((q_temp[0].front == q_temp[0].min_idx) || ((q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max == q_temp[0].min_idx))
			{
				if (q_temp[0].front == q_temp[0].min_idx)
				{
					DequeFront(&q_temp[0], &num_ret);
#if UB_TEST == 0
					Array_print(q_temp);
#endif
				}
				else if ((q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max == q_temp[0].min_idx)
				{
					DequeRear(&q_temp[0], &num_ret);
#if UB_TEST == 0
					Array_print(q_temp);
#endif
				}
				if (insert_sort(q_temp))
				{
					Array_terminate(q_temp);
					free(q_temp);
					return UB;
				}
			}
			dir = DirNext = q_temp[0].dir;
			if (dir == both)
				dir = q_temp[0].que[q_temp[0].front] < q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max] ? lower : upper;
			int BlockSpace = 0;
			for (i = 1; i < STACK; i++)
				BlockSpace += TIER - q_temp[i].num;
			switch (dir)
			{
			case lower:
				if (nblocking_lower(&q_temp[0]) > BlockSpace)
				{
					if (nblocking_upper(&q_temp[0]) > BlockSpace)
						return 100;
					else
						dir = DirNext = upper;
				}
				break;
			case upper:
				if (nblocking_upper(&q_temp[0]) > BlockSpace)
				{
					if (nblocking_lower(&q_temp[0]) > BlockSpace)
						return 100;
					else
						dir = DirNext = lower;
				}
				break;
			}
		default:
			UB++;
			if (dir == lower)
			{
				PriorityEdge = q_temp[0].que[q_temp[0].front];
			}
			else if (dir == upper)
			{
				PriorityEdge = q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max];
			}
			NumBlocking = nblocking(q_temp, dir);
			if (NumBlocking == 1)
			{
				DirNext = both;
				if (PriorityEdge == q_temp[0].que[q_temp[0].min_idx] + 1)
				{
					Deque(&q_temp[0], &num_ret, dir);
					Deque(&q_temp[0], &num_ret, dir);
					insert_media(q_temp, 0);
#if UB_TEST == 0
					Array_print(q_temp);
#endif
					break;
				}
			}
			if (IsEmpty(&q_temp[STACK - 1]))
			{
				Deque(&q_temp[0], &num_ret, dir);
				Enque(&q_temp[STACK - 1], num_ret, dir);
				insert_media(q_temp, STACK - 1);
#if UB_TEST == 0
				Array_print(q_temp);
#endif
				break;
			}
			BlockingTable = CreateBlockingTable(q_temp, dir, &j);
			for (i = j - 1; i >= 0; i--)
			{
				if (BlockingTable[i].blocking != 0)
					continue;
				Deque(&q_temp[0], &num_ret, dir);
				Enque(&q_temp[BlockingTable[i].idx], num_ret, BlockingTable[i].dir);
				insert_media(q_temp, BlockingTable[i].idx);
#if UB_TEST == 0
				Array_print(q_temp);
#endif
				break;
			}
			if (i >= 0)
				;
			else
			{
				for (i = j - 1; i >= 0; i--)
				{
					if (BlockingTable[i].blocking != 1)
						continue;
					Deque(&q_temp[0], &num_ret, dir);
					Enque(&q_temp[BlockingTable[i].idx], num_ret, BlockingTable[i].dir);
					insert_media(q_temp, BlockingTable[i].idx);
#if UB_TEST == 0
					Array_print(q_temp);
#endif
					break;
				}
			}
			free(BlockingTable);
			break;
		}
	}
	Array_terminate(q_temp);
	free(q_temp);
	return UB;
}

LB_idx *CreateBlockingTable(IntDequeue *q, direction dir, int *Size)
{
	int *stack_idx = malloc(STACK * sizeof(int));
	int i = 0;
	int j = 0;
	int PriorityEdge = 0;
	for (i = 1; i < STACK; i++)
	{
		if (!(IsFull(&q[i])))
		{
			stack_idx[j] = i;
			j++;
		}
	}
	*Size = 2 * j + 1;
	LB_idx *BlockingTable = malloc(*Size * (sizeof *BlockingTable));
	Deque(&q[0], &PriorityEdge, dir);
	switch (dir)
	{
	case lower:
		//*最上部のブロックを動かした際の下界値*//
		BlockingTable[2 * j].blocking = EnqueRear(&q[0], PriorityEdge);
		BlockingTable[2 * j].idx = 0;
		BlockingTable[2 * j].dir = upper;
		DequeRear(&q[0], &PriorityEdge);
		break;
	case upper:
		//*最下部のブロックを動かした際の下界値*//
		BlockingTable[2 * j].blocking = EnqueFront(&q[0], PriorityEdge);
		BlockingTable[2 * j].idx = 0;
		BlockingTable[2 * j].dir = lower;
		DequeFront(&q[0], &PriorityEdge);
		break;
	}
	// 下側からの積み替え
	for (i = 0; i < j; i++)
	{
		//*最下部のブロックを動かした際の下界値*//
		BlockingTable[i].blocking = EnqueFront(&q[stack_idx[i]], PriorityEdge);
		BlockingTable[i].idx = stack_idx[i];
		BlockingTable[i].dir = lower;
		DequeFront(&q[stack_idx[i]], &PriorityEdge);
	}
	// 上側からの積み替え
	for (i = 0; i < j; i++)
	{
		//*最上部のブロックを動かした際の下界値*//
		BlockingTable[j + i].blocking = EnqueRear(&q[stack_idx[i]], PriorityEdge);
		BlockingTable[j + i].idx = stack_idx[i];
		BlockingTable[j + i].dir = upper;
		DequeRear(&q[stack_idx[i]], &PriorityEdge);
	}
	Enque(&q[0], PriorityEdge, dir);
	free(stack_idx);
	return BlockingTable;
}

// ブロッキングテーブルを表示する
void printBlockingTable(LB_idx *BlockingTable, int Size)
{
	int i = 0;
	for (i = 0; i < Size; i++)
	{
		printf("BlockingTable[%d].blocking = %d\n", i, BlockingTable[i].blocking);
		printf("BlockingTable[%d].idx = %d\n", i, BlockingTable[i].idx);
		printf("BlockingTable[%d].dir = %d\n", i, BlockingTable[i].dir);
	}
}
