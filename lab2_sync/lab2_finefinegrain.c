int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert_fg function.
    /*
    root = tree->root;
    pre =root;
    cond =1
    #### 1 Reading #####
    Search한다. root부터. root부터 순회 하며 LOCK 을 다 잡는다
    IF FOUND NODE? 
        then UNLOCK from root, to this parent

        //still locked in this node
            this ->left or right = new_node 
            //WRITE
            UNLOCK this node    
            return true;
    
    */
    // You need to implement lab2_node_insert_cg function.

    //hand-over-hand lock mechanism
    struct lab2_node *leaf = tree->root;
    struct lab2_node *pleaf = tree->root;
    struct lab2_node *pre = NULL;
    int cond = 1; // volatile?
    int pKey;

    //#### 0. Initialize
    if (tree->root != NULL)
    {
        leaf = tree->root;
        pre = leaf;
    }
    else
    {
        pthread_mutex_lock(&(mutex_Tree));
        if (tree->root == NULL){
            tree->root = new_node;
            pthread_mutex_unlock(&(mutex_Tree));
            return LAB2_SUCCESS;            
        }
        else//then tree root changed already, goto loop 
        {
            leaf = tree->root;
            pre = leaf;
            /* code */
            pthread_mutex_unlock(&(mutex_Tree));
        }

    }
    //#### I. Traverse
    pthread_mutex_lock(&pre->mutex);
    leaf = (pre->key < new_node->key) ? pre->right : pre->left;
    while (leaf != NULL)
    {
        if (pthread_mutex_lock(&leaf->mutex) != 0)
            break; //if leaf deleted, lock returns fail
        pre = leaf;

        if (leaf->key < new_node->key)
            leaf = leaf->left;
        else
            leaf = leaf->right;
    }
    // After All traverse done, tree has lock from root to 'pre'
    leaf = tree->root;
    while (leaf != pre)
    {
        pleaf = leaf;
        leaf = (leaf->key < new_node->key) ? leaf->right : leaf->left; //leaf null?
        pthread_mutex_unlock(&pleaf->mutex);
    }
    //####  END I. Traverse & Lock/unLock
    //"Now , It has locked only "pre" Node."
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
    return LAB2_SUCCESS;
}
