#ifndef ___lower_bound
#define ___lower_bound
#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "data_info.h"

/*下側に位置するブロッキングブロック数*/
int nblocking_lower(IntDequeue *q);

/*上側に位置するブロッキングブロック数*/
int nblocking_upper(IntDequeue *q);

int nblocking(IntDequeue *q, direction dir);

//*---一つのスタックにおいて、すべてのブロックを取り出すまでの最小積み替え回数---*//
int min_relocation(IntDequeue *q,direction dir);

int pre_min_relocation(IntDequeue *q,direction dir);

int min_relocation_upper(IntDequeue *q);

int min_relocation_lower(IntDequeue *q);

//*---LB1を計算---*//
int lower_bound1(IntDequeue *q);

//*---スタックにおけるpriorityの比較関数---*//
int pricmp(const IntDequeue *n1, const IntDequeue *n2);

//*---引数の２の累乗を数える--*//
int num_expo(int number);

#endif
