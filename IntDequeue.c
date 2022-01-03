/* int型デックIntDequeue（ソース部）*/

#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "data_info.h"
#include "lower_bound.h"
#include "branch_and_bound.h"

/*--- キューの初期化 ---*/
int Initialize(IntDequeue *q)
{
	q->num = q->front = q->rear = q->min_idx  = q->LB = 0;
	q->dir = both;
	if ((q->que = calloc(TIER, sizeof(int))) == NULL) {
		q->max = 0;								/* 配列の確保に失敗 */
		return -1;
	}
	q->max = TIER;
	return 0;
}

/*---配列キューの初期化---*/
void Array_initialize(IntDequeue *q) {
	for (int i = 0; i < STACK; i++) {
		Initialize(&q[i]);
	}
}

/*--- キューの先頭にデータをエンキュー ---*/
int EnqueFront(IntDequeue *q, int x)
{
	if (q->num >= q->max)
		return -1;							/* キューは満杯 */
	else {
		q->num++;
		if (--q->front < 0)
			q->front = q->max - 1;
		q->que[q->front] = x;
		if (q->num == 1) {			/*初めてエンキューされた時はそこが最小値*/
			q->min_idx = q->front;
			return 0;
		}
		else if (x < q->que[q->min_idx]) {
			q->min_idx = q->front;
			q->dir = lower;
			return 0;
		}
		else;
	}
	switch (q->dir) {
	case lower:
		q->LB++;
		if (q->LB == min_relocation(q,upper)) q->dir = both;
		return 1;
	case upper:
		if (q->LB == min_relocation(q,upper)) return 0;
		else {
			q->LB++;
			return 1;
		}
	case both:
		if (q->LB == min_relocation(q,upper)) {
			q->dir = upper;
			return 0;
		}
		else {
			q->LB++;
			return 1;
		}
	}
	return -1;
}

/*--- キューの末尾にデータをエンキュー ---*/
int EnqueRear(IntDequeue *q, int x)
{
	if (q->num >= q->max)
		return -1;							/* キューは満杯 */
	else {
		q->num++;
		if (q->num == 1) {
			q->min_idx = q->rear;
			q->que[q->rear++] = x;
			if (q->rear == q->max)
				q->rear = 0;
			return 0;
		}
		else if (x < q->que[q->min_idx]) {
			q->min_idx = q->rear;
			q->que[q->rear++] = x;
			if (q->rear == q->max)
				q->rear = 0;
			q->dir = upper;
			return 0;
		}
		q->que[q->rear++] = x;
		if (q->rear == q->max)
			q->rear = 0;
	}
	switch (q->dir) {
	case upper:
		q->LB++;
		if (q->LB == min_relocation(q,lower)) q->dir = both;
		return 1;
	case lower:
		if (q->LB == min_relocation(q,lower)) return 0;
		else {
			q->LB++;
			return 1;
		}
	case both:
		if (q->LB == min_relocation(q,lower)) {
			q->dir = lower;
			return 0;
		}
		else {
			q->LB++;
			return 1;
		}
	}
	return -1;
}

int pre_EnqueFront(IntDequeue *q, int x)
{
	if (q->num >= q->max)
		return -1;							/* キューは満杯 */
	else {
		q->num++;
		if (--q->front < 0)
			q->front = q->max - 1;
		q->que[q->front] = x;
		if (q->num == 1) {			/*初めてエンキューされた時はそこが最小値*/
			q->min_idx = q->front;
			return 0;
		}
		else if (x < q->que[q->min_idx]) {
			q->min_idx = q->front;
			q->dir = lower;
			return 0;
		}
		else;
	}
	switch (q->dir) {
	case lower:
		q->LB++;
		if (q->LB == pre_min_relocation(q,upper)) q->dir = both;
		return 1;
	case upper:
		if (q->LB == pre_min_relocation(q,upper)) return 0;
		else {
			q->LB++;
			return 1;
		}
	case both:
		if (q->LB == pre_min_relocation(q,upper)) {
			q->dir = upper;
			return 0;
		}
		else {
			q->LB++;
			return 1;
		}
	}
	return -1;
}

