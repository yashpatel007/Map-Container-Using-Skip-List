// C++ code for inserting element in skip list 
#include <bits/stdc++.h> 
#include <iostream>
#include <utility>

//using namespace std; 

// Class to implement node 
template <typename key_T, typename val_T>
class Node
{ 
public:   
  std::pair<key_T, val_T> value ;
  // Array to hold pointers to node of different level 
  Node **forward;
  Node *previous; 
  Node(std::pair<key_T, val_T>,int);
  Node(int); 
  ~Node(){
  delete [] forward;
  //delete value;
  delete previous;
  }
  
}; 
template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(std::pair<key_T,val_T> p ,int level) 
{ 
  this->value=p;
  // Allocate memory to forward 
  forward = new Node<key_T, val_T>*[level+1]; 
  // Fill forward array with 0(NULL) 
  memset(forward, 0, sizeof(Node<key_T, val_T>*)*(level+1));
  //pointer to previous node
  previous = 0; 
}; 

template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(int level) 
{ 
   this->value.first=NULL;
  // Allocate memory to forward 
  forward = new Node<key_T, val_T>*[level+1]; 
  // Fill forward array with 0(NULL) 
  memset(forward, 0, sizeof(Node<key_T, val_T>*)*(level+1));
  //pointer to previous node
  previous = 0; 
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
  // size of the skip-list
  int slsize;
  // pointer to header node
  Node<key_T, val_T> *header;
  //pointer to tail node
  Node<key_T, val_T> *tail;

  public:
  Map();
//  ~Map(){
//        Node<key_T, val_T> *tem_header = header;
//        Node<key_T, val_T> *temp;
//	while(tem_header != NULL){
//	temp = tem_header->forward[0];
//	//delete &tem_header;
//	tem_header = temp;
//	}
//  }
  
  Map<key_T, val_T>(int, float); 
  int randomLevel(); 
  Node<key_T, val_T>* createNode(std::pair <key_T, val_T>, int); 
  
  //pair<pair<key_T,val_T>&,bool>
 
   
  //const val_T& at(const key_T); 
  void displayList();
  std::size_t size() const ;
  bool empty() const{return slsize==0;}
  //template <typename key_T, typename val_T>
  class Iterator{
    public:
    //watch your steps <-> constructor overloading happens here
    Iterator(const Iterator& iter) : cur(iter.get_current()){}
    Iterator(Node<key_T, val_T>* node) : cur(node){}
    Node<key_T, val_T>* get_current() const{return cur;}
    
    Iterator& operator=(const Iterator& itr){
        cur = itr.get_current();
	return *this;
    }
    
    Iterator& operator++(){
	if(cur == NULL) {
              return *this;
            }
            cur = cur->forward[0];
            return *this;
    }

    Iterator operator++(int){
	Map<key_T, val_T>::Iterator ret = *this;
	if(cur == NULL) return *this;
        cur = cur->forward[0];
	return ret;
    }
    Iterator& operator--(){
	if(cur == NULL) return *this;
	cur = cur->previous;
	return *this;
    }
    Iterator operator--(int){
	Map<key_T, val_T>::Iterator retrn = *this;
	if(cur == NULL) return *this;
        cur = cur->previous;
	return retrn;
    }
    
    val_T& operator*() const{
        return *cur->value.second;
    }
    val_T* operator->() const{
	return cur->value.second;
    }					
    
    ~Iterator(){
        // setting cirrent as zero .....cus it is what it is dude..:)
        cur = 0;
    };
    private:
        Node<key_T, val_T> *cur;
  
  };
  
  class ConstIterator{
    public:
    ConstIterator(const ConstIterator& citr) : cur(citr.get_current()){}
    ConstIterator(const Iterator& iter) : cur(iter.get_current()){}
    ConstIterator(Node<key_T, val_T> *node) : cur(node){}
    Node<key_T, val_T>* get_current() const {return cur;}
    
    
    //defining operators for ConstIterator
    ConstIterator& operator=(const ConstIterator& citr){
    cur = citr.get_current();
    return *this;
    }
    ConstIterator& operator++(){
    if(cur == NULL) return *this;
    cur = cur->forward[0];
    return *this;
    }
    ConstIterator operator++(int){
    Map<key_T, val_T>::ConstIterator ret = *this;
    if(cur == NULL) return ret;
    cur = cur->forward[0];
    return ret;
    }
    ConstIterator& operator--(){
    if(cur == NULL) return *this;
    cur = cur->previous;
    return *this;
    }
    ConstIterator operator--(int){
    Map<key_T, val_T>::ConstIterator ret = *this;
    if(cur == NULL) return ret;
    cur = cur->previous;
    return ret;
    }
    val_T& operator*() const{
	return *cur->value;
    }
    val_T* operator->() const{
    return cur->value;
    }
    
    ~ConstIterator(){
        cur = 0;
    };
    
  private:
      Node<key_T,val_T> *cur;
  };
  
  class ReverseIterator{
     public:
    //watch your steps <-> constructor overloading happens here
    ReverseIterator(const ReverseIterator& iter) : cur(iter.get_current()){}
    ReverseIterator(Node<key_T, val_T>* node) : cur(node){}
    Node<key_T, val_T>* get_current() const{return cur;}
    
    ReverseIterator& operator=(const ReverseIterator& itr){
        cur = itr.get_current();
	return *this;
    }
    
    ReverseIterator& operator++(){
	if(cur == NULL) return *this;
        cur = cur->forward[0];
	return *this;
    }

    ReverseIterator operator++(int){
	 Map<key_T, val_T>::ReverseIterator ret = *this;
         if(cur == NULL) return ret;
         cur = cur->forward[0];
         return ret;
    }
    ReverseIterator& operator--(){
	if(cur == NULL) return *this;
	cur = cur->previous;
	return *this;
    }
    ReverseIterator operator--(int){
	Map<key_T, val_T>::ReverseIterator retrn = *this;
	if(cur == NULL) return *this;
        cur = cur->previous;
	return retrn;
    }
    val_T& operator*() const{
	return *cur->value;
    }
    val_T* operator->() const{
    return cur->value;
    }
    
    ~ReverseIterator(){
        // setting cirrent as zero .....cus it is what it is dude..:)
        cur = 0; 
    }
    private:
    Node<key_T,val_T> *cur;
    
  };
  
  // iterator functions
  Iterator begin();
  Iterator end();
  ConstIterator begin() const{return ConstIterator(header->forward[0]);}
  ConstIterator end() const{return Map<key_T, val_T>::ConstIterator(tail->previous);}
  ReverseIterator rbegin();
  ReverseIterator rend();
  
  //map function for element access 
  std::pair<Iterator,bool> insert(std::pair<key_T, val_T>);
  
  //at function
  val_T& at(const key_T&);
  const val_T& at(const key_T&)const;
  
  //erase
  void erase(const key_T&); 
  void erase(Iterator);
  
  //clear
  void clear();
   //find
  Iterator find(const key_T);
  ConstIterator find(const key_T key) const{
  Node<key_T, val_T> *current = header;   
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] && 
            current->forward[i]->value.first < key) 
            current = current->forward[i];
    }   
    current = current->forward[0]; 
    if(current and current->value.first == key){  
        std::cout<<"Found key: "<<key<<"\n";
        return ConstIterator(current);
    }
    else{
      std::cout<<"KEY not found"<<std::endl;
      return ConstIterator(tail->previous);
    } 
  };// find ends
  
  
  
};

