/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "lab2_sync_types.h"

#define LEFT 0
#define RIGHT 1

pthread_mutex_t mutex_Tree = PTHREAD_MUTEX_INITIALIZER; //for coarse grain
pthread_mutex_t tree_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t TreeEmpty = PTHREAD_COND_INITIALIZER;

// INSERT 가 생산자고, DELETE가 소비자야
// 생산 할 때는 Pre -> child 로 이어줘
// 소비 할 때는 Suc -> child를 떼어서 rm노드와 바꿔

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree)
{
    // You need to implement lab2_node_print_inorder function.
    lab2_node *node = tree->root;
    if (lab2_node_print(node) < 0)
        return LAB2_ERROR;
    return LAB2_SUCCESS;
}
int lab2_node_print(lab2_node *node)
{
    if (node == NULL)
        return LAB2_ERROR; //node is NULL
    if (node->left != NULL)
        lab2_node_print(node->left);
    //printf("%d ", node->key);
    if (node->right != NULL)
        lab2_node_print(node->right);
    return LAB2_SUCCESS;
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create()
{
    // You need to implement lab2_tree_create function.
    lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree));
    tree->root = NULL;
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node *lab2_node_create(int key)
{
    // You need to implement lab2_node_create function.
    lab2_node *baby = (lab2_node *)malloc(sizeof(lab2_node));
    pthread_mutex_init(&(baby->mutex), NULL);
    baby->key = key;
    baby->left = NULL;
    baby->right = NULL;
    return baby;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert function.
    struct lab2_node *leaf = tree->root;
    struct lab2_node *pre = leaf;
    if (tree->root == NULL)
    {
        tree->root = new_node;
        return LAB2_SUCCESS;
    }
    while (leaf != NULL)
    {
        pre = leaf;
        if (leaf->key <= new_node->key)
            leaf = leaf->right;
        else
            leaf = leaf->left;
    }
    if (pre->key <= new_node->key)
        pre->right = new_node;
    else
        pre->left = new_node;

    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert_fg function.
    //hand-over-hand lock mechanism
    struct lab2_node *leaf = tree->root;
    struct lab2_node *pleaf = tree->root;
    struct lab2_node *pre = NULL;
    int cond = 1; // volatile?
    int pKey;

    //#### 0. Initialize
    while(tree->root==NULL){
        pthread_mutex_lock(&mutex_Tree);
        while(tree->root == NULL){
            tree->root = new_node;
            cond = 0;
        }        
        pthread_mutex_unlock(&mutex_Tree);
    }
    if(cond)
    {    
        //#### I. Traverse
        //hand-over-hand lock
        //현재 Tree is not empty
        //Need 3 variable
        pre = tree->root;
        pthread_mutex_lock(&(pre->mutex));
        leaf = (pre->key <= new_node->key) ? pre->right : pre->left;

        while (leaf != NULL) // Need to search More
        {
            pthread_mutex_lock(&(leaf->mutex));
            pleaf = leaf;
            leaf = (leaf->key <= new_node->key)? leaf->right : leaf->left;
            //there is no such danger like "leaf has deleted" BECAUSE Delete Needs 4 Locks(SAFE)
            pthread_mutex_unlock(&(pre->mutex));
            pre = pleaf;
        }

        // After All traverse done, thread has lock on node 'pre'
        //####  END I. Traverse & Lock/unLock

        //#### II. Execution
        pKey = pre->key;
        if (pre != NULL && (pKey == pre->key))
        {
            //condition : 1. insert 2. simple deletion 3.complex deletion
            //all cond true mean Insetion/deletion is executing in other subtree.
            if (pre->key > new_node->key)
                pre->left = new_node;
            else
                pre->right = new_node;
        }
        pthread_mutex_unlock(&(pre->mutex)); //## UNLOCK ##
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert_cg function.
    /* Acquiring Lock */

    struct lab2_node *leaf = NULL;
    struct lab2_node *pre = NULL;

    int cond = 1; // volatile?
    int pKey;
    pthread_mutex_lock(&mutex_Tree);
    /* critical section START*/

    if (tree->root != NULL)
    {
        leaf = tree->root;
        pre = tree->root; //tree->root?
    }
    else
    {
        if(tree->root != NULL)
        {
            leaf = tree->root;
            pre = tree->root;
        }else{
            tree->root = new_node;
            pthread_mutex_unlock(&mutex_Tree);        
            return LAB2_SUCCESS;
        }
    }
    while (leaf != NULL)
    {
        pre = leaf;
        if (leaf->key <= new_node->key)
            leaf = leaf->right;
        else
            leaf = leaf->left;
    }

    if (pre->key <= new_node->key)
        pre->right = new_node;
    else
        pre->left = new_node;

    /* critical section END*/
    /* Release lock  */
    pthread_mutex_unlock(&mutex_Tree);

    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove function.
    /*
    //0 . Init
    //I . Traverse
    //II. Execute from "remove"
    */

    lab2_node *premove = NULL;
    lab2_node *remove = tree->root;
    lab2_node *pleaf = tree->root;
    lab2_node *leaf = tree->root;
    lab2_node *psuccessor;
    lab2_node *successor;
    lab2_node *child;
    int state = 0; //FALSE

    while (leaf != NULL)
    {
        premove = remove;
        remove = leaf;
        if (remove->key == key)
            break; //i find a key 
        else if (remove->key < key)
            leaf = leaf->right;
        else
            leaf = leaf->left;
    }
    /*#2 execution
        1. simple deletion
        2. complex deletion
    */
    if (remove != NULL) //i find value
    {
        //state = true
        //else if remove has a two child
        if (remove->left != NULL && remove->right != NULL)
        {
            //1. find successor
            successor = remove->right;
            psuccessor = remove;
            while (successor->left != NULL)
            {
                psuccessor = successor;
                successor = successor->left;
            }
            //2. change value with successor
            remove->key = successor->key;
            //delete successor
            if(remove != psuccessor) psuccessor->left = successor->right;//?
            else remove->right = successor->right;
            free(successor);successor = NULL;
            state = 1; //TRUE
        }
        //if remove has a child or no child
        else if (remove->left == NULL || remove->right == NULL)
        {
            child = (remove->right == NULL) ? remove->left : remove->right;
            if(child ==NULL) //Case No Child
            {
                if(remove == tree->root){
                    //tree Deletion
                    tree->root = NULL;
                    free(remove); remove = NULL;
                } 
                else{// case General
                    if(premove->left == remove)
                        premove->left =NULL;
                    else premove->right = NULL;
                    free(remove); remove = NULL;
                }
            }
            else // Case one Child
            {
                if(remove == tree->root){
                    //Tree->root = child;
                    tree->root = child;
                    free(remove); remove = NULL;
                } 
                else{// case General
                    if(premove->left == remove)
                        premove->left = child;
                    else premove->right = child; 
                    free(remove); remove = NULL;
                }
            }
            state = 1; //TRUE
        }
    }
    else //Cant find value
        state = 0;                              //"No such Key"
    return (state) ? LAB2_SUCCESS : LAB2_ERROR; //error : No such Key
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
	if (tree->root == NULL)
		return LAB2_SUCCESS;

	lab2_node *root = tree->root;               // remove에 필요한 변수 선언
	lab2_node *p = tree->root;
	lab2_node *q = NULL;
	lab2_node *r = NULL;
	lab2_node *leftChild = NULL;
	lab2_node *rightChild = NULL;
	int status = 0;
	int errNum = 0;
	int childNum = 0;

	pthread_mutex_init(&p->mutex, NULL);         // lock 초기화
	pthread_mutex_lock(&p->mutex);               // 노드 p lock
	while (p) {
		if (p->key < key) {                     // 노드 p의 key 값이 삭제할 key 값보다 작으면
			q = p;                           // 노드 p를 right child 로 이동 시키기 위해서 p의 right child 부모를 q에 삽입
			pthread_mutex_unlock(&p->mutex);      // 노드 p unlock
			p = p->right;                     // 노드 p를 right child로 내려줌
		}
		else if (p->key > key) {               // 노드 p의 key 값이 삭제할 key 값보다 크면
			q = p;                           // 노드 p를 left child로 이동 시키기 위해서 p의 left child 부모를 q에 삽입
			pthread_mutex_unlock(&p->mutex);      // 노드 p unlock
			p = p->left;                     // 노드 p를 left child로 내려줌
		}
		else {                              // 삭제할 key값을 찾으면
			errNum = 1;                        // error number를 1 증가
			if (p->right != NULL) {               // 노드 p의 right child의 값이 있으면
				childNum++;                     // 노드 p의 right child가 존재 하기 때문에 child number를 1증가
				rightChild = p->right;            // rightChild에 노드 p의 right child 값 삽입
				pthread_mutex_init(&rightChild->mutex, NULL);   // 삽입한 rightChild lock 초기화
			}
			if (p->left != NULL) {               // 노드 p의 left child의 값이 있으면
				childNum++;                     // 노드 p의 left child가 존재 하기 때문에 child number를 1증가
				leftChild = p->left;            // leftChild에 노드 p의 left child 값 삽입
				pthread_mutex_init(&leftChild->mutex, NULL);   // 삽입한 leftChild lock 초기화
			}
			break;
		}
		pthread_mutex_unlock(&p->mutex);         // 마지막으로 노드 p unlock
	}

	if (errNum == 0)                        // error number가 1로 증가 되지 않았으면, 삭제할 key 값이 tree안에 존재하지 않으므로
		return LAB2_ERROR;                     // error를 return 한다

	if (q != NULL)                           // 노드 p의 부모를 q에 삽입 했는데, 삽입한 q값이 존재하면
		pthread_mutex_init(&q->mutex, NULL);      // 노드 q lock 초기화

	if (childNum == 0) {                     // 노드 p의 child 개수가 0이면
		if (q == NULL) {                     // p의 부모가 없으면
			lab2_node_delete(p);               // p의 값 삭제
			goto success;
		}

		pthread_mutex_lock(&q->mutex);            // 노드 p의 부모 q lock
		pthread_mutex_lock(&p->mutex);            // 노드 p lock
		if (q->key > key) {                     // 노드 p의 부모 q의 key 값이 삭제할 key값 보다 크면
			q->left = NULL;                     // 노드 q의 left child 초기화 
			pthread_mutex_unlock(&p->mutex);      // 노드 p unlock
			lab2_node_delete(p);               // 노드 p 삭제
		}
		else {                              // 노드 p의 부모 q의 key 값이 삭제할 key값 보다 작으면
			q->right = NULL;                  // 노드 q의 right child 초기화
			pthread_mutex_unlock(&p->mutex);      // 노드 p unlock
			lab2_node_delete(p);               // 노드 p 삭제
		}
		pthread_mutex_unlock(&q->mutex);         // 마지막으로 노드 p의 부모 q unlock
		goto success;
	}

	else if (childNum == 1) {                  // 노드 p의 child 개수가 1이면
		pthread_mutex_lock(&p->mutex);            // 노드 p lock
		if (q == NULL) {                     // 노드 p의 부모 q가 NULL이면
			q = p;                           // 노드 q에 노드 p값 삽입
			pthread_mutex_lock(&root->mutex);      // root lock

			if (p->left != NULL) {               // 노드 p의 left child값이 있으면
				pthread_mutex_lock(&leftChild->mutex);   // 노드 p의 left child lock
				tree->root = p->left;            // root값에 노드 p의 left child 값 삽입
				pthread_mutex_unlock(&leftChild->mutex);   // left child unlock
				pthread_mutex_unlock(&root->mutex);   // root unlock
				lab2_node_delete(q);            // 노드 q 삭제
				pthread_mutex_unlock(&p->mutex);   // 노드 p unlock
			}
			else {                           // 노드 p의 right child값이 있으면
				pthread_mutex_lock(&rightChild->mutex);   // 노드 p의 right child lock
				tree->root = p->right;            // root값에 노드 p의 right child 값 삽입
				pthread_mutex_unlock(&rightChild->mutex);   // right child unlock
				pthread_mutex_unlock(&root->mutex);   // root unlock
				lab2_node_delete(q);            // 노드 q 삭제
				pthread_mutex_unlock(&p->mutex);   // 노드 p unlock
			}
			goto success;
		}

		pthread_mutex_lock(&q->mutex);            // 노드 q lock
		if (q->key > key) {                     // 노드 q의 key값이 삭제할 key값 보다 크면
			if (p->left != NULL) {               // 노드 p의 left child값이 존재하면
				pthread_mutex_lock(&leftChild->mutex);   // left child lock
				q->left = p->left;               // 노드 q의 left child에 노드 p의 left child값 삽입
				pthread_mutex_unlock(&q->mutex);   // 노드 q unlock
				lab2_node_delete(p);            // 노드 p 삭제
				pthread_mutex_unlock(&leftChild->mutex);   // left child unlock
				pthread_mutex_unlock(&p->mutex);   // 노드 p unlock
			}
			else {                           // 노드 p의 right child값이 존재하면
				pthread_mutex_lock(&rightChild->mutex);   // right child lock
				q->left = p->right;               // 노드 q의 left child에 노드 p의 right child값 삽입
				pthread_mutex_unlock(&q->mutex);   // 노드 q unlock
				lab2_node_delete(p);            // 노드 p 삭제
				pthread_mutex_unlock(&rightChild->mutex);   // right child unlock
				pthread_mutex_unlock(&p->mutex);   // 노드 p unlock
			}
		}
		else {                              // 아래 위와 반대
			if (p->left != NULL) {
				pthread_mutex_lock(&leftChild->mutex);
				q->right = p->left;
				pthread_mutex_unlock(&q->mutex);
				lab2_node_delete(p);
				pthread_mutex_unlock(&leftChild->mutex);
				pthread_mutex_unlock(&p->mutex);
			}
			else {
				pthread_mutex_lock(&rightChild->mutex);
				q->right = p->right;
				pthread_mutex_unlock(&q->mutex);
				lab2_node_delete(p);
				pthread_mutex_unlock(&rightChild->mutex);
				pthread_mutex_unlock(&p->mutex);
			}
		}
		goto success;
	}

	else {                                 // 노드 p의 child 개수가 2면
		r = p;
		p = p->left;

		pthread_mutex_init(&p->mutex, NULL);
		pthread_mutex_init(&r->mutex, NULL);
		while (1) {
			pthread_mutex_lock(&p->mutex);         // 노드 p lock
			if (q != NULL) {                  // 노드 q의 값이 NULL이 아니면
				pthread_mutex_lock(&r->mutex);      // 노드 r lock
				r = p;
				pthread_mutex_unlock(&r->mutex);   // 노드 r unlock
				p = p->right;                  // 노드 p를 right child로 내려줌

				if (p->left != NULL) {            // 노드 p의 left child값이 존재하면
					pthread_mutex_lock(&leftChild->mutex);   // leftChild lock
					leftChild = p->left;               // leftChild에 노드 p의 left child값 삽입
					pthread_mutex_unlock(&leftChild->mutex);   // leftChild unlock
				}
				if (p->right != NULL) {            // 노드 p의 right child값이 존재하면
					pthread_mutex_lock(&rightChild->mutex);   // rightChild lock
					rightChild = p->right;               // rightChild에 노드 p의 right child값 삽입
					pthread_mutex_unlock(&rightChild->mutex);   // rightChild unlock
				}
			}
			else {                           // 노드 q의 값이 NULL이면
				pthread_mutex_lock(&root->mutex);   // root lock
				if (r->key > p->key) {            // 노드 r의 key값이 노드 p의 key값 보다 크면
					pthread_mutex_lock(&r->mutex);   // 노드 r lock
					if (q == NULL) {            // 노드 q가 NULL이면
						p->right = r->right;      // 노드 p의 right child에 노드 r의 right child값 삽입
						tree->root = p;            // root값에 노드 p 삽입
						pthread_mutex_unlock(&r->mutex);   // 노드 r unlock
						pthread_mutex_unlock(&root->mutex);   // root unlock
						lab2_node_delete(r);      // 노드 r 삭제
					}
					else {
						p->right = r->right;      // 노드 p의 right child에 노드 r의 right child값 삽입
						pthread_mutex_unlock(&r->mutex);   // 노드 r unlock
						if (q->key > p->key) {      // 노드 q의 key값이 노드 p의 key값 보다 크면
							pthread_mutex_lock(&leftChild->mutex);   // leftChild lock
							q->left = p;         // 노드 q의 left child에 노드 p값 삽입
							pthread_mutex_unlock(&leftChild->mutex);   // lock 해제
							pthread_mutex_unlock(&r->mutex);
							pthread_mutex_unlock(&root->mutex);
						}
						else {
							pthread_mutex_lock(&rightChild->mutex);   // 위와 반대
							q->right = p;
							pthread_mutex_unlock(&rightChild->mutex);
							pthread_mutex_unlock(&r->mutex);
							pthread_mutex_unlock(&root->mutex);
						}
						lab2_node_delete(r);
					}
					goto success;
				}
				else {                        // 노드 r의 key값이 노드 p의 key값 보다 작으면
					if (p->left != NULL) {         // 노드 p의 left child값이 존재하면
						pthread_mutex_lock(&r->mutex);   // 노드 r lock
						r->right = p->left;         // 노드 r의 right child에 노드 p의 left child값 대입
						pthread_mutex_unlock(&r->mutex);   // lock 해제
						pthread_mutex_unlock(&root->mutex);
					}
					else {
						pthread_mutex_lock(&r->mutex);   // 노드 r lock
						r->right = NULL;            // 노드 r right child 초기화
						pthread_mutex_unlock(&r->mutex);   // lock 해제
						pthread_mutex_unlock(&root->mutex);
					}
				}

				if (q == NULL) {                  // 노드 q 가 NULL 이면
					pthread_mutex_lock(&q->mutex);      // 위의 부모부터 lock
					pthread_mutex_lock(&r->mutex);
					r = tree->root;                  // 노드 r에 root값 삽입
					pthread_mutex_lock(&rightChild->mutex);   // rightChild lock
					p->right = tree->root->right;      // 노드 p의 right child값에 root의 right child값 삽입
					pthread_mutex_unlock(&rightChild->mutex);   // rightChild unlock
					pthread_mutex_lock(&leftChild->mutex);   // leftChild lock
					p->left = tree->root->left;         // 노드 p의 left child값에 root의 left child값 삽입
					pthread_mutex_lock(&rightChild->mutex);   // leftChild unlock
					pthread_mutex_unlock(&r->mutex);   // lock해제
					pthread_mutex_unlock(&q->mutex);
					tree->root = p;                  // root에 p값 삽입
					pthread_mutex_unlock(&root->mutex);
					lab2_node_delete(r);            // 노드 r 삭제
					goto success;
				}

				if (q->key > p->key) {               // 노드 q의 key값이 노드 p의 key값 보다 크면
					pthread_mutex_lock(&q->mutex);      // 위에서부터 lock
					pthread_mutex_lock(&leftChild->mutex);
					p->left = q->left->left;         // 노드 p의 left child에 노드 q의 left child의 left child값 삽입
					pthread_mutex_lock(&rightChild->mutex);   // rightChild lock
					p->right = q->left->right;         // 노드 p의 right child에 노드 q의 left child의 right child값 삽입
					pthread_mutex_unlock(&rightChild->mutex);   // rightChild lock
					lab2_node_delete(q->left);         // 노드 q의 left child 삭제
					q->left = p;                  // 노드 q의 left child에 노드 p 삽입
					pthread_mutex_unlock(&leftChild->mutex);   // 락 해제
					pthread_mutex_unlock(&q->mutex);
					pthread_mutex_unlock(&root->mutex);
				}
				else {                           // 위와 반대 노드 q의 right child 부분
					pthread_mutex_lock(&q->mutex);
					pthread_mutex_lock(&rightChild->mutex);
					pthread_mutex_lock(&leftChild->mutex);
					p->left = q->right->left;
					pthread_mutex_unlock(&leftChild->mutex);
					p->right = q->right->right;
					lab2_node_delete(q->right);
					q->right = p;
					pthread_mutex_unlock(&rightChild->mutex);
					pthread_mutex_unlock(&q->mutex);
					pthread_mutex_unlock(&root->mutex);
				}
				break;
			}
			pthread_mutex_unlock(&p->mutex);
		}
	}
success:
	return LAB2_SUCCESS;
}

int lab2_node_remove_cg(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove_cg function.
    //#1 traverse
    
    lab2_node *premove = NULL;
    lab2_node *remove = tree->root;
    lab2_node *pleaf = tree->root;
    lab2_node *leaf = tree->root;
    lab2_node *psuccessor =NULL;
    lab2_node *successor =NULL;
    lab2_node *child =NULL;
    int state = 0; //FALSE

    pthread_mutex_lock(&(mutex_Tree));
    while (leaf != NULL)
    {
        premove = remove;
        remove = leaf;
        if (remove->key == key)
            break; //i find a key 
        else if (remove->key < key)
            leaf = leaf->right;
        else
            leaf = leaf->left;
    }
    /*#2 execution
        1. simple deletion
        2. complex deletion
    */
    if (remove != NULL) //i find value
    {
        //state = true
        //else if remove has a two child
        if (remove->left != NULL && remove->right != NULL)
        {
            //1. find successor
            successor = remove->right;
            psuccessor = remove;
            while (successor->left != NULL)
            {
                psuccessor = successor;
                successor = successor->left;
            }
            //2. change value with successor
            remove->key = successor->key;
            //delete successor
            if(remove != psuccessor) psuccessor->left = successor->right;//?
            else remove->right = successor->right;
            free(successor);successor = NULL;
            state = 1; //TRUE
        }
        //if remove has a child or no child
        else if (remove->left == NULL || remove->right == NULL)
        {
            child = (remove->right == NULL) ? remove->left : remove->right;
            if(child ==NULL) //Case No Child
            {
                if(remove == tree->root){
                    //tree Deletion
                    tree->root = NULL;
                    free(remove); remove = NULL;
                } 
                else{// case General
                    if(premove->left == remove)
                        premove->left =NULL;
                    else premove->right = NULL;
                    free(remove); remove = NULL;
                }
            }
            else // Case one Child
            {
                if(remove == tree->root){
                    //Tree->root = child;
                    tree->root = child;
                    free(remove); remove = NULL;
                } 
                else{// case General
                    if(premove->left == remove)
                        premove->left = child;
                    else premove->right = child; 
                    free(remove); remove = NULL;
                }
            }
            state = 1; //TRUE
        }
    }
    else //Cant find value
        state = 0;                              //"No such Key"
    pthread_mutex_unlock(&(mutex_Tree));
    return (state) ? LAB2_SUCCESS : LAB2_ERROR; //error : No such Key
}

/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree)
{
    // You need to implement lab2_tree_delete function.
    if (tree->root != NULL)
        lab2_node_delete(tree->root);
    tree->root = NULL;
    free(tree);
    tree = NULL;
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node)
{
    // You need to implement lab2_node_delete function.
    if (node->left != NULL)
        lab2_node_delete(node->left);
    if (node->right != NULL)
        lab2_node_delete(node->right);

    node->left = NULL;
    node->right = NULL;
    free(node);
    node = NULL;
}