int pre_EnqueRear(IntDequeue *q, int x)
{
	if (q->num >= q->max)
		return -1;							/* キューは満杯 */
	else {
		q->num++;
		if (q->num == 1) {
			q->min_idx = q->rear;
			q->que[q->rear++] = x;
			if (q->rear == q->max)
				q->rear = 0;
			return 0;
		}
		else if (x < q->que[q->min_idx]) {
			q->min_idx = q->rear;
			q->que[q->rear++] = x;
			if (q->rear == q->max)
				q->rear = 0;
			q->dir = upper;
			return 0;
		}
		q->que[q->rear++] = x;
		if (q->rear == q->max)
			q->rear = 0;
	}
	switch (q->dir) {
	case upper:
		q->LB++;
		if (q->LB == pre_min_relocation(q,lower)) q->dir = both;
		return 1;
	case lower:
		if (q->LB == pre_min_relocation(q,lower)) return 0;
		else {
			q->LB++;
			return 1;
		}
	case both:
		if (q->LB == pre_min_relocation(q,lower)) {
			q->dir = lower;
			return 0;
		}
		else {
			q->LB++;
			return 1;
		}
	}
	return -1;
}

int pre_Enque(IntDequeue *q, int x, direction dir)
{
	switch (dir) {
	case lower:
		return pre_EnqueFront(q, x);
	case upper:
		return pre_EnqueRear(q, x);
	default: return -1;
	}
}

int Enque(IntDequeue *q, int x, direction dir)
{
	switch (dir) {
	case lower:
		return EnqueFront(q, x);
	case upper:
		return EnqueRear(q, x);
	default: return -1;
	}
}

int PushFront(IntDequeue *q, int x) {
	if (q->num >= q->max)
		return -1;							/* キューは満杯 */
	else {
		q->num++;
		if (--q->front < 0)
			q->front = q->max - 1;
		q->que[q->front] = x;
		if (q->num == 1) {			/*初めてエンキューされた時はそこが最小値*/
			q->min_idx = q->front;
			return 0;
		}
		else if (x < q->que[q->min_idx]) {
			q->min_idx = q->front;
			return 0;
		}
		return 0;
	}
}

int PushRear(IntDequeue *q, int x) {
	if (q->num >= q->max)
		return -1;							/* キューは満杯 */
	else {
		q->num++;
		if (q->num == 1) {
			q->min_idx = q->rear;
		}
		else if (x < q->que[q->min_idx]) {
			q->min_idx = q->rear;
		}
		q->que[q->rear++] = x;
		if (q->rear == q->max)
			q->rear = 0;
		return 0;
	}
}

int Push(IntDequeue *q, int x, direction dir) {
	switch(dir) {
	case lower:
			return PushFront(q, x);
		case upper:
			return PushRear(q, x);
		default:
			return - 1;
	}
}

/*--- キューの先頭からデータをデキュー ---*/
int DequeFront(IntDequeue *q, int *x)
{
	if (q->num <= 0)						/* キューは空 */
		return -1;
	else {
		q->num--;
		*x = q->que[q->front++];
		if (q->front == q->max)
			q->front = 0;
		if (IsEmpty(q)) {
			q->min_idx = q->front;
			q->dir = both;
			q->LB = 0;
			return 0;
		}
		else if (*x == q->que[q->min_idx]) {
			q->min_idx = SearchMin(q);
			q->dir = retrieval_direction(q,&q->LB);
			return 0;
		}
	}
	switch (q->dir) {
	case lower:
		q->LB--;
		return 0;
	case upper:
		if ((q->LB - 1) == min_relocation(q,upper)) {
			q->LB--;
			return 0;
		}
		else {
			if (min_relocation(q,lower) == q->LB) q->dir = both;
			return 0;
		}
	case both:
		q->LB--;
		if (q->LB == min_relocation(q,upper)) {
			return 0;
		}
		else {
			q->dir = lower;
			return 0;
		}
	}
	return -1;
}

/*--- キューの末尾からデータをデキュー ---*/
int DequeRear(IntDequeue *q, int *x)
{
	if (q->num <= 0)						/* キューは空 */
		return 1;
	else {
		q->num--;
		if (--q->rear < 0)
			q->rear = q->max - 1;
		*x = q->que[q->rear];
		if (IsEmpty(q)) {
			q->LB = 0;
			q->dir = both;
			return 0;
		}
		else if (*x == q->que[q->min_idx]) {
			q->min_idx = SearchMin(q);
			q->dir = retrieval_direction(q,&q->LB);
			return 0;
		}
	}
	switch (q->dir) {
	case upper:
		q->LB--;
		return -1;
	case lower:
		if ((q->LB - 1) == min_relocation(q,lower)) {
			q->LB--;
			return -1;
		}
		else {
			if (min_relocation(q,upper) == q->LB) q->dir = both;
			return 0;
		}
	case both:
		q->LB--;
		if (q->LB == min_relocation(q,lower)) {
			return -1;
		}
		else {
			q->dir = upper;
			return -1;
		}
	}
	return 1;
}