template <typename key_T, typename val_T>
Map<key_T, val_T>::Map() 
{ //create an empty container
  this->MAXLVL = 100; 
  this->P = 0.5; 
  level = 0; 
  key_T k;// null init key in header
  val_T v;//no init value in header
  //size --added
  slsize=0;
  // create header node  
  header = new Node<key_T, val_T>(MAXLVL); 
  // init tail node
  tail = new Node<key_T, val_T>(MAXLVL);
  
  //tail prev is header
  //header->forward[0]=tail;
  tail->previous = header;
  // header prev = null
	header->previous = 0; 
  tail->forward[0] =NULL;
};

template <typename key_T, typename val_T>
Map<key_T, val_T>::Map(int MAXLVL, float P) 
{ 
  this->MAXLVL = MAXLVL; 
  this->P = P; 
  level = 0; 
  key_T k;// null init key in header
  val_T v;//no init value in header
  //size --added
  slsize=0;
  // create header node  
  header = new Node<key_T, val_T>(MAXLVL); 
  // init tail node
  tail = new Node<key_T, val_T>(MAXLVL);
  
  //tail prev is header
  //header->forward[0]=tail;
  tail->previous = header;
  // header prev = null
	header->previous = 0; 
  tail->forward[0] =NULL;
};

template <typename key_T, typename val_T>
void Map<key_T, val_T>::clear(){
      Node<key_T, val_T> *temp_head = header;
      Node<key_T, val_T> *temp;
      if(temp != NULL){std::cout<<"yes";}
      while(temp != NULL){
        temp = temp_head->forward[0];
	std::cout<<temp_head->value.first<<std::endl;
        delete temp_head;
        std::cout<<"deleted"<<std::endl;
        temp_head = temp;
	}
		slsize=0;
		header=NULL;
                tail = NULL;
                Map();
		
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::Iterator Map<key_T, val_T>::begin(){
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::Iterator(header->forward[0]);
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::Iterator Map<key_T, val_T>::end(){
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::Iterator(tail->previous);
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::ReverseIterator Map<key_T, val_T>::rbegin(){
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::ReverseIterator(tail->previous);
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::ReverseIterator Map<key_T, val_T>::rend(){
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::ReverseIterator(header->forward[0]);
}

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
Node<key_T, val_T>* Map<key_T, val_T>::createNode(std::pair <key_T, val_T> p,int level) 
{ 
  //cout<<"node created with"<<p.first<<"and lev:"<<level<<endl;
  Node<key_T, val_T> *n = new Node<key_T, val_T>(p,level); 
  return n; 
}; 

// Insert given key in skip list 
template <typename key_T, typename val_T>
std::pair<typename Map<key_T,val_T>::Iterator,bool> Map<key_T, val_T>::insert(std::pair<key_T, val_T> p) 
{  
  // key_T key = p.first;
  // val_T val = p.second;
  // pair<Iterator,bool>
  // make temp uiterator as Iterator temp = Iterator(n<-newly created node); and return the pointer
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

  //========JUST EXPERIMENTING ,AYE===========================
     // while(current->forward[0] != tail && current->forward[0]->value.first < p.first){
    // 		current = current->forward[0];
   // 	}//not working ...umm
  //=================================== 

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
    for(int i=0;i<=rlevel;i++) 
    { 
      n->forward[i] = update[i]->forward[i]; 
      update[i]->forward[i] = n; 
    }

    
    n->previous = update[0];

    std::cout<< n->previous->value.first<<std::endl;
		
        if(n->forward[0] != NULL){
        //std::cout<<"inif";
	n->forward[0]->previous = n;
	}
	else{
            //std::cout<<"in else";
            tail->previous = n;
	} 
    
    std::cout << "Successfully Inserted key " << p.first<< "\n";
    this->slsize++; 
    Iterator retit = Iterator(n);
    return std::make_pair(retit, true);
    }else{
    Iterator retit = Iterator(current);
    std::cout<<"key already there"<<std::endl;
    return std::make_pair(retit, false);
    } 

}; 

template <typename key_T, typename val_T>


 void Map<key_T, val_T>::erase(const key_T& key) 
{ 
    Node<key_T, val_T> *current = header; 
    // create update array and initialize it 
    Node<key_T, val_T> *update[MAXLVL+1]; 
    memset(update,0, sizeof(Node<key_T, val_T>*)*(MAXLVL+1)); 
  
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
       // displayList();
        
        //======================================
         if(current->forward[0] != NULL){
            current->forward[0]->previous = current->previous;
        }
        else{
            tail->previous = current->previous;
            //displayList();
            current->previous->forward[0] = NULL;
            //displayList();
            }

        //======================================
  
        // Remove levels having no elements  
        while(level>0 && 
              header->forward[level] == 0) 
            level--; 
        
         
         std::cout<<"Successfully deleted key "<<key<<"\n";
         this->slsize--; 
    }else{
        throw std::out_of_range("Key Not in Map");
    } 
}; 
  
template <typename key_T, typename val_T>
void Map<key_T, val_T>::erase(Iterator it) 
{ 
    key_T key = it.get_current()->value.first;
    Node<key_T, val_T> *current = header; 
    // create update array and initialize it 
    Node<key_T, val_T> *update[MAXLVL+1]; 
    memset(update,0, sizeof(Node<key_T, val_T>*)*(MAXLVL+1)); 
    
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] != NULL  && 
              current->forward[i]->value.first < key) 
            current = current->forward[i]; 
        update[i] = current; 
    } 
  
    current = current->forward[0]; 
  
    // If current node is target node 
    if(current != NULL and current->value.first == key) 
    { 
        for(int i=0;i<=level;i++) 
        { 
                if(update[i]->forward[i] != current) 
                break; 
  
            update[i]->forward[i] = current->forward[i]; 
        } 
       // displayList();
        
        //======================================
         if(current->forward[0] != NULL){
            current->forward[0]->previous = current->previous;
        }
        else{
            tail->previous = current->previous;
            //displayList();
            current->previous->forward[0] = NULL;
            //displayList();
            }

        //======================================
  
        // Remove levels having no elements  
        while(level>0 && 
              header->forward[level] == 0) 
            level--; 
        
         
         std::cout<<"Successfully deleted key "<<key<<"\n";
         this->slsize--; 
    } 
};

