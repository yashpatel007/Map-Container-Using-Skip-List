// C++ code for inserting element in skip list 

#include <bits/stdc++.h> 
#include <iostream>
#include <utility>
using namespace std; 

// Class to implement node 
template <typename key_T, typename val_T>
class Node
{ 
public: 	
  pair<key_T, val_T> value;
  // Array to hold pointers to node of different level 
	Node **forward; 
	Node(pair<key_T, val_T>,int); 
}; 
template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(pair<key_T,val_T> p ,int level) 
{ 
this->value=p;
	// Allocate memory to forward 
	forward = new Node<key_T, val_T>*[level+1]; 
	// Fill forward array with 0(NULL) 
	memset(forward, 0, sizeof(Node<key_T, val_T>*)*(level+1)); 
}; 

// Class for Skip list 
template <typename key_T, typename val_T>
class Map
{ 
	// Maximum level for this skip list 
	int MAXLVL; 

	// P is the fraction of the nodes with level 
	// i pointers also having level i+1 pointers 
	float P; 

	// current level of skip list 
	int level; 

	// pointer to header node
  
	Node<key_T, val_T> *header; 
  public: 
	Map<key_T, val_T>(int, float); 
	int randomLevel(); 
	Node<key_T, val_T>* createNode(pair <key_T, val_T>, int); 
  
	void insert(pair<key_T, val_T>); 
  void deleteElement(key_T); 
  void searchElement(key_T); 
	void displayList(); 
}; 

template <typename key_T, typename val_T>
Map<key_T, val_T>::Map(int MAXLVL, float P) 
{ 
	this->MAXLVL = MAXLVL; 
	this->P = P; 
	level = 0; 
  key_T k;// null init key in header
  val_T v;//no init value in header
	// create header node and initialize key to -1 
	header = new Node<key_T, val_T>(std::make_pair(k,v),MAXLVL); 
}; 

// create random level for node 
template <typename key_T, typename val_T>
int Map<key_T, val_T>::randomLevel() 
{ 
	float r = (float)rand()/RAND_MAX; 
	int lvl = 0; 
	while (r < P && lvl < MAXLVL) 
	{ 
		lvl++; 
		r = (float)rand()/RAND_MAX; 
	} 
	return lvl; 
}; 

// create new node 
template <typename key_T, typename val_T>
Node<key_T, val_T>* Map<key_T, val_T>::createNode(pair <key_T, val_T> p,int level) 
{ 
  cout<<"node created with"<<p.first<<"and lev:"<<level<<endl;
	Node<key_T, val_T> *n = new Node<key_T, val_T>(p,level); 
	return n; 
}; 

// Insert given key in skip list 
template <typename key_T, typename val_T>
void Map<key_T, val_T>::insert(pair<key_T, val_T> p) 
{ 
  // key_T key = p.first;
  // val_T val = p.second;

	Node<key_T, val_T> *current = header; 

	// create update array and initialize it 
	Node<key_T, val_T> *update[MAXLVL+1]; 
	memset(update, 0, sizeof(Node<key_T, val_T>*)*(MAXLVL+1)); 

	/* start from highest level of skip list 
		move the current pointer forward while key 
		is greater than key of node next to current 
		Otherwise inserted current in update and 
		move one level down and continue search 
	*/
	for (int i = level; i >= 0; i--) 
	{ 
		while (current->forward[i] != NULL && 
			current->forward[i]->value.first < p.first) 
			current = current->forward[i]; 
		  update[i] = current; 
	} 

	/* reached level 0 and forward pointer to 
	right, which is desired position to 
	insert key. 
	*/
	current = current->forward[0]; 

	/* if current is NULL that means we have reached 
	to end of the level or current's key is not equal 
	to key to insert that means we have to insert 
	node between update[0] and current node */
	if (current == NULL || current->value.first != p.first) 
	{ 
		// Generate a random level for node 
		int rlevel = randomLevel(); 

		// If random level is greater than list's current 
		// level (node with highest level inserted in 
		// list so far), initialize update value with pointer 
		// to header for further use 
		if (rlevel > level) 
		{ 
			for (int i=level+1;i<rlevel+1;i++) 
				update[i] = header; 

			// Update the list current level 
			level = rlevel; 
		} 

		// create new node with random level generated 
		Node<key_T, val_T>* n = createNode(p,rlevel); 

		// insert node by rearranging pointers 
		for (int i=0;i<=rlevel;i++) 
		{ 
			n->forward[i] = update[i]->forward[i]; 
			update[i]->forward[i] = n; 
		} 
		cout << "Successfully Inserted key " << p.first<< "\n"; 
	} 
}; 