int Deque(IntDequeue *q, int *x, direction dir)
{
	switch (dir) {
	case lower:
		return DequeFront(q, x);
	case upper:
		return DequeRear(q, x);
	default: return -1;
	}
}

int pre_DequeFront(IntDequeue *q, int *x)
{
	if (q->num <= 0)						/* キューは空 */
		return -1;
	else {
		q->num--;
		*x = q->que[q->front++];
		if (q->front == q->max)
			q->front = 0;
		if (IsEmpty(q)) {
			q->min_idx = q->front;
			q->dir = both;
			q->LB = 0;
			return 0;
		}
		else if (*x == q->que[q->min_idx]) {
			q->min_idx = SearchMin(q);
			q->dir = pre_retrieval_direction(q,&q->LB);
			return 0;
		}
	}
	switch (q->dir) {
	case lower:
		q->LB--;
		return 0;
	case upper:
		if ((q->LB - 1) == pre_min_relocation(q,upper)) {
			q->LB--;
			return 0;
		}
		else {
			if (pre_min_relocation(q,lower) == q->LB) q->dir = both;
			return 0;
		}
	case both:
		q->LB--;
		if (q->LB == pre_min_relocation(q,upper)) {
			return 0;
		}
		else {
			q->dir = lower;
			return 0;
		}
	}
	return -1;
}

/*--- キューの末尾からデータをデキュー ---*/
int pre_DequeRear(IntDequeue *q, int *x)
{
	if (q->num <= 0)						/* キューは空 */
		return 1;
	else {
		q->num--;
		if (--q->rear < 0)
			q->rear = q->max - 1;
		*x = q->que[q->rear];
		if (IsEmpty(q)) {
			q->LB = 0;
			q->dir = both;
			return 0;
		}
		else if (*x == q->que[q->min_idx]) {
			q->min_idx = SearchMin(q);
			q->dir = pre_retrieval_direction(q,&q->LB);
			return 0;
		}
	}
	switch (q->dir) {
	case upper:
		q->LB--;
		return -1;
	case lower:
		if ((q->LB - 1) == pre_min_relocation(q,lower)) {
			q->LB--;
			return -1;
		}
		else {
			if (pre_min_relocation(q,upper) == q->LB) q->dir = both;
			return 0;
		}
	case both:
		q->LB--;
		if (q->LB == pre_min_relocation(q,lower)) {
			return -1;
		}
		else {
			q->dir = upper;
			return -1;
		}
	}
	return 1;
}

int pre_Deque(IntDequeue *q, int *x, direction dir)
{
	switch (dir) {
	case lower:
		return pre_DequeFront(q, x);
	case upper:
		return pre_DequeRear(q, x);
	default: return -1;
	}
}

int RemoveFront(IntDequeue *q) {
	if (q->num <= 0)						/* キューは空 */
		return -1;
	else {
		q->num--;
		int x = q->que[q->front++];
		if (q->front == q->max)
			q->front = 0;
		if (IsEmpty(q)) {
			q->min_idx = q->front;
			return 0;
		}
		else if (x == q->que[q->min_idx]) {
			q->min_idx = SearchMin(q);
			return 0;
		}
		return 0;
	}
}

int RemoveRear(IntDequeue *q) {
	if (q->num <= 0)						/* キューは空 */
		return -1;
	else {
		q->num--;
		if (--q->rear < 0)
			q->rear = q->max - 1;
		int x = q->que[q->rear];
		if (IsEmpty(q)) {
			return 0;
		}
		else if (x == q->que[q->min_idx]) {
			q->min_idx = SearchMin(q);
			return 0;
		}
		return 0;
	}
}

int Remove(IntDequeue *q, direction dir) {
	switch (dir) {
	case lower:
		return RemoveFront(q);
		break;
	case upper:
		return RemoveRear(q);
		break;
	default:
		return -1;
	}
}

/*--- キューの先頭からデータをピーク ---*/
int PeekFront(const IntDequeue *q, int *x)
{
	if (q->num <= 0)							/* キューは空 */
		return -1;
	*x = q->que[q->front];
	return 0;
}

