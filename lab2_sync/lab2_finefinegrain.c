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
    int protect =0;
    int child[2] = {0, 0};
    //int childchilde[2] = {0,};
    //define LEFT 0 RIGHT 1
    //## 0. Initialize
    pthread_mutex_lock(&tree_mutex);

    if(tree->root == NULL){
        //No deletion
        pthread_mutex_unlock(&tree_mutex);
        return LAB2_ERROR; // Tree is empty        
    }
    if(pthread_mutex_lock(&(remove->mutex)) > 0)
        write(STDERR_FILENO, "why this happend?\n" , 64);
    if(remove->key == key){
        leaf = NULL;
        cond =1;
    }
    else leaf = (remove->key < key) ? remove->right: remove->left;

    while (leaf != NULL)
    {
        pthread_mutex_lock(&(leaf->mutex));
        premove = remove;
        remove = leaf;
        if(premove == tree->root)
            pthread_mutex_unlock(&tree_mutex);
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

        if (remove->left != NULL){
            pthread_mutex_lock(&(remove->left->mutex));
            child[LEFT] = 1; }
        if (remove->right != NULL){
            pthread_mutex_lock(&(remove->right->mutex));
            child[RIGHT] = 1; }
            
        if(premove !=NULL){ //lock : premove, remove
            laststep = (premove->left == remove )? LEFT : RIGHT;
            if(child[LEFT] & child[RIGHT]){ //TWO CHILD
                //lock : premove , remove , left ,  rightchild
                //go right and find successor  3
                
                psuccessor = remove->right;
                successor = remove->right;
                leaf = successor->left;

                while(leaf != NULL){
                    pthread_mutex_lock(&(leaf->mutex));
                    psuccessor = successor;
                    successor = leaf;
                    leaf = leaf->left;
                }

                if(successor == remove->right){
                    //then 
                    remove->right = successor->right;
                    pthread_mutex_unlock(&(successor->mutex));
                    free(successor); successor = NULL;
                    child[RIGHT] = 0;
                    protect =1;

                }
                else if(psuccessor == remove->right){
                    psuccessor ->left = successor->right;
                    pthread_mutex_unlock(&(successor->mutex));
                    free(successor); successor =NULL;
                }
                else{
                    psuccessor->left = successor->right;
                    pthread_mutex_unlock(&(successor->mutex));
                    free(successor); successor = NULL;
                    leaf = remove->right->left;
                    do{
                        pthread_mutex_unlock(&(leaf->mutex));
                        leaf = leaf->left;
                    }while(leaf != psuccessor);
                }
                // pthread_mutex_unlock(&(remove->mutex));
                //unlock : left right         
            }else if(child[LEFT]){ //ONE LEFT CHILD
                //premove->laststep = remove->left
                //lock : premove(or tree mutex) remove, LEFT           
                //case General
                if(laststep == LEFT){ 
                    premove->left = remove->left;
                }
                else if(laststep == RIGHT) {
                    premove->right = remove->left;
                }

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
                psuccessor = remove->right;
                successor = remove->right;
                leaf = successor->left;

                while(leaf != NULL){
                    pthread_mutex_lock(&(leaf->mutex));
                    psuccessor = successor;
                    successor = leaf;
                    leaf = leaf->left;
                }

                if(successor == remove->right){
                    //then 
                    remove->right = successor->right;
                    pthread_mutex_unlock(&(successor->mutex));
                    free(successor); successor = NULL;
                    child[RIGHT] = 0;
                    protect =1;

                }
                else if(psuccessor == remove->right){
                    psuccessor ->left = successor->right;
                    pthread_mutex_unlock(&(successor->mutex));
                    free(successor); successor =NULL;
                }
                else{
                    psuccessor->left = successor->right;
                    pthread_mutex_unlock(&(successor->mutex));
                    free(successor); successor = NULL;
                    leaf = remove->right;
                    do{
                        leaf = leaf->left;
                        pthread_mutex_unlock(&(leaf->mutex));
    
                    }while(leaf != psuccessor);
                }
                // pthread_mutex_unlock(&(remove->mutex));
                //unlock : left right         
            }else if(child[LEFT]){ //ONE LEFT CHILD
                //premove->laststep = remove->left
                //lock : premove(or tree mutex) remove, LEFT           
                //case General
                child[LEFT] = 0;
                tree->root = remove->left;
                pthread_mutex_unlock(&(tree->root->mutex));
                //unlock left

            }else {//ONE RIGHT CHILD or No child
                //premove->laststep= remove->right;
                //lock : premove remove remove->right
                tree->root = remove->right; //could be tree->root=NULL;
                if(tree->root != NULL)
                    pthread_mutex_unlock(&(tree->root->mutex));
                //unlock right
            }
        }

        if(child[LEFT])
            pthread_mutex_unlock(&(remove->left->mutex));
        if(child[RIGHT])
            pthread_mutex_unlock(&(remove->right->mutex));
        
        pthread_mutex_unlock(&(remove->mutex));
        if( !(child[LEFT] & child[RIGHT]) && !(protect)){
            free(remove); remove = NULL;
        }
        state = 1;

        if(premove !=NULL)
            pthread_mutex_unlock(&(premove->mutex));
        else
            pthread_mutex_unlock(&tree_mutex);
        //unlock

    }else{
        //lock remove
        state = 0;
        pthread_mutex_unlock(&(remove->mutex));
    }
    
    return (state != 0 )? LAB2_SUCCESS:LAB2_ERROR; //No such Key
}