// Search for element in skip list 
template <typename key_T, typename val_T>
typename Map<key_T, val_T>::Iterator Map<key_T, val_T>::find(const key_T key)
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
        std::cout<<"Found key: "<<key<<"\n";
        return Iterator(current);
    }
    else{
      std::cout<<"KEY not found"<<std::endl;
      return Iterator(tail->previous);
    } 
};



template <typename key_T, typename val_T>
val_T& Map<key_T, val_T>::at(const key_T& key) 
{ 
    Node<key_T, val_T> *current = header; 
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] && 
        current->forward[i]->value.first < key) 
        current = current->forward[i]; 
    } 
    current = current->forward[0];
    if(current and current->value.first == key){
      std::cout<<"Key found: ";  
    return current->value.second;
    }
    else{
      std::cout<<"KEY not found"<<std::endl;
      throw  std::out_of_range("key not found");
    } 
};

template <typename key_T, typename val_T>
const val_T& Map<key_T, val_T>::at(const key_T& key) const
{ 
    Node<key_T, val_T> *current = header; 
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] && 
        current->forward[i]->value.first < key) 
        current = current->forward[i]; 
    } 
    current = current->forward[0];
    if(current and current->value.first == key){
      std::cout<<"Key found: ";  
      return current->value.second;
    }
    else{
      std::cout<<"KEY not found"<<std::endl;
      throw  std::out_of_range("key not found");
    } 
};

