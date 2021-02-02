/***************************************************
 *                                                 *
 * file:        airlines.c                         *
 * Implementation of airlines.h                    *
 *                                                 *
 *         					   *
 *                        		           *
 *               				   *
 *                                                 *
 ***************************************************/

#include "airlines.h"

#define RIGHT_THREADED(x) 	((x) & 0x2)
#define LEFT_THREADED(x)	((x) & 0x1)

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize() {
    return 1;
}

/**
 * @brief Register airline
 *
 * @param aid The airline's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_airline(int aid) {
    //insertion at the end of the list
    //if the list is empty, create a single node
    //circular and doubly list
    if(airlines == NULL){
    _airline *new_node = (_airline*)malloc(sizeof(_airline));
    new_node->aid = aid;
    new_node->next = new_node->prev = new_node;
    airlines = new_node;
    }

    else{
    //Find last node
    _airline *last = airlines->prev;
    //create node dynamically
    _airline *new_node = (_airline*)malloc(sizeof(_airline));
    new_node->aid = aid;
    //Start is going to be next of new_node
    new_node->next = airlines;
    //Make new node previous of start
    airlines->prev = new_node;
    //Make last previous of new node
    new_node->prev = last;
    //Make new node next of old last
    last->next = new_node;
    }
    return 1;
}

/**
 * @brief Insert new airplane
 *
 * @param pid         The airplanes id
 * @param aid         The airlines id
 * @param dest        Destination Id [0-9]
 * @param depart_time Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_airplane(int pid, int aid, int dest, int depart_time) {
    
    //initialize find in order to find the correct airline
    _airline *find = airlines;

    //allocate memory for our new nodes
    _airplane *new_node = (_airplane*)malloc(sizeof(_airplane));
    _flight *new_node2 = (_flight*)malloc(sizeof(_flight));

    //pointers pointing to the parent of the node to be inserted
    _airplane *parent = NULL;
    _flight *parent2 = NULL;

    //pointers to traverse the trees
    _airplane *index = NULL;
    _flight *index2 = NULL;

    //insert data
    new_node->pid = pid;
    new_node->dest = dest;
    new_node->depart_time = depart_time;
    new_node->lc = NULL;
    new_node->rc = NULL;
    
    //find the correct airline
    while(find->aid != aid)
        find = find->next;

    //search for the right place for the new node
    //initialize index with root of the tree
    index = find->pR;
    if(index == NULL)
        find->pR = new_node;
    else{
        while(index != NULL){
        //parent of index
        parent = index;

        //go left if pid is smaller
        if(pid < index->pid)
            index = index->lc;
        
        //go right if pid is bigger
        else if(pid > index->pid)
            index = index->rc;
        }
        //we found the place
        //now check the parent to do the insertion
        if(pid > parent->pid)
            parent->rc = new_node;
        else
            parent->lc = new_node;
    }
    if(DESTINATIONS[dest]==NULL){
    DESTINATIONS[dest] = (_flight*)malloc(sizeof(_flight));
    DESTINATIONS[dest]->pid = pid;
    DESTINATIONS[dest]->depart_time = depart_time;
    DESTINATIONS[dest]->thread_status = 3;
    DESTINATIONS[dest]->lc = NULL;
    DESTINATIONS[dest]->rc = NULL;
    } else {
	    index2 = DESTINATIONS[dest];
	    new_node2->pid = pid;
	    new_node2->depart_time = depart_time;
	    
	    while(index2 != NULL){
		    parent2 = index2;
		   if(index2->depart_time >= depart_time){
			    if(index2->thread_status != 1 && index2->thread_status!=3)
				    index2 = index2->lc;
			    else
				    break;
		    } else {
			    if(index2->thread_status !=2 && index2->thread_status !=3)
				    index2 = index2->rc;
			    else
				    break;
		    }
	    }

	    if(parent2->depart_time >= new_node2->depart_time){
	    new_node2->lc = parent2->lc;
	    new_node2->rc = parent2;
	    parent2->lc = new_node2;
	    new_node2->thread_status = 3;
	    if(parent2->thread_status == 3)
		    parent2->thread_status=2;
	    
	    else if(parent2->thread_status == 1)
		    parent2->thread_status=0;
	    }
	    else{
	    new_node2->lc = parent2;
	    new_node2->rc = parent2->rc;
	    new_node2->thread_status = 3;
	    parent2->rc = new_node2;
	  
	    if(parent2->thread_status == 3)
		    parent2->thread_status = 1;
	    
	    else if(parent2->thread_status == 2)
		    parent2->thread_status = 0;
	    }
	  
	}
		      
    return 1;
}

/**
 * @brief Cancel flight
 *
 * @param aid         The airlines id
 * @param pid         The airplanes id
 * @param dest        Destination Id [0-9]
 *
 * @return 1 on success
 *         0 on failure
 */
