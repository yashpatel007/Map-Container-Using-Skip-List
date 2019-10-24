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
  Node *previous; 
  Node(pair<key_T, val_T>,int);
  Node(int); 
}; 
template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(pair<key_T,val_T> p ,int level) 
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
{ this->value.first=NULL;
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
  Map<key_T, val_T>(int, float); 
  int randomLevel(); 
  Node<key_T, val_T>* createNode(pair <key_T, val_T>, int); 
  
  //pair<pair<key_T,val_T>&,bool>
   
  void erase(key_T); 
  
  val_T& at(const key_T); 
  //const val_T& at(const key_T); 
  void displayList();
  int size();
  bool empty(){return slsize==0;}
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
    Node<key_T, val_T>* get_current() const{return cur;}
    
    
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
  ConstIterator begin() const;
  ConstIterator end() const;
  ReverseIterator rbegin();
  ReverseIterator rend();
  
  //map function for element access 
  pair<Iterator,bool> insert(pair<key_T, val_T>);
  Iterator find(key_T);
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
typename Map<key_T, val_T>::ConstIterator Map<key_T, val_T>::begin() const{
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::ConstIterator(header->forward[0]);
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::ConstIterator Map<key_T, val_T>::end() const{
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::ConstIterator(tail->previous);
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
Node<key_T, val_T>* Map<key_T, val_T>::createNode(pair <key_T, val_T> p,int level) 
{ 
  //cout<<"node created with"<<p.first<<"and lev:"<<level<<endl;
  Node<key_T, val_T> *n = new Node<key_T, val_T>(p,level); 
  return n; 
}; 

// Insert given key in skip list 
template <typename key_T, typename val_T>
pair<typename Map<key_T,val_T>::Iterator,bool> Map<key_T, val_T>::insert(pair<key_T, val_T> p) 
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
   // 	}
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

    cout<< n->previous->value.first<<endl;
		
        if(n->forward[0] != NULL){
        cout<<"inif";
	n->forward[0]->previous = n;
	}
	else{
        cout<<"in else";
            tail->previous = n;
	} 
    
    cout << "Successfully Inserted key " << p.first<< "\n";
    this->slsize++; 
    Iterator retit = Iterator(n);
    return std::make_pair(retit, true);
  }else{
      Iterator retit = Iterator(current);
    cout<<"key already there"<<endl;
    return std::make_pair(retit, false);
  } 

}; 

template <typename key_T, typename val_T>
void Map<key_T, val_T>::erase(const key_T key) 
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
        
         
         cout<<"Successfully deleted key "<<key<<"\n";
         this->slsize--; 
    } 
}; 
  
// Search for element in skip list 
template <typename key_T, typename val_T>
typename Map<key_T, val_T>::Iterator Map<key_T, val_T>::find(key_T key) 
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
        cout<<"Found key: "<<key<<"\n";
        return Iterator(current);
    }
    else{
      cout<<"KEY not found"<<endl;
      return Iterator(tail->previous);
    } 
};

template <typename key_T, typename val_T>
val_T& Map<key_T, val_T>::at(const key_T key) 
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
      cout<<"Key found: ";  
    return current->value.second;
    }
    else{
      cout<<"KEY not found"<<endl;
      throw  std::out_of_range("key not found");
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
  cout<<"********************"<<endl;
}; 

template <typename key_T, typename val_T>
int Map<key_T, val_T>::size() 
{ cout<<"Size is: "<<this->slsize<<endl;
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
  // create SkipList object with MAXLVL and P 
  Map<int, int>lst(3, 0.5); 
  lst.displayList();
  
  // Ok lets text everything that is implemented ........RUM RUM ...
  lst.insert(std::make_pair(2,22)); 
  lst.insert(std::make_pair(3,33)); 
  lst.insert(std::make_pair(4,44));
  auto k = lst.insert(std::make_pair(5,55));
  //lst.displayList();-> list is empty
  //SIZE function
  cout<<"Size is: "<<lst.size()<<endl;  
  //k should be an iterator and bool pair
  auto e =k.first;
  cout<<"e from k is :"<<e.get_current()->value.second<<endl;
  e--;// check if k is actually an iterator
  cout<<"e from k  is :"<<e.get_current()->value.second<<endl;
  //check if iterator is padded by refrence
  e.get_current()->value.second=444;
  cout<<"e from k  pbr check is :"<<e.get_current()->value.second<<endl;
  e.get_current()->value.second=44;//lets just change it back.
 
  // testing at function ->  mapped_T at(key)
   cout<<"element at key 2 is :"<<lst.at(2)<<endl;
  // checking the ret value of at is by refrence or not 
  lst.at(2)=222;
  cout<<"element at pbr key 2 is :"<<lst.at(2)<<endl;
  lst.at(2)=22;
  
  
  // ===============================try erasing the value 
  lst.erase(5);
  lst.displayList();
  lst.insert(std::make_pair(5,55));
  lst.displayList();
//  // lets see the issue// just in case you are thinking JCI -> just checking iterator
  auto jci = lst.end();
  cout<<"JCI:"<<jci.get_current()->value.first<<endl;
  jci--;
  cout<<"JCI:"<<jci.get_current()->value.first<<endl;
  //====================CONST ITERATOR==================
  Map<int,int>::ConstIterator ci =lst.begin();  
  cout<<"CI:"<<ci.get_current()->value.first<<endl;
  ci++;
  cout<<"CI:"<<ci.get_current()->value.first<<endl;
  ci--;
  cout<<"CI:"<<ci.get_current()->value.first<<endl;
  //this should fail ----- is happening---need to see ******
  ci.get_current()->value.second=222;
  cout<<"CI: changing second "<<ci.get_current()->value.second<<endl;
  //=================================================================
  auto it = lst.begin();
  cout<<it.get_current()->value.second<<endl;
  it++;//to second
  cout<<it.get_current()->value.second<<endl;
  //=================================================================
  
  auto rit = lst.rbegin();
  rit--;
  rit++;
  cout<<"RIT: "<<rit.get_current()->value.second<<endl;
  //=================================================================
  auto eit = lst.end();// at 5
  cout<<"previous node is: "<<eit.get_current()->value.second<<endl;
  eit--;// at 4
  lst.erase(3);
  eit--;
  cout<<"previous node w/t -- is: "<<eit.get_current()->value.second<<endl;
  
  // ideally it and rit should point to same object
  cout<<((it.get_current()->value.first==rit.get_current()->value.first) ? true:false)<<endl;
  cout<<"size is:"<<lst.size()<<"is empty->"<<lst.empty()<<endl;

  //  lst.erase(2);
  //  lst.erase(3);
  //  lst.erase(4);
  //  lst.erase(5);
  //  cout<<"size is:"<<lst.size()<<"is empty->"<<lst.empty()<<endl;
  // ==== Handel other types later...haha.. wait, are you serious or sirius. 
  
  cout<<"lets test it on other types"<<endl;
  
  Map<int, person> mp(3, 0.5);
  person p1;
  p1.name="yash";
  mp.insert(std::make_pair(1, p1));
} 

