#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"


#ifdef EITHER
int branch_and_bound(IntDequeue *q, int UB, int UB_cur, int LB, direction Dir, LB_idx *BT, int TableSize) {
	static int BlockingFlag = 0;
	static int DstStack = 0;
	static int num_ret;
	static int depth = 0;
	static int SecondPosition = 0;
	static int MinRelocation = 0;
	static int LB_temp = 0;
	static int BlockSpace = 0;
	static int k = 0;
	static int NumBlocking = 0;
	static int temp = 0;
	if (UB == UB_cur) {
		depth = 0;
		return MinRelocation = UB;
	}
	int i = 0;
	int j = 0;
	int Size = 0;
	int PriorityEdge = 0;
	direction dir = Dir;
	direction DirNext = Dir;
	IntDequeue *q_temp = NULL;
	LB_idx *BlockingTable = NULL;
	LB_idx *LB_next = NULL;
	switch (dir)
	{
	case both:
		qsort(q + 1, STACK - 1, sizeof(IntDequeue), (int(*)(const void *, const void *))pricmp);
		break;
	default:
		depth++;

#if TEST==0
		printf("Block relocation(depth=%d)\n", depth);
#endif

		LB--;
		k = 0;
		if (dir == lower) {
			PriorityEdge = q[0].que[q[0].front];
			SecondPosition = (q[0].front + 1) % q[0].max;
		}
		else if (dir == upper) {
			PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
			SecondPosition = (q[0].front + q[0].num - 2) % q[0].max;
		}
		if (SecondPosition == q[0].min_idx) {
			DirNext = both;
			if (PriorityEdge == q[0].que[q[0].min_idx] + 1) {
				q_temp = malloc(STACK*(sizeof *q_temp));
				Array_initialize(q_temp);
				Array_copy(q_temp, q);
				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("Priority Retrieval:%d\n", num_ret);
#endif

				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("Number Retrieval:%d\n", num_ret);
#endif

				insert_media(q_temp, 0);
				if (branch_and_bound(q_temp, UB, UB_cur, LB, DirNext, NULL, 0)) {
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}

#if TEST==0
				printf("Lower termination.\n");
				Array_print(q_temp);
#endif

				depth--;
				Array_terminate(q_temp);
				free(q_temp);
				return 0;
			}
		}
		if (BlockingFlag) {
			BlockingTable = malloc(TableSize*(sizeof(*BlockingTable)));
			for (i = 0; i < TableSize; i++) {
				BlockingTable[i] = BT[i];
				if (BlockingTable[i].idx == DstStack) {
					BlockingTable[i].blocking = Enque(&q[DstStack], PriorityEdge, dir);
					if (BlockingTable[i].blocking != -1) Deque(&q[DstStack], &num_ret, dir);
				}
				if ((LB + depth == UB_cur)  && (BlockingTable[i].blocking!=0)) k++;
			}
		}
		else {
			BlockingTable = CreateBlockingTable(q, dir, &TableSize);
			if (LB + depth == UB_cur) {
				for (i = 0; i < TableSize; i++) {
					if (BlockingTable[i].blocking) k++;
				}
			}
		}
		if (k == TableSize) {

#if TEST==0 
			switch (dir) {
			case lower:
				printf("Lower termination!\n");
				break;
			case upper:
				printf("Upper termination!\n");
				break;
			}
#endif

			depth--;
			free(BlockingTable);
			return 0;
		}
		if (DirNext != both) {
			Deque(&q[0], &num_ret, dir);
			BlockingFlag = 1;
			LB_next = CreateBlockingTable(q, dir, &Size);
			NumBlocking = 0;
			if (LB + depth == UB_cur) {
				for (i = 0; i < Size; i++) {
					if (LB_next[i].blocking) NumBlocking++;
					else break;
				}
				if (NumBlocking == Size) {
					NumBlocking = 0;
					for (i = 0; i < TableSize; i++) {
						if (BlockingTable[i].blocking==0) {
							Enque(&q[BlockingTable[i].idx], PriorityEdge, dir);
							temp = Enque(&q[BlockingTable[i].idx], q[0].que[SecondPosition], dir);
							if (temp == 0) {
								Deque(&q[BlockingTable[i].idx], &num_ret, dir);
								Deque(&q[BlockingTable[i].idx], &num_ret, dir);
								break;
							}
							else if(temp==1){
								Deque(&q[BlockingTable[i].idx], &num_ret, dir);
								Deque(&q[BlockingTable[i].idx], &num_ret, dir);
								NumBlocking++;
							}
							else {
								Deque(&q[BlockingTable[i].idx], &num_ret, dir);
								NumBlocking++;
							}
						}
					}
					if ((k + NumBlocking) == TableSize) {

#if TEST==0 
						printf("Next termination!\n");
#endif

						depth--;
						Enque(&q[0], PriorityEdge,dir);
						free(BlockingTable);
						free(LB_next);
						return 0;
					}
				}
			}
		}
		else BlockingFlag = 0;
		if (DirNext == both) {
			q_temp = malloc(STACK*(sizeof *q_temp));
			Array_initialize(q_temp);
			Array_copy(q_temp, q);
			Deque(&q_temp[0], &num_ret, dir);
		}
		for (i = TableSize - 1; i >= 0; i--) {
			if (BlockingTable[i].blocking !=0)	continue;
			DstStack = BlockingTable[i].idx;
			if (DirNext == both) {
				Enque(&q_temp[BlockingTable[i].idx], PriorityEdge, dir);

#if TEST==0
				Array_print(q_temp);
#endif

				if (branch_and_bound(q_temp, UB, UB_cur, LB, DirNext, NULL, 0)) {
					free(BlockingTable);
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}
				Array_copy(q_temp, q);

#if TEST==0
				Array_print(q_temp);
#endif

				Deque(&q_temp[0], &num_ret, dir);
			}
			else {
				Enque(&q[BlockingTable[i].idx], PriorityEdge, dir);

#if TEST==0
				Array_print(q);
#endif

				if (branch_and_bound(q, UB, UB_cur, LB, DirNext, LB_next, Size)) {
					free(BlockingTable);
					return MinRelocation;
				}

#if TEST==0
				Array_print(q);
#endif

				Deque(&q[BlockingTable[i].idx], &num_ret, dir);
			}
		}
		LB += 1;

#if TEST==0
		printf("Blocking:\n");
#endif

		if (LB + depth > UB_cur);
		else {
			for (i = TableSize - 1; i >= 0; i--) {
				if (BlockingTable[i].blocking !=1)	continue;
				DstStack = BlockingTable[i].idx;
				if (DirNext == both) {
					Enque(&q_temp[BlockingTable[i].idx], PriorityEdge, dir);

#if TEST==0
					Array_print(q_temp);
#endif

					if (branch_and_bound(q_temp, UB, UB_cur, LB, DirNext, NULL, 0)) {
						free(BlockingTable);
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					Deque(&q_temp[0], &num_ret, dir);
				}
				else {
					Enque(&q[BlockingTable[i].idx], PriorityEdge, dir);

#if TEST==0
					Array_print(q);
#endif

					if (branch_and_bound(q, UB, UB_cur, LB, DirNext, LB_next, Size)) {
						free(BlockingTable);
						return MinRelocation;
					}

#if TEST==0
					Array_print(q);
#endif

					Deque(&q[BlockingTable[i].idx], &num_ret, dir);
				}
			}
		}

#if TEST==0
		printf("Termination.\n");
#endif

		if (DirNext == both) {
			Array_terminate(q_temp);
			free(q_temp);
		}
		else {
			Enque(&q[0], PriorityEdge, dir);
			free(LB_next);
		}
		free(BlockingTable);
		depth--;
		return 0;
	}

	while ((q[0].front == q[0].min_idx) || ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx)) {
		if (q[0].front == q[0].min_idx) {
			DequeFront(&q[0], &num_ret);


#if TEST==0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif


		}
		else  if ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx) {
			DequeRear(&q[0], &num_ret);


#if TEST==0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif


		}
		if (insert_sort(q)) {
			depth = 0;
			return MinRelocation = UB_cur;
		}


#if TEST==0
		Array_print(q);
#endif


	}
	BlockSpace = 0;
	BlockingFlag = 0;
	for (i = 1; i < STACK; i++)	BlockSpace += TIER - q[i].num;
	dir = q[0].dir;
	if(dir==both) dir = q[0].que[q[0].front] < q[0].que[(q[0].front + q[0].num - 1) % q[0].max] ? lower : upper;
	switch (dir)
	{
	case lower:
		if (nblocking_lower(&q[0]) > BlockSpace);
		else {
			if (LB + depth == UB_cur - 1) {
				j = UpperBound(q, dir) + depth;
				if (j < UB) UB = j;
			}
			if (branch_and_bound(q, UB, UB_cur, LB, lower, NULL, 0)) {
				return MinRelocation;
			}
		}

#if TEST==0
		Array_print(q);
#endif

		BlockingFlag = 0;
		if (nblocking_upper(&q[0]) > BlockSpace);
		else {
			LB_temp = LB + min_relocation(&q[0], upper) - q[0].LB;
			if (LB_temp + depth > UB_cur);
			else {
				if (LB_temp + depth == UB_cur - 1) {
					j = UpperBound(q, dir) + depth;
					if (j < UB) UB = j;
				}
				if (branch_and_bound(q, UB, UB_cur, LB_temp, upper, NULL, 0)) {
					return MinRelocation;
				}
			}

#if TEST==0
			Array_print(q);
#endif

		}
		break;
	case upper:
		if (nblocking_upper(&q[0]) > BlockSpace);
		else {
			if (LB + depth == UB_cur - 1) {
				j = UpperBound(q, dir) + depth;
				if (j < UB) UB = j;
			}
			if (branch_and_bound(q, UB, UB_cur, LB, upper, NULL, 0)) {
				return MinRelocation;
			}
		}

#if TEST==0
		Array_print(q);
#endif

		BlockingFlag = 0;
		if (nblocking_lower(&q[0]) > BlockSpace);
		else {
			LB_temp = LB + min_relocation(&q[0], lower) - q[0].LB;
			if (LB_temp + depth > UB_cur);
			else {
				if (LB_temp + depth == UB_cur - 1) {
					j = UpperBound(q, dir) + depth;
					if (j < UB) UB = j;
				}
				if (branch_and_bound(q, UB, UB_cur, LB_temp, lower, NULL, 0)) {
					return MinRelocation;
				}
			}
		}

#if TEST==0
		Array_print(q);
#endif

		break;
	}
	if (depth == 0) {
		UB_cur++;

#if TEST==0
		printf("UB++\n");
#endif

		if (branch_and_bound(q, UB,UB_cur, LB,both, NULL, 0)) {
			return MinRelocation;
		}
		return -1;
	}
	else {
		return 0;
	}
}
#endif







