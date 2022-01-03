#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "data_info.h"
#include "branch_and_bound.h"

/*下側に位置するブロッキングブロック数*/
int nblocking_lower(IntDequeue *q) {
	if (IsEmpty(q)) {
		return 0;
	}
	else {
		return ((q->min_idx - q->front) >= 0) ? q->min_idx - q->front : q->min_idx - q->front + q->max;
	}
}

/*上側に位置するブロッキングブロック数*/
int nblocking_upper(IntDequeue *q) {
	if (IsEmpty(q)) {
		return 0;
	}
	else {
		return ((q->rear - q->min_idx - 1) >= 0) ? q->rear - q->min_idx - 1 : q->rear - q->min_idx - 1 + q->max;
	}
}

int nblocking(IntDequeue *q, direction dir) {
	switch (dir) {
	case lower:
		return nblocking_lower(q);
	case upper:
		return nblocking_upper(q);
	default:
		return nblocking_lower(q)< nblocking_upper(q)?nblocking_lower(q):nblocking_upper(q);
	}
}


int min_relocation(IntDequeue *q, direction dir) {
	if (q->num <= 3) {
		return nblocking(q, dir);
	}
	int relocation = 0;
	static IntDequeue *q_temp = NULL;
	if (q_temp == NULL) {
		q_temp = malloc(sizeof(*q_temp));
		Initialize(q_temp);
		Copy(q_temp, q);
	}
	switch (dir) {
	case lower:
		relocation=min_relocation_lower(q_temp);
		if (q_temp != NULL) {
			Terminate(q_temp);
			free(q_temp);
			q_temp = NULL;
		}
		return relocation;
		break;
	case upper:
		relocation = min_relocation_upper(q_temp);
		if (q_temp != NULL) {
			Terminate(q_temp);
			free(q_temp);
			q_temp = NULL;
		}
		return relocation;
		break;
	case both:
		while (q_temp->min_idx == q_temp->front || q_temp->min_idx == ((q_temp->rear - 1 < 0) ? q_temp->max - 1 : q_temp->rear - 1)) {
			if (q_temp->min_idx == q_temp->front) RemoveFront(q_temp);
			else if (q_temp->min_idx == ((q_temp->rear - 1 < 0) ? q_temp->max - 1 : q_temp->rear - 1)) RemoveRear(q_temp);
			if (IsEmpty(q_temp)) break;
		}
		if (q_temp->num <= 4) {
			relocation = nblocking(q_temp, dir);
			if (q_temp != NULL) {
				Terminate(q_temp);
				free(q_temp);
				q_temp = NULL;
			}
			return relocation;;
		}
		else {
			retrieval_direction(q_temp, &relocation);
			if (q_temp != NULL) {
				Terminate(q_temp);
				free(q_temp);
				q_temp = NULL;
			}
			return relocation;
		}
	}
	return relocation;
}

int pre_min_relocation(IntDequeue *q, direction dir) {
	if (q->num <= 2) {
		return 0;
	}
	int relocation = 0;
	static IntDequeue *q_temp = NULL;
	if (q_temp == NULL) {
		q_temp = malloc(sizeof(*q_temp));
		Initialize(q_temp);
		Copy(q_temp, q);
	}
	switch (dir) {
	case lower:
		relocation=min_relocation_lower(q_temp);
		if (q_temp != NULL) {
			Terminate(q_temp);
			free(q_temp);
			q_temp = NULL;
		}
		return relocation;
		break;
	case upper:
		relocation = min_relocation_upper(q_temp);
		if (q_temp != NULL) {
			Terminate(q_temp);
			free(q_temp);
			q_temp = NULL;
		}
		return relocation;
		break;
	case both:
		while (q_temp->min_idx == q_temp->front || q_temp->min_idx == ((q_temp->rear - 1 < 0) ? q_temp->max - 1 : q_temp->rear - 1)) {
			if (q_temp->min_idx == q_temp->front) RemoveFront(q_temp);
			else if (q_temp->min_idx == ((q_temp->rear - 1 < 0) ? q_temp->max - 1 : q_temp->rear - 1)) RemoveRear(q_temp);
			if (IsEmpty(q_temp)) break;
		}
		if (q_temp->num <= 2) {
			relocation = 0;
			if (q_temp != NULL) {
				Terminate(q_temp);
				free(q_temp);
				q_temp = NULL;
			}
			return relocation;;
		}
		else {
			pre_retrieval_direction(q_temp, &relocation);
			if (q_temp != NULL) {
				Terminate(q_temp);
				free(q_temp);
				q_temp = NULL;
			}
			return relocation;
		}
	}
	return relocation;
}


int min_relocation_upper(IntDequeue *q) {
	int Nblocking_upper = nblocking_upper(q);
	int relocation = 0;
	for (int i = 0; i <= Nblocking_upper; i++) {
		RemoveRear(q);
	}
	relocation += Nblocking_upper + min_relocation(q, both);
	return relocation;
}

int min_relocation_lower(IntDequeue *q) {
	int Nblocking_lower = nblocking_lower(q);
	int relocation = 0;
	for (int i = 0; i <= Nblocking_lower; i++) {
		RemoveFront(q);
	}
	relocation += Nblocking_lower + min_relocation(q, both);
	return relocation;
}

//*---LB1を計算---*//
int lower_bound1(IntDequeue *q) {
	int LB1 = 0;
	for (int i = 0; i < STACK; i++) {
		LB1 += q[i].LB;
	}
	return  LB1;
}

//*---スタックにおけるpriorityの比較関数---*//
int pricmp(const IntDequeue *n1, const IntDequeue *n2) {
	if (IsEmpty(n1) && IsEmpty(n2))	return 0;
	else if (IsEmpty(n2))	return -1;
	else if (IsEmpty(n1))	return 1;
	else {
		return n1->que[n1->min_idx]<n2->que[n2->min_idx] ? -1 :
			n1->que[n1->min_idx]>n2->que[n2->min_idx] ? 1 : 0;
	}
}

int num_expo(int number) {
	int k = 0;
	number /= 2;
	while (number != 0) {
		k++;
		number /= 2;
	}
	return k;
}
