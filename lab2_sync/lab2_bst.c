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

#include "lab2_sync_types.h"

#define LEFT 0
#define RIGHT 1

pthread_mutex_t mutex_Tree = PTHREAD_MUTEX_INITIALIZER; //for coarse grain
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
        pthread_mutex_lock(&pre->mutex);
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
int lab2_node_remove_fg(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove_fg function.
    //1 lock/unlock in find successor
    //2 lock/unlock in change its value
    // You need to implement lab2_node_remove_cg function.
    //#### 0. Initialize

    lab2_node *leaf = tree->root;
    lab2_node *pleaf =NULL;
    lab2_node *remove =tree->root;
    lab2_node *premove = NULL;
    lab2_node *successor;
    lab2_node *psuccessor;
    int cond =0;
    int state = 0;
    int i =0;
    int laststep = LEFT;
    int child[2] = {0, 0};
    //int childchilde[2] = {0,};
    //define LEFT 0 RIGHT 1
    //## 0. Initialize
    printf(" %d ",i++);
    pthread_mutex_lock(&(mutex_Tree));
        printf(" %d ",i++);

    if(tree->root == NULL){
        //No deletion
        pthread_mutex_unlock(&mutex_Tree);
        return LAB2_ERROR; // Tree is empty        
    }
    pthread_mutex_lock(&(remove->mutex));
    if(remove->key == key){
        leaf = NULL;
        cond =1;
    }
    else leaf = (remove->key < key) ? remove->right: remove->left;
        printf(" %d ",i++);

    while (leaf != NULL)
    {
        pthread_mutex_lock(&leaf->mutex);
        premove = remove;
        remove = leaf;
        if(premove == tree->root)
            pthread_mutex_unlock(&(mutex_Tree));
        if (remove->key == key){
            cond = 1;
            break;
        }
        else if (remove->key < key)
            leaf = leaf->right;
        else
            leaf = leaf->left;        
        
        if(premove != NULL) 
            pthread_mutex_unlock(&(premove->mutex));
        
    }
    //####  END I. Traverse & Lock/unLock
    //"And now, remove node locked"

    //#### II. Execution
    if(cond){
            printf(" %d ",i++);

        if (remove->left != NULL){
            pthread_mutex_lock(&remove->left->mutex);
            child[LEFT] = 1; }
        if (remove->right != NULL){
            pthread_mutex_lock(&remove->right->mutex);
            child[RIGHT] = 1; }
            
        if(premove !=NULL){ //lock : premove, remove
            laststep = (premove->left == remove )? LEFT : RIGHT;
            if(child[LEFT] & child[RIGHT]){ //TWO CHILD
                //lock : premove , remove , left ,  rightchild
                //go right and find successor       
                psuccessor = remove->right;            //successor 부모
                successor = remove->right->left;       //successor 위치
                pthread_mutex_lock(&successor->mutex); // ...how unlock it?
                while (successor->left != NULL)
                {
                    pthread_mutex_lock(&(successor->mutex));
                    psuccessor = successor;
                    successor = successor->left;
                }
                pthread_mutex_lock(&(successor->mutex));
                //successor = remove's successor
                remove->key = successor->key;
                psuccessor->left = successor->right;
                pthread_mutex_unlock(&successor->mutex);
                free(successor);
                successor = NULL;
                psuccessor = remove->right->left;
                while (psuccessor != NULL)
                {
                    pthread_mutex_unlock(&(psuccessor->mutex));
                    psuccessor = psuccessor->left;
                }
                // pthread_mutex_unlock(&(remove->mutex));
                //unlock : left right         
            }else if(child[LEFT]){ //ONE LEFT CHILD
                //premove->laststep = remove->left
                //lock : premove(or tree mutex) remove, LEFT           
                //case General
                if(laststep == LEFT) premove->left = remove->left;
                else if(laststep == RIGHT) premove->right = remove->left;
                //unlock left

            }else {//ONE RIGHT CHILD or No child
                //premove->laststep= remove->right;
                //lock : premove remove remove->right
                if(laststep == LEFT) premove->left = remove->right;
                else if(laststep == RIGHT) premove->right = remove->right;
                //unlock right
            }
        }
        else{ 
            //lock : remove , tree mutex (root deletion) 
            if(child[LEFT] & child[RIGHT]){ //TWO CHILD
                //lock : premove , remove , left ,  rightchild
                //go right and find successor       
                psuccessor = remove->right;            //successor 부모
                successor = remove->right->left;       //successor 위치
                pthread_mutex_lock(&successor->mutex); // ...how unlock it?
                while (successor->left != NULL)
                {
                    pthread_mutex_lock(&(successor->mutex));
                    psuccessor = successor;
                    successor = successor->left;
                }
                pthread_mutex_lock(&(successor->mutex));
                //successor = remove's successor
                remove->key = successor->key;
                psuccessor->left = successor->right;
                pthread_mutex_unlock(&successor->mutex);
                free(successor);
                successor = NULL;
                psuccessor = remove->right->left;
                while (psuccessor != NULL)
                {
                    pthread_mutex_unlock(&(psuccessor->mutex));
                    psuccessor = psuccessor->left;
                }
                // pthread_mutex_unlock(&(remove->mutex));
                //unlock : left right         
            }else if(child[LEFT]){ //ONE LEFT CHILD
                //premove->laststep = remove->left
                //lock : premove(or tree mutex) remove, LEFT           
                //case General
                tree->root = remove->left;
                //unlock left

            }else {//ONE RIGHT CHILD or No child
                //premove->laststep= remove->right;
                //lock : premove remove remove->right
                tree->root = remove->right; //could be tree->root=NULL;
                //unlock right
            }
        }
        if(child[LEFT])
            pthread_mutex_unlock(&(remove->left->mutex));
        if(child[RIGHT])
            pthread_mutex_unlock(&(remove->right->mutex));

        if( !(child[LEFT] & child[RIGHT]) ){
            pthread_mutex_unlock(&(remove->mutex));
            free(remove); remove = NULL;
        }
        state = 1;
        if(premove !=NULL)
            pthread_mutex_unlock(&(premove->mutex));
        else
            pthread_mutex_unlock(&(mutex_Tree));
        //unlock
    }else{
        //lock remove
        state = 0;
        pthread_mutex_unlock(&(remove->mutex));
    }
    // //"4월29일,
    // //remove fg 는 4개의노드 premove, remove, remove->l remove->r 을 Lock 해주어야 한다
    // // if(premove != NULL)
    // //     pthread_mutex_lock(&premove->mutex);//dead
    // if (remove->left != NULL)
    // {
    //     pthread_mutex_lock(&remove->left->mutex);
    //     child[LEFT] = 1;
    // }
    // if (remove->right != NULL)
    // {
    //     pthread_mutex_lock(&remove->right->mutex);
    //     child[RIGHT] = 1;
    // }
    // //if(remove->key == key)    {
    // //Complex Deletion
    // if (child[LEFT] & child[RIGHT])
    // {                                          //it has both child
    //     psuccessor = remove->right;            //successor 부모
    //     successor = remove->right->left;       //successor 위치
    //     pthread_mutex_lock(&successor->mutex); // ...how unlock it?
    //     while (successor->left != NULL)
    //     {
    //         pthread_mutex_lock(&(successor->mutex));
    //         psuccessor = successor;
    //         successor = successor->left;
    //     }
    //     pthread_mutex_lock(&(successor->mutex));
    //     //successor = remove's successor
    //     remove->key = successor->key;
    //     psuccessor->left = successor->right;
    //     pthread_mutex_unlock(&successor->mutex);
    //     free(successor);
    //     successor = NULL;
    //     psuccessor = remove->right->left;
    //     while (psuccessor != NULL)
    //     {
    //         pthread_mutex_unlock(&(psuccessor->mutex));
    //         psuccessor = psuccessor->left;
    //     }
    //     pthread_mutex_unlock(&(remove->left->mutex));
    //     pthread_mutex_unlock(&(remove->right->mutex));
    //     pthread_mutex_unlock(&(remove->mutex));
    // }
    // //Simple Deletion
    // else if (child[LEFT])
    // {
    //     if (premove->left == remove)
    //         premove->left = remove->left;
    //     else
    //         premove->right = remove->left;
    //     free(remove);
    //     remove = NULL;
    //     pthread_mutex_unlock(&(premove->left->mutex));
    // }
    // else
    // { //child[RIGHT] & No child;
    //     if (premove->left == remove)
    //         premove->left = remove->right;
    //     else
    //         premove->right = remove->right;
    //     free(remove);
    //     remove = NULL;
    //     pthread_mutex_unlock(&(premove->right->mutex));
    // }
    // if (premove != NULL)
    //     pthread_mutex_unlock(&(premove->mutex));
    // else
    //     return LAB2_ERROR;
    return (state != 0 )? LAB2_SUCCESS:LAB2_ERROR; //No such Key
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
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
