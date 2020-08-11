
#define Infinity (6000000000L)

#ifndef _BinHeap_H

#define MaxTrees (12)
#define Capacity (4095)

struct BinNode;
typedef struct BinNode *BinTree;
struct Collection;
typedef struct Collection *BinQueue;

BinQueue Initialize( void );
BinQueue Merge( BinQueue H1, BinQueue H2 );
int IsEmpty( BinQueue H );
#endif