// Display skip list level wise 
template <typename key_T, typename val_T>
void Map<key_T, val_T>::displayList() 
{ 
  std::cout<<"\n*****Skip List*****"<<"\n"; 
  for (int i=0;i<=level;i++) 
  { 
    Node<key_T, val_T> *node = header->forward[i]; 
    std::cout << "Level " << i << ": "; 
    while (node != NULL) 
    { 
      std::cout << "key" <<node->value.first;
      node = node->forward[i]; 
    } 
    std::cout << "\n"; 
  }
  std::cout<<"********************"<<std::endl;
}; 

template <typename key_T, typename val_T>
std::size_t Map<key_T, val_T>::size() const
{ 
  //std::cout<<"Size is: "<<this->slsize<<std::endl;
  return this->slsize;
}

struct person{
  char * name;
};

// Driver to test above code 
int main() 
{ 
  // Seed random number generator 
  srand((unsigned)time(0)); 
//  // create SkipList object with MAXLVL and P 
//  Map<int, int>lst(3, 0.5); 
//  //lst.displayList();
//  
//  // Ok lets text everything that is implemented ........RUM RUM ...
//  lst.insert(std::make_pair(2,22)); 
//  lst.insert(std::make_pair(3,33)); 
//  lst.insert(std::make_pair(4,44));
//  auto k = lst.insert(std::make_pair(5,55));
//  lst.insert(std::make_pair(6,66));
//  lst.insert(std::make_pair(7,77));
//  lst.insert(std::make_pair(8,88));
//  //lst.displayList();-> list is empty
//  //SIZE function
//  std::cout<<"Size is: "<<lst.size()<<std::endl;
//  
//  //k should be an iterator and bool pair
//  auto e =k.first;
//  std::cout<<"e from k is :"<<e.get_current()->value.second<<std::endl;
//  e--;// check if k is actually an iterator
//  std::cout<<"e from k  is :"<<e.get_current()->value.second<<std::endl;
//  //check if iterator is padded by refrence
//  e.get_current()->value.second=444;
//  std::cout<<"e from k  pbr check is :"<<e.get_current()->value.second<<std::endl;
//  e.get_current()->value.second=44;//lets just change it back.
// 
//  // testing at function ->  mapped_T at(key)
//   std::cout<<"element at key 2 is :"<<lst.at(2)<<std::endl;
//  // checking the ret value of at is by refrence or not 
//  lst.at(2)=222;
//  std::cout<<"element at pbr key 2 is :"<<lst.at(2)<<std::endl;
//  lst.at(2)=22;
//  
//  
//  //===============================try erasing the value 
//  lst.erase(5);
//  lst.displayList();
//  lst.insert(std::make_pair(5,55));
//  //lst.displayList();
//  // lets see the issue// just in case you are thinking JCI -> just checking iterator
//  auto jci = lst.end();
//  std::cout<<"JCI:"<<jci.get_current()->value.first<<std::endl;
//  jci--;
//  std::cout<<"JCI:"<<jci.get_current()->value.first<<std::endl;
//  //====================CONST ITERATOR==================
//  Map<int,int>::ConstIterator ci =lst.begin();  
//  std::cout<<"CI:"<<ci.get_current()->value.first<<std::endl;
//  ci++;
//  std::cout<<"CI:"<<ci.get_current()->value.first<<std::endl;
//  ci--;
//  std::cout<<"CI:"<<ci.get_current()->value.first<<std::endl;
//  //this should fail ----- is happening---need to see ******
//  ci.get_current()->value.second=222;
//  std::cout<<"CI: changing second "<<ci.get_current()->value.second<<std::endl;
//  ci.get_current()->value.second=22;// change back the value
//  //=================================================================
//  auto it = lst.begin();
//  std::cout<<it.get_current()->value.second<<std::endl;
//  it++;//to second
//  std::cout<<it.get_current()->value.second<<std::endl;
//  //==================================================================
//  
//  auto rit = lst.rbegin();
//  rit--;
//  rit++;
//  std::cout<<"RIT: "<<rit.get_current()->value.second<<std::endl;
//  
//  auto rit1 = lst.rend();
//  rit1++;
//  rit1--;
//  std::cout<<"RIT1: "<<rit1.get_current()->value.second << std::endl;
//  //=================================================================
//  auto eit = lst.end();// at 5
//  std::cout<<"previous node is: "<<eit.get_current()->value.second<<std::endl;
//  eit--;// at 4
//  lst.erase(3);
//  eit--;
//  std::cout<<"previous node w/t -- is: "<<eit.get_current()->value.second<<std::endl;
//  
//  // ideally it and rit should point to same object
//  std::cout<<((it.get_current()->value.first==rit.get_current()->value.first) ? true:false)<<std::endl;
//  std::cout<<"size is:"<<lst.size()<<"is empty->"<<lst.empty()<<std::endl;
//
//  //  lst.erase(2);
//  //  lst.erase(3);
//  //  lst.erase(4);
//  //  lst.erase(5);
//  //  cout<<"size is:"<<lst.size()<<"is empty->"<<lst.empty()<<endl;
//  // ==== Handel other types later...haha.. wait, are you serious or sirius. 
//  
//  std::cout<<"lets test it on other types"<<std::endl;
//  
//  Map<int, person> mp(3, 0.5);
//  person p1;
//  p1.name="yash";
//  mp.insert(std::make_pair(1, p1));
  
  // STANDARD MAP CONTAINER
  
  //=============== MAP ctor and INSERT working ======================
  
  //inserting 10 elements
  Map<int, int> mp;
  mp.insert(std::make_pair(2,4)); 
  mp.insert(std::make_pair(3,9)); 
  mp.insert(std::make_pair(4,16));
  mp.insert(std::make_pair(5,25));
  mp.insert(std::make_pair(6,36));
  mp.insert(std::make_pair(7,49));
  mp.insert(std::make_pair(8,64));
  mp.insert(std::make_pair(9,81));
  mp.insert(std::make_pair(10,100));
 
  //================ Checking size ===================================
  std::cout<<"Size is:"<<mp.size()<<std::endl;
 
  //======================= is EMPTY ======WORKING =============
  std::cout<<"is empty: "<<mp.empty()<<std::endl;// shoud give 0 as its false
  for(int i=2; i<11; i++){
      mp.erase(i);
  }
  std::cout<<"is empty: "<<mp.empty()<<std::endl;// shoud give 1 as its true
  // mp.displayList();// should be fine
  for(int i=2; i<11; i++){
      mp.insert(std::make_pair(i,i*i));
  }
  //mp.displayList();// should be filled...
  
  //==============ITERATOR begin() and end()====================================
  // iterator by refrence so can change the mapped type
  auto b = mp.begin();
  std::cout<<"begin itr : "<<b.get_current()->value.second<<std::endl;// 4
  b++;//3
  b++;//4
  b++;//5
  std::cout<<"beiin itr : "<<b.get_current()->value.second<<std::endl;// give 25
  
  auto l = mp.end();
  std::cout<<"end ite : "<<l.get_current()->value.second<<std::endl;// should be 100
  l--;//9
  l--;//8
  l--;//7
  std::cout<<"end ite : "<<l.get_current()->value.second<<std::endl;// should give 49
  
 //============== ConstIterator begin and end ===========================================
  
  Map<int,int>::ConstIterator cb = mp.begin();
  std::cout<<"const cb : "<<cb.get_current()->value.second<<std::endl;//a t 2 op 4
  cb++;//3
  cb++;//4
  cb--;//3
  std::cout<<"const cb : "<<cb.get_current()->value.second<<std::endl;
  
  //should not br able to change value
  cb.get_current()->value.second=27;// but it did
  //change it back
  cb.get_current()->value.second=9;
  
  Map<int,int>::ConstIterator rce = mp.end(); 
  std::cout<<"const rcb : "<<rce.get_current()->value.second<<std::endl;//a t 10 op 100
  rce--;//9
  rce--;//8
  rce++;//9
  std::cout<<"const rcb : "<<rce.get_current()->value.second<<std::endl;// should be 81
  //should not br able to change value
  rce.get_current()->value.second=9*9*9;// but it did
  //change it back
  rce.get_current()->value.second=81;
  
  //========================== ReverseIterator ======================================
  Map<int,int>::ReverseIterator rit = mp.rbegin();
  std::cout<<" rit : "<<rit.get_current()->value.second<<std::endl;//should be 100
  rit--;//9
  rit--;//8
  rit++;//9
  std::cout<<" rit : "<<rit.get_current()->value.second<<std::endl;// give 81
  
  Map<int,int>::ReverseIterator rite = mp.rend();
  std::cout<<" rite : "<<rite.get_current()->value.second<<std::endl;//should 2 op  4
  rite++;//3
  rite++;//4
  rite--;//3
  std::cout<<" rite : "<<rite.get_current()->value.second<<std::endl;// give 9
  
  //=========================== find(const key_T) =======================================
  auto fin = mp.find(5);
  std::cout<<"find():"<<fin.get_current()->value.second<<std::endl;
  
  // if it indeed is an iterator
  fin++;
  std::cout<<"find():"<<fin.get_current()->value.second<<std::endl;// should give 36
  
  // see if key not found then return end of iterator
  auto fintes = mp.find(16);
  std::cout<<"find() tes:"<<fintes.get_current()->value.second<<std::endl;// should give 100
  
  // ======================== find (const key_t) const ===================================
  auto constfind = mp.find(6);
  std::cout<<" const itr find() :"<<constfind.get_current()->value.second<<std::endl;
  
  constfind++;
  std::cout<<" const itr find() :"<<constfind.get_current()->value.second<<std::endl;
  constfind.get_current()->value.second=7*7*7;// shoudnt do
  constfind.get_current()->value.second=49;
  
  //====================== at() both function ===============================
  
  auto at1 = mp.at(4);
  auto atc = mp.at(5);
  std::cout<<"at1:"<<at1<<std::endl;
  at1 = 64;
  std::cout<<"at1:"<<at1<<std::endl;
  at1 = 16;
  std::cout<<"at1:"<<at1<<std::endl;
  
  // shoudnt happen
  std::cout<<"atc:"<<atc<<std::endl;
  atc=125;
  std::cout<<"atc:"<<atc<<std::endl;
  atc=25;
  std::cout<<"atc:"<<atc<<std::endl;
  //=============================erase both()=====================================
  mp.insert(std::make_pair(11,121));
  mp.insert(std::make_pair(12,144));
  mp.erase(11);
  mp.displayList();
  auto eraseit = mp.end();
  mp.erase(eraseit);
  mp.displayList();
  //mp.erase(12); working!!
  std::cout<<"erase it"<<eraseit.get_current()->value.second<<std::endl;
  
  //====================Clear function ..easy======
  mp.clear();
  mp.displayList();
} 
 