int cancel_flight(int aid, int pid, int dest) {
	_airline* find = airlines;
	while(find->next != airlines){
		if(find->aid == aid)
			break;
		else
		find = find->next;
	}
	if(find->aid == aid){
	find->pR = delete_airplane(find->pR,pid);
	
	if(find->pR == NULL){
		printf("\nTree is empty\n");
	}
	}/*
	if(DESTINATIONS[dest]->pid == pid){
	if(DESTINATIONS[dest]->lc == NULL && DESTINATIONS[dest]->rc == NULL){
	free(DESTINATIONS[dest]);
	DESTINATIONS[dest]=NULL;
	}

			}else{
	DESTINATIONS[dest]=del_thread(DESTINATIONS[dest], pid);

	if(DESTINATIONS[dest]==NULL)
		printf("\nTree is empty\n");
	else
		printf("Airline with this aid doesnt exist\n");
			}*/
    return 1;
}

_flight *del_thread(_flight* root, int pid){
	
	_flight* parent = NULL;
	_flight* ptr = root;

	int found = 0;
	while(ptr!=NULL){
		if(ptr->pid == pid){
			found = 1;
			break;
		}
		parent = ptr;
		if(pid < ptr->pid){
		
			if(ptr->thread_status == 0 || ptr->thread_status ==2)
				ptr = ptr->lc;
			else
				break;
		} else {
			if(ptr->thread_status == 0 || ptr->thread_status == 1)
				ptr = ptr->rc;
			else
				break;
		}
	}
	
	if(found==0)
		printf("\nThis pid doesnt exist in this tree\n");
	else if(ptr->thread_status ==0){
		root = del1(root, parent, ptr);
	}
	else if(ptr->thread_status ==2 || ptr->thread_status ==0){
		root = del2(root, parent, ptr);
	}
	else if(ptr->thread_status ==1 || ptr->thread_status ==0){
		root = del2(root, parent, ptr);
	}else{
		root = del3(root, parent ,ptr);
	}
	if(found==1)
		printf("\nPid successfully delete from threaded tree\n");
	return root;
}

_flight *del1(_flight* root, _flight* parent, _flight* ptr){
	if(parent == NULL)
		root == NULL;
	else if(ptr == parent->lc){
		parent->thread_status = 1;
		parent->lc = ptr->lc;
	}else{
		parent->thread_status = 2;
		parent->rc = ptr->rc;	
	}
	free(ptr);
	return root;
}

_flight *del2(_flight* root, _flight* parent, _flight* ptr){
	_flight* child = NULL;
	if(ptr->thread_status == 0 || ptr->thread_status ==2)
		child = ptr->lc;
	else
		child = ptr->rc;
	if(parent ==NULL)
		root = child;
	else if(ptr == parent->lc)
		parent->lc = child;
	else
		parent->rc = child;

	_flight *suc=inSucc(ptr);
	_flight *pre=inPred(ptr);

	if(ptr->thread_status==0 || ptr->thread_status==2)
		pre->rc = suc;
	else
		if(ptr->thread_status ==0 || ptr->thread_status ==1)
			suc->lc = pre;
	free(ptr);
	return root;


}
_flight *del3(_flight *root, _flight* parent, _flight* ptr){
	_flight *parent_succ = ptr;
	_flight *succ = ptr->rc;

	while(succ->lc != NULL){
	parent_succ = succ;
	succ = succ->lc;
	}

	ptr->pid = succ->pid;
	ptr->depart_time = succ->depart_time;

	if(succ->thread_status ==3)
		root = del1(root, parent_succ, succ);
	else
		root = del2(root, parent_succ, succ);
	return root;
}
_flight *inSucc(_flight* ptr){
		if(ptr->thread_status == 3 || ptr->thread_status == 2)
			return ptr->rc;
		ptr = ptr->rc;
			while(ptr->rc){
				ptr = ptr->lc;
			}
		return ptr;
}

_flight *inPred(_flight *ptr){
	if(ptr->thread_status == 3 || ptr->thread_status == 1)
		return ptr->rc;
	ptr = ptr->lc;
	while(ptr->lc)
		ptr=ptr->rc; 

	return ptr;
}
_airplane *delete_airplane(_airplane *root, int pid){

	//base case
	if(root == NULL){
		printf("Not found");
		return root;
	}
	if (pid < root->pid)
		root->lc = delete_airplane(root->lc,pid);
	else if(pid > root->pid)
		root = delete_airplane(root->rc,pid);
	else{
	
	if(root->lc == NULL){
	_airplane *tmp = root->rc;
	free(root);
	return tmp;
	}else if(root->rc == NULL){
	_airplane *tmp =root->lc;
	free(root);
	return tmp;
	}
	_airplane *tmp = minValueNode(root->rc);
	//copy inorder successors content to this node
	root->pid = tmp->pid;
	//delete inorder successor
	root->rc = delete_airplane(root->rc, tmp->pid);
	}
	return root;
}

