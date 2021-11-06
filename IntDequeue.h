#ifndef ___IntDequeue
#define ___IntDequeue
#include "data_info.h"

/*--- キューを実現する構造体 ---*/
typedef struct {
	int max;		/* キューの容量 */
	int num;		/* 現在の要素数 */
	int front;		/* 先頭要素カーソル */
	int rear;		/* 末尾要素カーソル */
	int *que;		/* キュー本体（の先頭要素へのポインタ）*/
	int min_idx;/*キューの中で最小値をとる添え字へのカーソル*/
	direction dir; /*取り出すときにお得な方向*/
	int LB;
} IntDequeue;

/*--- キューの初期化 ---*/
int Initialize(IntDequeue *q);

/*---配列キューの初期化---*/
void Array_initialize(IntDequeue *q);

/*--- キューの先頭にデータをエンキュー (下界値計算も行う)---*/
int EnqueFront(IntDequeue *q, int x);

/*--- キューの末尾にデータをエンキュー(下界値計算も行う) ---*/
int EnqueRear(IntDequeue *q, int x);

int Enque(IntDequeue *q, int x, direction dir);

int PushFront(IntDequeue *q, int x);

int PushRear(IntDequeue *q, int x);

int Push(IntDequeue *q, int x,direction dir);

/*--- キューの先頭からデータをデキュー(下界値計算も行う) ---*/
int DequeFront(IntDequeue *q, int *x);

/*--- キューの末尾からデータをデキュー(下界値計算も行う) ---*/
int DequeRear(IntDequeue *q, int *x);

int Deque(IntDequeue *q, int *x, direction dir);

/*---先頭のデータを削除---*/
int RemoveFront(IntDequeue *q);

/*---末尾のデータを削除---*/
int RemoveRear(IntDequeue *q);

int Remove(IntDequeue *q, direction dir);

/*--- キューの先頭からデータをピーク ---*/
int PeekFront(const IntDequeue *q, int *x);

/*--- キューの末尾からデータをピーク ---*/
int PeekRear(const IntDequeue *q, int *x);

/*--- 全データを削除 ---*/
void Clear(IntDequeue *q);

/*---配列キューの全データを削除---*/
void Array_clear(IntDequeue *q);

/*--- キューの容量 ---*/
int Capacity(const IntDequeue *q);

/*--- キュー上のデータ数 ---*/
int Size(const IntDequeue *q);

/*--- キューは空か ---*/
int IsEmpty(const IntDequeue *q);

int IsArrayEmpty(const IntDequeue *q);

/*--- キューは満杯か ---*/
int IsFull(const IntDequeue *q);

/*--- キューからの探索 ---*/
int Search(const IntDequeue *q, int x);

/*--- キューからの探索（論理的添字を返却）---*/
int Search2(const IntDequeue *q, int x);

/*--- 全データの表示 ---*/
void Print(const IntDequeue *q);

/*---配列キューの表示---*/
void Array_print(const IntDequeue *q);

/*--- キューの後始末 ---*/
void Terminate(IntDequeue *q);

/*---キューの最小値を探索---*/
int SearchMin(IntDequeue *q);

/*---キューのコピー---*/
void Copy(IntDequeue *q1,const IntDequeue *q2);

/*---配列キューのコピー---*/
void Array_copy(IntDequeue *q1,const IntDequeue *q2);

/*---配列キューの後始末---*/
void Array_terminate(IntDequeue *q);

void Swap_IntDequeue(IntDequeue *q1, IntDequeue *q2);
#endif
