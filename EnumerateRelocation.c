#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

#ifdef EITHER
int enumerate_relocation(IntDequeue *q,int depth, direction Dir) {
	static int SecondPosition = 0;
	static int MinRelocation = 100;
	static int num_ret = 0;
	int BlockSpace = 0;
	int i = 0;
	int k = 0;
	int PriorityEdge = 0;
	direction dir = Dir;
	direction DirNext = Dir;
	IntDequeue *q_temp = NULL;
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
			q_temp = malloc(STACK*(sizeof *q_temp));
			Array_initialize(q_temp);
			Array_copy(q_temp, q);
			if (PriorityEdge == q[0].que[q[0].min_idx] + 1) {
				Deque(&q_temp[0], &num_ret,dir);

#if TEST==0
				printf("Number Retrieval:%d\n", num_ret);
#endif

				Deque(&q_temp[0], &num_ret,dir);

#if TEST==0
				printf("Priority Retrieval:%d\n", num_ret);
#endif

				insert_media(q_temp, 0);

#if TEST == 0
				Array_print(q_temp);
#endif

				enumerate_relocation(q_temp, depth, DirNext);
				Array_terminate(q_temp);
				free(q_temp);
				return MinRelocation;
			}
			Deque(&q_temp[0],&num_ret, dir);
		}
		Deque(&q[0], &num_ret,dir);
		for (i = 1; i < STACK; i++) {
#if TEST==0
			printf("depth:%d\n", depth);
#endif

			if (DirNext == both) {
				if (IsFull(&q_temp[i])) continue;
				Enque(&q_temp[i], PriorityEdge, dir);


#if TEST==0
				Array_print(q_temp);
#endif

				enumerate_relocation(q_temp, depth, DirNext);
				Array_copy(q_temp, q);

#if TEST==0
				Array_print(q_temp);
#endif

				if (IsEmpty(&q_temp[i])) break;
			}
			else {
				if (IsFull(&q[i])) continue;
				Enque(&q[i], PriorityEdge, dir);


#if TEST==0
				Array_print(q);
#endif

				enumerate_relocation(q, depth, DirNext);
				Deque(&q[i], &num_ret,dir);

#if TEST==0
				Array_print(q);
#endif

				if (IsEmpty(&q[i])) break;
			}
		}
		if (DirNext==both) {
			Array_terminate(q_temp);
			free(q_temp);
		}
		Enque(&q[0], PriorityEdge, dir);
		depth--;
		return MinRelocation;
	}

	while ((q[0].front == q[0].min_idx) || ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx)) {
		if (q[0].front == q[0].min_idx) {
			DequeFront(&q[0],&num_ret);

#if TEST==0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif

		}
		else  if ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx) {
			DequeRear(&q[0],&num_ret);

#if TEST==0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif

		}
		if (insert_sort(q)) {
#if TEST==0
			printf("relocation:%d\n", depth);
#endif

			MinRelocation = depth < MinRelocation ? depth : MinRelocation;
			if (depth == 0) {
				k = MinRelocation;
				MinRelocation = 100;
				return k;
			}
			return MinRelocation;
		}

#if TEST==0
		Array_print(q);
#endif

	}
	k = 0;
	for (i = 0; i < STACK; i++) {
		k += q[i].num;
	}
	if (k <= 2 * STACK) {
		k = lower_bound1(q);
		MinRelocation = (k +depth)< MinRelocation ? (k+depth) : MinRelocation;
		if (depth == 0) {
			k = MinRelocation;
			MinRelocation = 100;
			return k;
		}
		return MinRelocation;
	}
	for (i = 1; i < STACK; i++) BlockSpace += TIER - q[i].num;
	if (nblocking_upper(&q[0]) > BlockSpace);
	else {
		enumerate_relocation(q, depth, upper);
	}

#if TEST==0
	Array_print(q);
#endif

		if (nblocking_lower(&q[0]) > BlockSpace);
		else {
			enumerate_relocation(q, depth, lower);
		}

#if TEST==0
			Array_print(q);
#endif

			if (depth == 0) {
				k = MinRelocation;
				MinRelocation = 100;
			}
		return k;
}

#endif