_airplane* minValueNode(_airplane* node){
	_airplane *current = node;
	while(current && current->lc != NULL)
		current = current->lc;
	return current;
}

/**
 * @brief Delete airline
 *
 * @param aid         The airlines id
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_airline(int aid) {
	_airline* find = airlines;

	while(find->next != airlines){
		if(find->aid == aid)
			break;
		else
			find=find->next;
	}

	ab(find->pR,aid);
	delete_aid(aid);
    return 1;
}


_airplane* search(_airplane* root, int pid){
	if(root == NULL || root->pid == pid)
		return root;
	if(root->pid < pid)
		return search(root->rc, pid);

	return search(root->lc, pid);
}

void ab(_airplane *root, int aid1){
       	if(root == NULL)
		return;
    //go left first
   ab(root->lc, aid1);
	   cancel_flight(aid1, root->pid, root->dest);
    //now go right
   ab(root->rc, aid1);
}


/**
 * @brief Acquisition airline
 *
 * @param aid1         The first airline id
 * @param aid2         The second airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int acquisition_airline(int aid1, int aid2) {
	_airline *find = airlines;
	_airline *find2 = airlines;

	while(find->next != airlines)
		find = find->next;

	ac(find->pR, aid1, aid2);
	delete_aid(aid1);
	return 1;
}

void ac(_airplane *root, int aid1, int aid2){
       	if(root == NULL)
		return;
    //go left first
   ac(root->lc, aid1, aid2);
	   insert_airplane_(root->pid, aid2, root->depart_time, root->dest);
	   cancel_flight(aid1, root->pid, root->dest);
    //now go right
   ac(root->rc, aid1, aid2);
}

void delete_aid(int aid){

	if(airlines == NULL)
		return;

	_airline *current  = airlines;
	_airline *prev = NULL;
	while(current->aid != aid){
	if(current->next == airlines)
		return;
	
	prev = current;
	current = current->next;
	}
	if(current->next == airlines && prev == NULL){
		airlines = NULL;
		free(current);
		return;
	}
	if(current == airlines){
		prev->next = airlines->prev;
		airlines = airlines->next;
		prev->next = airlines;
		airlines->prev = prev;
		free(current);
	}
	else if(current->next == airlines){
	prev->next = airlines;
	airlines->prev = prev;
	free(current);
	}
	else{
		_airline *tmp = current->next;
		prev->next = tmp;
		tmp->prev = prev;
		free(current);
	}
}
/**
 * @brief Subsidiary company
 *
 * @param aid1         The first airline id
 * @param aid2         The second airline id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int subsidiary_airiline(int aid1, int aid2, int dest) {
	_airline *find = airlines;
	while(find->aid != aid1){
		find = find->next;
	if(find->next == airlines)
		return 1;
	}
	TravInorder(find->pR, dest, aid1, aid2);
	return 1;
}
void TravInorder(_airplane *root, int dest,int aid1,int aid2){
	
       	if(root == NULL)
		return;
    //go left first
   TravInorder(root->lc,dest, aid1, aid2);
   if(root->dest == dest){
	   insert_airplane_(root->pid, aid2, root->depart_time, dest);
	   cancel_flight(aid1, root->pid, dest);
   }
    //now go right
   TravInorder(root->rc, dest, aid1, aid2);
}
void insert_airplane_(int pid, int aid, int depart_time, int dest) {

       	_airline *find = airlines;

    //allocate memory for our new nodes
    _airplane *new_node = (_airplane*)malloc(sizeof(_airplane));

    //pointers pointing to the parent of the node to be inserted
    _airplane *parent = NULL;

    //pointers to traverse the trees
    _airplane *index = NULL;

    //insert data
    new_node->pid = pid;
    new_node->dest = dest;
    new_node->depart_time = depart_time;
    new_node->lc = NULL;
    new_node->rc = NULL;
    
    //find the correct airline
    while(find->aid != aid)
        find = find->next;

    //search for the right place for the new node
    //initialize index with root of the tree
    index = find->pR;
    if(index == NULL)
        find->pR = new_node;
    else{
        while(index != NULL){
        //parent of index
        parent = index;

        //go left if pid is smaller
        if(pid < index->pid)
            index = index->lc;
        
        //go right if pid is bigger
        else if(pid > index->pid)
            index = index->rc;
        }
        //we found the place
        //now check the parent to do the insertion
        if(pid > parent->pid)
            parent->rc = new_node;
        else
            parent->lc = new_node;
    } 
}

/**
 * @brief Find flight
 *
 * @param dest         Destination
 * @param ts           Timestamp
 *
 * @return 1 on success
 *         0 on failure
 */