template <typename key_T, typename val_T>
void Map<key_T, val_T>::deleteElement(key_T key) 
{ 
    Node<key_T, val_T> *current = header; 
  
    // create update array and initialize it 
    Node<key_T, val_T> *update[MAXLVL+1]; 
    memset(update, 0, sizeof(Node<key_T, val_T>*)*(MAXLVL+1)); 
  
    /*    start from highest level of skip list 
        move the current pointer forward while key  
        is greater than key of node next to current 
        Otherwise inserted current in update and  
        move one level down and continue search 
    */
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] != NULL  && 
              current->forward[i]->value.first < key) 
            current = current->forward[i]; 
        update[i] = current; 
    } 
  
    /* reached level 0 and forward pointer to  
       right, which is possibly our desired node.*/
    current = current->forward[0]; 
  
    // If current node is target node 
    if(current != NULL and current->value.first == key) 
    { 
        /* start from lowest level and rearrange 
           pointers just like we do in singly linked list 
           to remove target node */
        for(int i=0;i<=level;i++) 
        { 
            /* If at level i, next node is not target  
               node, break the loop, no need to move  
              further level */
            if(update[i]->forward[i] != current) 
                break; 
  
            update[i]->forward[i] = current->forward[i]; 
        } 
  
        // Remove levels having no elements  
        while(level>0 && 
              header->forward[level] == 0) 
            level--; 
         cout<<"Successfully deleted key "<<key<<"\n"; 
    } 
}; 
  
// Search for element in skip list 
template <typename key_T, typename val_T>
void Map<key_T, val_T>::searchElement(key_T key) 
{ 
    Node<key_T, val_T> *current = header; 
  
    /*    start from highest level of skip list 
        move the current pointer forward while key  
        is greater than key of node next to current 
        Otherwise inserted current in update and  
        move one level down and continue search 
    */
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] && 
               current->forward[i]->value.first < key) 
            current = current->forward[i]; 
  
    } 
  
    /* reached level 0 and advance pointer to  
       right, which is possibly our desired node*/
    current = current->forward[0]; 
  
    // If current node have key equal to 
    // search key, we have found our target node 
    if(current and current->value.first == key){  
        cout<<"Found key: "<<key<<"\n";}
    else{
      cout<<"KEY not found"<<endl;
    } 
};

// Display skip list level wise 
template <typename key_T, typename val_T>
void Map<key_T, val_T>::displayList() 
{ 
	cout<<"\n*****Skip List*****"<<"\n"; 
	for (int i=0;i<=level;i++) 
	{ 
		Node<key_T, val_T> *node = header->forward[i]; 
		cout << "Level " << i << ": "; 
		while (node != NULL) 
		{ 
			cout << "key" <<node->value.first;
			node = node->forward[i]; 
		} 
		cout << "\n"; 
	} 
}; 


struct person{
  char * name;
};



// Driver to test above code 
int main() 
{ 
	// Seed random number generator 
	srand((unsigned)time(0)); 

	// create SkipList object with MAXLVL and P 
	Map<int, int>lst(3, 0.5); 

	lst.insert(std::make_pair(2,3)); 
	lst.insert(std::make_pair(3,4)); 
	lst.insert(std::make_pair(4,5)); 


  lst.searchElement(2);
  lst.searchElement(7);
  lst.displayList();

  lst.deleteElement(3); 
  lst.displayList();

cout<<"lets test it on other types"<<endl;

Map<int, person> mp(3, 0.5);
person p1;
p1.name="yash";
mp.insert(std::make_pair(1, p1));



} 