/*--- キューの末尾からデータをピーク ---*/
int PeekRear(const IntDequeue *q, int *x)
{
	if (q->num <= 0)							/* キューは空 */
		return -1;
	*x = q->que[q->rear];
	return 0;
}

/*--- 全データを削除 ---*/
void Clear(IntDequeue *q)
{
	q->num = q->front = q->rear = q->min_idx=q->LB=0;
	q->dir = both;
}

/*---配列キューの全データを削除---*/
void Array_clear(IntDequeue *q) {
	for (int i = 0; i < STACK; i++) {
		Clear(&q[i]);
	}
}

/*--- キューの容量 ---*/
int Capacity(const IntDequeue *q)
{
	return q->max;
}

/*--- キューに蓄えられているデータ数 ---*/
int Size(const IntDequeue *q)
{
	return q->num;
}

/*--- キューは空か ---*/
int IsEmpty(const IntDequeue *q)
{
	return q->num <= 0;
}

/*--- キューは空か ---*/
int IsArrayEmpty(const IntDequeue *q)
{
	int i = 0;
	for (i = 0; i < STACK; i++) {
		if (!(IsEmpty(&q[i]))) break;
	}
	if (i == STACK) return 1;
	else return 0;
}

/*--- キューは満杯か ---*/
int IsFull(const IntDequeue *q)
{
	return q->num >= q->max;
}

/*--- キューからの探索 ---*/
int Search(const IntDequeue *q, int x)
{
	int i, idx;

	for (i = 0; i < q->num; i++) {
		if (q->que[idx = (i + q->front) % q->max] == x)
			return idx;		/* 探索成功 */
	}
	return -1;				/* 探索失敗 */
}

/*--- キューからの探索（論理的添字を返却）---*/
int Search2(const IntDequeue *q, int x)
{
	int i;

	for (i = 0; i < q->num; i++) {
		if (q->que[(i + q->front) % q->max] == x)
			return i;		/* 探索成功 */
	}
	return -1;				/* 探索失敗 */
}

/*--- 全データの表示 ---*/
void Print(const IntDequeue *q)
{
	int i;

	for (i = 0; i < q->num; i++)
		printf("%3d ", q->que[(i + q->front) % q->max]);
	putchar('\n');
}

/*---配列キューの表示---*/
void Array_print(const IntDequeue *q) {
	for (int i = 0; i < STACK; i++) {
		printf("stack[%d]:", i);
		Print(&q[i]);
	}
	putchar('\n');
}

/*--- キューの後始末 ---*/
void Terminate(IntDequeue *q)
{
	if (q->que != NULL)
		free(q->que);							/* 配列を解放 */
	q->max = q->num = q->front = q->rear = q->min_idx = q->dir = q->LB = 0;
}

/*---キューの最小値を探索---*/
int SearchMin(IntDequeue *q) {
	int i;
	int k = q->front;
	int min=q->que[q->front];
	for (i = 1; i < q->num; i++) {
		if (q->que[(i + q->front) % q->max] < min) {
			min = q->que[(i + q->front) % q->max];
			k = (i + q->front) % q->max;
		}
	}
	return k;
}

/*---キューのコピー---*/
void Copy(IntDequeue *q1,const IntDequeue *q2) {
	q1->front = q2->front;
	q1->min_idx = q2->min_idx;
	q1->num = q2->num;
	q1->rear = q2->rear;
	q1->dir = q2->dir;
	q1->LB = q2->LB;
	for (int i = 0; i < q2->num; i++) {
		q1->que[(i + q2->front) % q2->max] = q2->que[(i + q2->front) % q2->max];
	}
}

/*---配列キューのコピー---*/
void Array_copy(IntDequeue *q1,const IntDequeue *q2) {
	for (int i = 0; i < STACK; i++) {
		Copy(&q1[i], &q2[i]);
	}
}

/*---配列キューの後始末---*/
void Array_terminate(IntDequeue *q) {
	for (int i = 0; i < STACK; i++) {
		Terminate(&q[i]);
	}
}

/*---キューを交換する---*/
void Swap_IntDequeue(IntDequeue *q1, IntDequeue *q2) {
	IntDequeue *q_temp = malloc(sizeof(*q_temp));
	Initialize(q_temp);
	Copy(q_temp, q1);
	Copy(q1, q2);
	Copy(q2, q_temp);
	Terminate(q_temp);
	free(q_temp);
}