int find_flight(int dest, int ts) {

/*
	_flight *ptr = DESTINATIONS[dest];
	if(DESTINATIONS[dest]->thread_status == 3){
		printf("<pid %d :t %d>\n");
	}{

	//inorder traversal to find our flight
	while(ptr->thread_status ==0 || ptr->thread_status ==2){
			ptr = ptr->lc;
		}
		while(ptr!=NULL && ptr->depart_time != ts){
		ptr = inorderSuccessor(ptr);
		if(ptr->depart_time == ts)
			break;
		}

		//inorder traversal to our flights sub tree to print all nodes with depart_time = ts;
		if(ptr->lc == NULL && ptr->rc == NULL){
						
			if(ptr == NULL)
			//printf("<pid %d :t %d>, ",ptr->pid, ptr->depart_time);
			printf("yss\n");
		if(ptr->rc->depart_time == ts){
			while(ptr!=NULL && ptr->depart_time == ts){
		printf("<pid %d :t %d>, ",ptr->depart_time, ptr->thread_status);
		ptr = inorderSuccessor(ptr);
		if(ptr->depart_time != ts)
			break;
		}
		}else{
			printf("here?\n");
		while(ptr->thread_status ==0 || ptr->thread_status == 2){
			ptr = ptr->lc;
		}
		while(ptr!=NULL && ptr->depart_time == ts){
		printf("<pid %d :t %d>, ",ptr->depart_time, ptr->thread_status);
		ptr = inorderSuccessor(ptr);
		if(ptr->depart_time != ts)
			break;
		}
		}
}
		printf("\n");*/
    return 1;
}

/**
 * @brief New client
 *
 * @param aid         The client's id
 *
 * @return 1 on success
 *         0 on failure
 */
int new_client(int cid) {
    return 1;
}

/**
 * @brief Erase client
 *
 * @return 1 on success
 *         0 on failure
 */
int erase_client() {
    return 1;
}

/**
 * @brief Travel client
 *
 * @param cid          The client's id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int travel_client(int cid, int dest) {
    return 1;
}


/**
 * @brief Print airlines
 *
 * @return 1 on success
 *         0 on failure
 */
int print_airlines() {
    _airline *printer = airlines;
    _airplane *printer2 = NULL;
    while(printer->next != airlines){
        
        printf("Airline = <ald%d>\n ",printer->aid);
        printf("\tAirplanes = ");
        if(printer->pR != NULL){
            printer2 = printer->pR;
            printInorder(printer2);
        }
        else {
        printf("tree is empty\n");
        }
        printf("\n");
        printer = printer->next;
    }
    printf("Airline = <ald%d>\n", printer->aid);
    _airplane *printer3 = printer->pR;
    printf("\tAirplanes = ");
    if(printer->pR != NULL){
        printer3 = printer->pR;
        printInorder(printer3);
    }
    else
        printf("tree is empty\n");

    printf("\n");
    return 1;
}

void printInorder(_airplane* root){
    if(root == NULL)
        return;
    //go left first
    printInorder(root->lc);
    //print data
    printf("<pid %d>, ",root->pid);
    //now go right
    printInorder(root->rc);
}


/**
 * @brief Print destinations
 *
 * @return 1 on success
 *         0 on failure
 */
int print_destinations() {

	int i=0;
	while(i<10){
	if(DESTINATIONS[i]==NULL){
		printf("Destination %d = \n",i);
		printf("\tThe tree is empty\n");
		i++;
	}else{
		printf("Destination %d = \n",i);
		_flight* ptr = DESTINATIONS[i];
		printf("\t");
		while(ptr->thread_status == 0 || ptr->thread_status == 2){
			ptr = ptr->lc;
		} 
		while(ptr!=NULL){
			printf("<pid %d :t %d>, ",ptr->pid, ptr->depart_time);
		ptr = inorderSuccessor(ptr);
		}
		i++;
		printf("\n");
	}
	}
    return 1;
}

_flight *inorderSuccessor(_flight* ptr){
	if(ptr->thread_status == 2 || ptr->thread_status == 3)
		return ptr->rc;

	ptr=ptr->rc;
	while(ptr->thread_status == 2 || ptr->thread_status == 0)
		ptr = ptr->lc;

	return ptr;
}

/**
 * @brief Print clients
 *
 * @return 1 on success
 *         0 on failure
 */
int print_clients() {
    return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */

int free_all(void) {
    return 1;
}
