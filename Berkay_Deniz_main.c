#include "binomial.h"
#include "fatal.h"
#include <math.h>
typedef struct BinNode *Position;
typedef struct node {
    int name;
    int arr_time;
    int exe_time;
    int remaining;
    int wt;
    int is_in_heap;
    struct node *next;
}node;
struct BinNode {
    double Item;
    struct node*ptr;
    Position    LeftChild;
    Position    NextSibling;
};
struct Collection{
    int CurrentSize;
    BinTree TheTrees[ MaxTrees ];
};
BinQueue Initialize( void ) {
    BinQueue H;
    int i;
    H = malloc( sizeof( struct Collection ) );
    if( H == NULL )
        FatalError( "Out of space!!!" );
    H->CurrentSize = 0;
    for( i = 0; i < MaxTrees; i++ )
        H->TheTrees[ i ] = NULL;
    return H;
}
static void DestroyTree( BinTree T ) {
    if( T != NULL ) {
        DestroyTree( T->LeftChild );
        DestroyTree( T->NextSibling );
        free( T );
    }
}
BinQueue Insert( node *ptr,double Item,BinQueue H ) {
    BinTree NewNode;
    BinQueue OneItem;
    int i;
    NewNode = malloc( sizeof( struct BinNode ) );
    if( NewNode == NULL )
        FatalError( "Out of space!!!" );
    NewNode->LeftChild = NewNode->NextSibling = NULL;
    NewNode->Item = Item;
    NewNode->ptr=ptr;

    OneItem = Initialize( );
    OneItem->CurrentSize = 1;
    OneItem->TheTrees[ 0 ] = NewNode;

    return Merge( H, OneItem );
}
node *DeleteMin( BinQueue H ) {
    int i, j;
    int MinTree;   /* The tree with the minimum item */
    BinQueue DeletedQueue;
    Position DeletedTree, OldRoot;
    double MinItem;
    node *minNode;

    if( IsEmpty( H ) )
        Error( "Empty binomial queue" );

    MinItem = Infinity;
    for( i = 0; i < MaxTrees; i++ ) {
        if( H->TheTrees[ i ] && H->TheTrees[ i ]->Item <= MinItem ) {
            if(H->TheTrees[ i ]->Item == MinItem){
                if(H->TheTrees[i]->ptr->arr_time<minNode->arr_time){
                    minNode=H->TheTrees[i]->ptr;
                }
            }
            MinItem = H->TheTrees[i]->Item;
            minNode=H->TheTrees[i]->ptr;
            MinTree = i;
        }
    }

    DeletedTree = H->TheTrees[ MinTree ];
    OldRoot = DeletedTree;
    DeletedTree = DeletedTree->LeftChild;
    free( OldRoot );

    DeletedQueue = Initialize( );
    DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
    for( j = MinTree - 1; j >= 0; j-- )
    {
        DeletedQueue->TheTrees[ j ] = DeletedTree;
        DeletedTree = DeletedTree->NextSibling;
        DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
    }

    H->TheTrees[ MinTree ] = NULL;
    H->CurrentSize -= DeletedQueue->CurrentSize + 1;

    Merge( H, DeletedQueue );
    return minNode;
}
int IsEmpty( BinQueue H ) {
    return H->CurrentSize == 0;
}
BinTree CombineTrees( BinTree T1, BinTree T2 ) {
    if (T1->Item > T2->Item)
        return CombineTrees(T2, T1);
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
}
BinQueue Merge( BinQueue H1, BinQueue H2 ) {
    BinTree T1, T2, Carry = NULL;
    int i, j;

    if( H1->CurrentSize + H2->CurrentSize > Capacity )
        Error( "Merge would exceed capacity" );

    H1->CurrentSize += H2->CurrentSize;
    for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
    {
        T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

        switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
        {
            case 0: /* No trees */
            case 1: /* Only H1 */
                break;
            case 2: /* Only H2 */
                H1->TheTrees[ i ] = T2;
                H2->TheTrees[ i ] = NULL;
                break;
            case 4: /* Only Carry */
                H1->TheTrees[ i ] = Carry;
                Carry = NULL;
                break;
            case 3: /* H1 and H2 */
                Carry = CombineTrees( T1, T2 );
                H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                break;
            case 5: /* H1 and Carry */
                Carry = CombineTrees( T1, Carry );
                H1->TheTrees[ i ] = NULL;
                break;
            case 6: /* H2 and Carry */
                Carry = CombineTrees( T2, Carry );
                H2->TheTrees[ i ] = NULL;
                break;
            case 7: /* All three */
                H1->TheTrees[ i ] = Carry;
                Carry = CombineTrees( T1, T2 );
                H2->TheTrees[ i ] = NULL;
                break;
        }
    }
    return H1;
}
BinTree printTree(BinTree p, BinTree *r, int i) {
    BinTree t[20]={NULL}, q; int j;
    for ( j=0; j<i; j++ ) t[j]= r[j];
    i=0;
    if (p!=NULL) {
        printf("& %2.1lf ",p->Item);
        q=p->NextSibling;
        j=0;
        do {
            while (q!=NULL) {
                printf("%2.1lf ",q->Item);
                if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
                q=q->NextSibling;
            }
            q=t[j++];
        } while (q!=NULL);
    }
    else return NULL;
    //for (j=0; j<i; j++) t[j]=NULL;
    printf("\n");
    printTree(p->LeftChild, r, i);
}
node *add_process(node *root,int name,int exe_time,int arr_time){
    if(root==NULL){
        root=(node *)malloc(sizeof(node));
        root->exe_time=exe_time;
        root->next=NULL;
        root->name=name;
        root->wt=0;
        root->arr_time=arr_time;
        root->is_in_heap=0;
        root->remaining=exe_time;
        return root;
    }
    else{
        node *temp=root;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=(node *)malloc(sizeof(node));
        temp->next->next=NULL;
        temp->next->remaining=exe_time;
        temp->next->is_in_heap=0;
        temp->next->arr_time=arr_time;
        temp->next->wt=0;
        temp->next->name=name;
        temp->next->exe_time=exe_time;
        return root;
    }
}
double calculate_priority(node *ptr,int e_max){
    if(ptr->exe_time==ptr->remaining)
        return ptr->exe_time;
    else{
        return (1.0/exp(-pow(2.0*ptr->remaining/(3.0*e_max),3)))*ptr->remaining;
    }
}
node *control_arrival(node *root,int time){
    while(root!=NULL) {
        if(root->arr_time==time)
            return root;
        root=root->next;
    }
    node *p=NULL;
    return p;
}
void increase_wt(node *root1){
    while(root1!=NULL){
        if(root1->is_in_heap==1){
            root1->wt++;
        }
        root1=root1->next;
    }
}
int main() {
    BinQueue H1, H2;
    BinTree p, r[20] = {NULL};
    H1 = Initialize();
    int q;
    for (q = 1; q <= 10; q++) {
        node *root = NULL;
        FILE *fileptr = NULL;
        fileptr = fopen("input.txt", "r");
        char c;
        int name;
        int arr_time;
        int exe_time;
        while (fscanf(fileptr, "%c", &c) == 1 && c == 'P') {
            if (fscanf(fileptr, "%d", &name) == 0)
                break;
            if (fscanf(fileptr, "%d", &exe_time) == 0)
                break;
            if (fscanf(fileptr, "%d", &arr_time) == 0)
                break;
            if (fscanf(fileptr, "%c", &c) == 0)
                break;
            root = add_process(root, name, exe_time, arr_time);
        }
        int e_max=0;
        int total_exe=0;
        int number_of_processes=0;
        node *temp = root;
        while (temp != NULL) {
            if (temp->exe_time > e_max)
                e_max = temp->exe_time;
            total_exe+=temp->exe_time;
            number_of_processes++;
            temp = temp->next;
        }
        int time = 0;
        int control_time=0;
        node *processing_node=NULL;
        node *adding_node=NULL;
        while(time<total_exe){
            adding_node=control_arrival(root,time);
            if(adding_node!=NULL){
                Insert(adding_node,calculate_priority(adding_node,e_max)+adding_node->arr_time*0.0000000000001,H1);
                adding_node->is_in_heap=1;
            }
            if(processing_node==NULL){
                processing_node=DeleteMin(H1);
                processing_node->is_in_heap=0;
                processing_node->remaining--;
                if(processing_node->remaining==0)
                    processing_node=NULL;
                control_time=time+q;
                time++;
                increase_wt(root);
                continue;
            }
            if(time!=control_time){
                processing_node->remaining--;
                if(processing_node->remaining==0)
                    processing_node=NULL;
                time++;
                increase_wt(root);
                continue;
            }
            if(time==control_time){
                Insert(processing_node,calculate_priority(processing_node,e_max)+processing_node->arr_time*0.0000000000001,H1);
                processing_node->is_in_heap=1;
                processing_node=DeleteMin(H1);
                processing_node->is_in_heap=0;
                processing_node->remaining--;
                if(processing_node->remaining==0)
                    processing_node=NULL;
                control_time=time+q;
                time++;
                increase_wt(root);
                continue;
            }
        }
        int total_wt=0;
        printf("q = %d \n",q);
        printf("PID    Waiting Time\n");
        node *iter=root;
        while(iter!=NULL){
            printf("P%d      %d",iter->name,iter->wt);
            printf("\n");
            total_wt+=iter->wt;
            iter=iter->next;
        }
        printf("For q=%d AWT is %d / %d = %f",q,total_wt,number_of_processes,(total_wt*1.0/number_of_processes));
        printf("\n");
        printf("\n");
    }
}

