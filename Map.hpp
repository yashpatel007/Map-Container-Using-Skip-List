// C++ code for inserting element in skip list 
// proff code here =================================
#include <bits/stdc++.h> 
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <utility>

//=========================================================================================================

/*
 * Wrapper class around std::map to handle slight difference in return value and also
 * provide an Iterator nested name.
 */

template <typename K, typename V>
class test_map : public std::map<K, V> {
    private:
        using base_t = std::map<K, V>;
    public:
        using Iterator = typename base_t::iterator;
        std::pair<typename base_t::iterator, bool>insert(const std::pair<const K, V> &p) {
            return this->base_t::insert(p);
        }
};

/*
 * Person class.
 */

struct Person {
    friend bool operator<(const Person &p1, const Person &p2) {
        return p1.name < p2.name;
    }
    friend bool operator==(const Person &p1, const Person &p2) {
        return p1.name == p2.name;
    }
    Person(const char *n) : name(n) {}
    void print() const {
        printf("Name: %s\n", name.c_str());
    }
    const std::string name;
    Person &operator=(const Person &) = delete;
};

void
print(const std::pair<const Person, int> &p) {
    p.first.print();
    printf("    %d\n", p.second);
}

/*
 * MyClass class.
 */

struct MyClass {
    friend bool operator<(const MyClass &o1, const MyClass &o2) {
        return o1.num < o2.num;
    }
    friend bool operator==(const MyClass &o1, const MyClass &o2) {
        return o1.num == o2.num;
    }
    //MyClass(double n) : num(n) {}
    double num;
};

void
print(const std::pair<const int, std::string> &p) {
    printf("%d, %s; ", p.first, p.second.c_str());
}

/*
 * Stress class.
 */

struct Stress {
    friend bool operator<(const Stress& o1, const Stress& o2) {
        return o1.val < o2.val;
    }
    friend bool operator==(const Stress& o1, const Stress& o2) {
        return o1.val == o2.val;
    }
    Stress(int _v) : val(_v){}
    int val;
};
// Helper function for stress testing. This orders iterators by what they point to.
template <template <typename, typename> class MAP_T>
inline bool
less(const typename MAP_T<const Stress, double>::Iterator &lhs, const typename MAP_T<const Stress, double>::Iterator &rhs) {
    return (*lhs).first.val < (*rhs).first.val;
}

/*
 * Additional test functions for BST.
 */

template <template <typename, typename> class MAP_T>
void traverse(const MAP_T<const Person, int> &, int level);

template <template <typename, typename> class MAP_T>
void traverse2(int level);

template <template <typename, typename> class MAP_T>
void check(const MAP_T<const Stress, double> &, const std::map<const Stress, double> &);


//========================================= MYIMPLEMENTATION  =============================================
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
  bool operator==(const Node<key_T,val_T>& n1){
    return (this == n1)? true : false;
  }
  bool operator!=(const Node<key_T,val_T>& n1){
    return (this != n1)? true : false;
  }
  ~Node(){
  delete [] forward;
  //delete value;
  delete previous;
  }
  
}; 
template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(std::pair<key_T,val_T> p ,int level) 
{ 
  this->value = p;
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
  //this is for making centinal nodes
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
  
//  ~Map(){
//        Node<key_T, val_T> *tem_header = header;
//        Node<key_T, val_T> *temp;
//	while(tem_header != NULL){
//	temp = tem_header->forward[0];
//	//delete &tem_header;
//	tem_header = temp;
//	}
//  }
      
  Map<key_T, val_T>();
  Map<key_T, val_T>(int, float); 
  int randomLevel(); 
  Node<key_T, val_T>* createNode(std::pair <key_T, val_T>, int); 
  
  //
  typedef std::pair<key_T, val_T> ValueType;
 
   
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
    
    ValueType& operator*() const{
        return cur->value;
    }
    ValueType* operator->() const{
	return cur->value;
    }
    
    bool operator==(const typename Map<key_T,val_T>::Iterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
    bool operator!=(const Iterator& itr1){
    return (this->get_current() != itr1.get_current())? true : false;
    }
    
    bool operator==(const typename Map<key_T,val_T>::ReverseIterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
   
    
    bool operator==(const typename Map<key_T,val_T>::ConstIterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
//   bool operator!=(const Map<key_T,val_T>::ConstIterator& itr1){
//    return (this->get_current() != itr1.get_current())? true : false;
//    }
    
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
    ValueType& operator*() const{
	return cur->value;
    }
    ValueType* operator->() const{
    return cur->value;
    }
    
    
    
    bool operator==(const typename Map<key_T,val_T>::ConstIterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
    bool operator!=(const ConstIterator& itr1){
    return (this->get_current() != itr1.get_current())? true : false;
    }
    bool operator==(const typename Map<key_T,val_T>::Iterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
    bool operator!=(const Iterator& itr1){
    return (this->get_current() != itr1.get_current())? true : false;
    }
    
    bool operator==(const typename Map<key_T,val_T>::ReverseIterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
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
    
  //defining operators for ReverseIterator
    ReverseIterator& operator=(const ReverseIterator& ritr){
            cur = ritr.get_current();
            return *this;
    }
    ReverseIterator& operator++(){
            if(cur == NULL) return *this;
            cur = cur->previous;
            return *this;
    }
    ReverseIterator operator++(int){
             Map<key_T, val_T>::ReverseIterator ret = *this;
            if(cur == NULL) return ret;
            cur = cur->previous;
            return ret;
    }
    ReverseIterator& operator--(){
            if(cur == NULL) return *this;
            cur = cur->forward[0];
            return *this;
    }
    ReverseIterator operator--(int){
            Map<key_T, val_T>::ReverseIterator ret = *this;
            if(cur == NULL) return ret;
            cur = cur->forward[0];
            return ret;
    }
    ValueType& operator*() const{
	return *cur->value;
    }
    ValueType* operator->() const{
    return cur->value;
    }
    
    bool operator==(const typename Map<key_T,val_T>::ReverseIterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
    bool operator!=(const ReverseIterator& itr1){
    return (this->get_current() != itr1.get_current())? true : false;
    }
     bool operator==(const typename Map<key_T,val_T>::ConstIterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
    bool operator!=(const ConstIterator& itr1){
    return (this->get_current() != itr1.get_current())? true : false;
    }
    bool operator==(const typename Map<key_T,val_T>::Iterator& itr1){
    return (this->get_current() == itr1.get_current())? true : false;
    }
    bool operator!=(const Iterator& itr1){
    return (this->get_current() != itr1.get_current())? true : false;
    }
    
    ~ReverseIterator(){
        // setting cirrent as zero .....cus it is what it is dude..:)
        cur = 0; 
    }
    private:
    Node<key_T,val_T> *cur;
    
  };
  
    //
    //map coparison function
bool operator==(Map<key_T, val_T>& map1){
    std::cout<<this->size()<<map1.size()<<"\n";
    if(this->size() != map1.size()) return false;
	auto itr1 = this->begin();
        auto itr2 = map1.begin();
	while(itr1 != this->end() && itr2 != map1.end()){
            std::cout<<itr1.get_current()->value.first<<itr2.get_current()->value.first<<"\n";
            if(*itr1 != *itr2){
                std::cout<<" i am here in if in map containor\n";
                return false;
            }
            ++itr1;
            ++itr2;
            }
            return true;
}

bool operator!=(const Map<key_T, val_T>& map1){
	return !(this == map1);
}

bool operator<(const Map<key_T, val_T>& map2){
		size_t size1 = this->size();
		size_t size2 = map2.size();
		if(size1 < size2) return true;
		if(size2 < size1) return false;
		//same size
		auto itr1 = this->begin();
		auto itr2 = map2.begin();
		while(itr1 != this->end() && itr2 != map2.end()){
			bool less = ((*itr1) < (*itr2));
			bool less2 = ((*itr2) < (*itr1));
			if(less) return true;
			if(less2) return false;
			++itr1;
			++itr2;
		}
		//maps are same
		return false;
    }
  
  // iterator functions
  Iterator begin();
  Iterator end();
  ConstIterator begin() const{return ConstIterator(header->forward[0]);}
  ConstIterator end() const{return Map<key_T, val_T>::ConstIterator(tail);}
  ReverseIterator rbegin();
  ReverseIterator rend();
  
  //map function for element access 
  std::pair<Iterator,bool> insert(const std::pair<key_T, val_T>);
  
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
      return ConstIterator(tail);
    } 
  };// find ends


//iterator comparision functions
//comparison operators on iterator
};

template <typename key_T, typename val_T>
Map<key_T, val_T>::Map() 
{ //create an empty container
  this->MAXLVL = 100; 
  this->P = 0.5; 
  level = 0; 
  //key_T k;// null init key in header
  //val_T v;//no init value in header
  //size --added
  slsize=0;
  // create header node  
  header = new Node<key_T, val_T>(MAXLVL); 
  // init tail node
  tail = new Node<key_T, val_T>(MAXLVL);
  
  //tail prev is header
  header->forward[0]=tail;
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
      return Map<key_T, val_T>::Iterator(tail);
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::ReverseIterator Map<key_T, val_T>::rbegin(){
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::ReverseIterator(tail->previous);
}

template <typename key_T, typename val_T>
typename Map<key_T, val_T>::ReverseIterator Map<key_T, val_T>::rend(){
      //return this->header->forward[0]->value;
      return Map<key_T, val_T>::ReverseIterator(header);
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
std::pair<typename Map<key_T,val_T>::Iterator,bool> Map<key_T, val_T>::insert(const std::pair<key_T, val_T> p) 
{  
  
  // pair<Iterator,bool>
  // make temp uiterator as Iterator temp = Iterator(n<-newly created node); and return the pointer
  Node<key_T, val_T> *current = header; 

  // create update array and initialize it 
  Node<key_T, val_T> *update[MAXLVL+1]; 
  
  memset(update, 0, sizeof(Node<key_T, val_T>*)*(MAXLVL+1)); 
  
  for (int i = level; i >= 1; i--) 
  { 
    while (current->forward[i] != NULL && 
      current->forward[i]->value.first < p.first) {
      current = current->forward[i]; }
    //fill update 
      update[i] = current; 
  }

  //========JUST EXPERIMENTING ,AYE================================
      while(current->forward[0] != tail && current->forward[0]->value.first < p.first){
     		current = current->forward[0];
    	}
  //=============================================================== 
  
   
  
    update[0] = current;
    Node<key_T, val_T> *n = update[0];
    n = n->forward[0];
  current = current->forward[0];
  if (current == NULL || !(current->value.first == p.first)) 
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
    n=NULL;
    n = createNode(p,rlevel); 

    // insert node by rearranging pointers 
    for(int i=0;i<=rlevel;i++) 
    { 
      n->forward[i] = update[i]->forward[i]; 
      update[i]->forward[i] = n; 
    }

    
    n->previous = update[0];

    //std::cout<< n->previous->value.first<<std::endl;
		
        if(!(n->forward[0] == tail)){
            std::cout<<"inif";
	n->forward[0]->previous = n;
	}
	else{
            tail->previous = n;
	} 
    
    std::cout << "Successfully Inserted key\n";
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
 
    for(int i = level; i >= 1; i--) 
    { 
        while(current->forward[i] != NULL  && 
              current->forward[i]->value.first < key){ 
            current = current->forward[i]; }
        update[i] = current; 
    } 
    
    while(current->forward[0] != tail && current->forward[0]->value.first < key){
	current = current->forward[0];
    }
    
    
        update[0] = current;
        Node<key_T, val_T> *n = update[0];
        n = n->forward[0];
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
         if(!(current->forward[0] == tail)){
            current->forward[0]->previous = current->previous;
        }
        else{
            tail->previous = current->previous;
            
            current->previous->forward[0] = tail;
            
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
 
    for(int i = level; i >= 1; i--) 
    { 
        while(current->forward[i] != NULL  && 
              current->forward[i]->value.first < key){ 
            current = current->forward[i]; }
        update[i] = current; 
    } 
    
    while(current->forward[0] != tail && current->forward[0]->value.first < key){
	current = current->forward[0];
    }
    
    
        update[0] = current;
        Node<key_T, val_T> *n = update[0];
        n = n->forward[0];
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
         if(!(current->forward[0] == tail)){
            current->forward[0]->previous = current->previous;
        }
        else{
            tail->previous = current->previous;
            
            current->previous->forward[0] = tail;
            
            }

        //======================================
  
   
     // Remove levels having no elements  
        while(level>0 && 
              header->forward[level] == 0) 
            level--; 
         std::cout<<"Successfully deleted key \n";
         this->slsize--; 
    }else{
        throw std::out_of_range("Key Not in Map");
    } 
};

// Search for element in skip list 
template <typename key_T, typename val_T>
typename Map<key_T, val_T>::Iterator Map<key_T, val_T>::find(const key_T key)
{ 
    Node<key_T, val_T> *current = header; 
  
    for(int i = level; i >= 0; i--) 
    { 
        while(current->forward[i] && 
               current->forward[i]->value.first < key) 
            current = current->forward[i]; 
    }   
   current = current->forward[0]; 
  
    // If current node have key equal to 
    // search key, we have found our target node 
    if(current and current->value.first == key){  
        std::cout<<"Found key: "<<key<<"\n";
        return Iterator(current);
    }
    else{
      std::cout<<"KEY not found"<<std::endl;
      return Iterator(tail);
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
    while (!(node == NULL)) 
    { 
      std::cout << "key" <<node->value.first;
        //print(node->value);
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

struct myname{
    friend bool operator<(const myname &p1, const myname &p2) {
        return p1.name < p2.name;
    }
    friend bool operator==(const myname &p1, const myname &p2) {
        return p1.name == p2.name;
    }
    void print() const {
        printf("Name: %s\n", name.c_str());
    }
    //myname(const char *n) : name(n) {}
    std::string name;
    //myname &operator=(const myname &) = delete;
    
};
void
print(const std::pair<myname, int> &p) {
    p.first.print();
    printf("    %d\n", p.second);
}


// Driver to test above code 
int main() 
{ 
  // Seed random number generator 
  srand((unsigned)time(0)); 
  
  
  //============================PROFF CODE=======================================
//        
//        myname p1={"Jane"};
//        myname p2={"John"};
//        myname p3={"Mary"};
//        myname p4={"Dave"};
//
//        Map<myname, int> map;
//
//        // Insert people into the map.
//        auto p1_it = map.insert(std::make_pair(p1, 1));
//        map.insert(std::make_pair(p2, 2));
//        map.insert(std::make_pair(p3, 3));
//        map.insert(std::make_pair(p4, 4));
//
//        // Check iterator equality.
//        {
//            // Returns an iterator pointing to the first element.
//            auto it1 = map.begin();
//            // Returns an iterator pointing to one PAST the last element.  This
//            // iterator is obviously conceptual only.  It cannot be
//            // dereferenced.
//            auto it2 = map.end();
//
//            it1++; // Second node now.
//            it1++; // Third node now.
//            it2--; // Fourth node now.
//            it2--; // Third node now.
//            assert(it1 == it2);
//            it2--; // Second node now.
//            it2--; // First node now.
//            assert(map.begin() == it2);
//        }
//         // Check insert return value.
//        {
//            printf("---- Test insert() return.\n");
//            // Insert returns an interator.  If it's already in, it returns an
//            // iterator to the already inserted element.
//            auto it = map.insert(std::make_pair(p1, 1));
//            assert(it.first == p1_it.first);
//            // Now insert one that is new.
//            myname p5={"Larry"};
//            it = map.insert(std::make_pair(p5, 5));
//            print(*(it.first));
//            map.erase(it.first);
//        }
//        
//        // Print the whole thing now, to verify ordering.
//        printf("---- Before erasures.\n");
//
//        // Iterate through the whole map, and call print() on each Person.
//        for (auto &e : map) {
//            print(e);
//        }
//
//        // Test multiple traversals of the same map.
//        //printf("---- Multiple traversals\n");
//        //traverse(map, 4);
//
////        // Test multiple BST at the same time.
////        printf("---- Multiple BST\n");
////        traverse2<MAP_T>(4);
//
//        /*
//         * Test some erasures.
//         */
//
//        // Erase first element.
//        map.erase(map.begin());
//        auto it = map.end();
//        --it; // it now points to last element.
//        it--; // it now points to penultimate element.
//        map.erase(it);
//
//        printf("---- After erasures.\n");
//
//        // Iterate through the whole map, and call print() on each Person.
//        for (auto &e : map) {
//            print(e);
//        }
//
//        // Test iterator validity.
//        {
//            // Iterators must be valid even when other things are inserted or
//            // erased.
//            printf("---- Test iterator non-invalidation\n");
//
//            // Get iterator to the first.
//            auto b = map.begin();
//
//            // Insert element which will be at the end.
//            myname p6 ={"Zeke"};
//            auto it = map.insert(std::make_pair(p6, 10));
//
//            // Iterator to the first should still be valid.
//            print(*b);
//
//            // Delete first, saving the actual object.
//            auto tmp(*b); // Save, so we can reinsert.
//            map.erase(map.begin()); // Erase it.
//
//            // Check iterator for inserted.  Iterator to end should still be valid.
//            print(*it.first); // This should still be valid.
//
//            // Reinsert first element.
//            map.insert(tmp);
//
//            // Erase inserted last element.
//            map.erase(it.first);
//        }
//        
//        
//         /*
//     * Test Map with MyClass.
//     */
//
//    {
//        Map<MyClass, std::string> map;
//
//        // Empty container, should print nothing.
//        for (auto it = map.begin(); it != map.end(); ++it) {
//            abort();
//        }
//
//        MyClass m1, m2, m3, m4;
//        m1={4};
//        m2={3};
//        m3={1};
//        m4={2};
//        auto m1_it = map.insert(std::make_pair(m1, "mmm1"));
//        map.insert(std::make_pair(m2, "mmm2"));
//        map.insert(std::make_pair(m3, "mmm3"));
//        map.insert(std::make_pair(m4, "mmm4"));
//
//        // Should print 0.0 1.0 2.0 3.0
//        for (auto &e : map) {
//            printf("%3.1f ", e.first.num);
//        }
//        printf("\n");
//    
//    
//    }
//        
//        
//        
//
//  
//  
  //================================================================================
  
  
//  //=================================check mapn < ==================================================
//        Map<int,int>m1;
//        Map<int,int>m2;
//  
//        for(int i=1; i<5;i++){
//            m1.insert({i,i*i*i});
//            m2.insert({i,i*i*i});
//        }
//        //maps are equal
//        if(m1<m2){
//            std::cout<<"m1<m2";
//        }else{
//            std::cout<<"false";
//        }
//        m1.insert({5,125});
//        m2.insert({5,1000});
//        if(m1<m2){
//            std::cout<<"m2<m1";
//        }else{
//            std::cout<<"false";
//        }
  
//==============================================HAPPINESS BEGINS=======================================
//  //inserting 10 elements
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
//  
//  
//
//  
  mp.erase(10);
  mp.displayList();
  mp.insert(std::make_pair(10,100));
  mp.displayList();
//  //================ Checking size ===================================
//  std::cout<<"Size is:"<<mp.size()<<std::endl;
//// 
////  //======================= is EMPTY ======WORKING =============
//  std::cout<<"is empty: "<<mp.empty()<<std::endl;// shoud give 0 as its false
//  for(int i=2; i<11; i++){
//      std::cout<<i;
//      mp.erase(i);
//  }
//  std::cout<<"is empty: "<<mp.empty()<<std::endl;// shoud give 1 as its true
//  // mp.displayList();// should be fine
//  for(int i=2; i<11; i++){
//      mp.insert(std::make_pair(i,i*i));
//  }
//  mp.displayList();// should be filled...
//  
////  //==============ITERATOR begin() and end()====================================
//  // iterator by refrence so can change the mapped type
//  auto b = mp.begin();
//  std::cout<<"begin itr : "<<b.get_current()->value.second<<std::endl;// 4
//  b++;//3
//  b++;//4
//  b++;//5
//  std::cout<<"beiin itr : "<<b.get_current()->value.second<<std::endl;// give 25
//  
//  auto l = mp.end();
//  std::cout<<"end ite : "<<l.get_current()->value.second<<std::endl;// should be 0
//  l--;//10
//  l--;//9
//  l--;//8
//  std::cout<<"end ite : "<<l.get_current()->value.second<<std::endl;// should give 64
//  
//// //============== ConstIterator begin and end ===========================================
////  
//  Map<int,int>::ConstIterator cb = mp.begin();
//  std::cout<<"const cb : "<<cb.get_current()->value.second<<std::endl;//a t 2 op 4
//  cb++;//3
//  cb++;//4
//  cb--;//3
//  std::cout<<"const cb : "<<cb.get_current()->value.second<<std::endl;//9
//  
//  //should not br able to change value
//  cb.get_current()->value.second=27;// but it did
//  //change it back
//  cb.get_current()->value.second=9;
//  
//  Map<int,int>::ConstIterator rce = mp.end(); 
//  std::cout<<"const rcb : "<<rce.get_current()->value.second<<std::endl;//a t 0 op 0
//  rce--;//10
//  std::cout<<"const rcb : "<<rce.get_current()->value.second<<std::endl;//a t 0 op 0
//  rce--;//9
//  rce++;//10
//  std::cout<<"const rcb : "<<rce.get_current()->value.second<<std::endl;// should be 100
//  //should not br able to change value
//  rce.get_current()->value.second=10*10*10;// but it did
//  //change it back
//  rce.get_current()->value.second=100;
//  
////  //========================== ReverseIterator ======================================
//  mp.displayList();
//  Map<int,int>::ReverseIterator rit = mp.rbegin();
//  std::cout<<" rit : "<<rit.get_current()->value.second<<std::endl;//should be 100
//  rit++;//9
//  rit++;//8
//  rit--;//9
//  std::cout<<" rit : "<<rit.get_current()->value.second<<std::endl;// give 81
//  
//  Map<int,int>::ReverseIterator rite = mp.rend();
//  std::cout<<" rite : "<<rite.get_current()->value.second<<std::endl;//should 0 op  0
//  rite--;//2
//  rite--;//3
//  rite++;//2
//  std::cout<<" rite : "<<rite.get_current()->value.second<<std::endl;// give 4
//  
////  //=========================== find(const key_T) =======================================
//  auto fin = mp.find(5);
//  std::cout<<"find():"<<fin.get_current()->value.second<<std::endl;
//  
//  // if it indeed is an iterator
//  fin++;
//  std::cout<<"find():"<<fin.get_current()->value.second<<std::endl;// should give 36
//  
//  // see if key not found then return end of iterator
//  auto fintes = mp.find(16);
//  std::cout<<"find() tes:"<<fintes.get_current()->value.second<<std::endl;// should give 100
//  
////  // ======================== find (const key_t) const ===================================
//  auto constfind = mp.find(6);
//  std::cout<<" const itr find() :"<<constfind.get_current()->value.second<<std::endl;//36
//  
//  constfind++;
//  std::cout<<" const itr find() :"<<constfind.get_current()->value.second<<std::endl;//49
//  constfind.get_current()->value.second=7*7*7;// shoudnt do
//  constfind.get_current()->value.second=49;
//  
//  //====================== at() both function ===============================  
//  auto at1 = mp.at(4);
//  auto atc = mp.at(5);
//  std::cout<<"at1:"<<at1<<std::endl;
//  at1 = 64;
//  std::cout<<"at1:"<<at1<<std::endl;
//  at1 = 16;
//  std::cout<<"at1:"<<at1<<std::endl;
//  
//  // shoudnt happen
//  std::cout<<"atc:"<<atc<<std::endl;
//  atc=125;
//  std::cout<<"atc:"<<atc<<std::endl;
//  atc=25;
//  std::cout<<"atc:"<<atc<<std::endl;
//  
////  //=============================erase both()=====================================
//  mp.insert(std::make_pair(11,121));
//  mp.insert(std::make_pair(12,144));
//  mp.erase(11);
//  mp.displayList();
//  auto eraseit = mp.end();
//  eraseit--;
//  mp.erase(eraseit);
//  mp.displayList();
//  //mp.erase(12); working!!
//  std::cout<<"erase it"<<eraseit.get_current()->value.second<<std::endl;
//  
////  //====================Clear function ..easy=*************Not working
////  //mp.clear();
//// 
////  //======================Not Working ================================
////  
////  //====================== iterators ========================
//  auto cmpitr1=mp.begin();//2
//  auto cmpitr2 = mp.begin();//2
//  auto cmpitr3 = mp.end();//0
//  cmpitr3--;//10
//  
//  auto constcmpitr1= mp.begin();//2
//  auto constcmpitr2 = mp.begin();//2
//  auto constcmpitr3 = mp.end();//0
//  constcmpitr3--;//10
//  
//  auto revcmpitr1=mp.rbegin();//10
//  auto revcmpitr2 = mp.rbegin();//10
//  auto revcmpitr3 = mp.rend();//0
//  revcmpitr3--;//2
//  //itr ==itr
//  if(cmpitr1==cmpitr2){
//      std::cout<<"yes1\n";
//  }
//  //itr!=itr
//  if(cmpitr1!=cmpitr3){
//      std::cout<<"yes2\n";
//  }
//  //itr=constitr
//  if(cmpitr1==constcmpitr2){
//      std::cout<<"yes3\n";
//  }
//  // itr!=constitr
//  if(cmpitr1!=constcmpitr3){
//      std::cout<<"yes4\n";
//  }
//  //const itr
//  if(constcmpitr1==constcmpitr2){
//      std::cout<<"yes5\n";
//  }
//  if(constcmpitr1!=constcmpitr3){
//      std::cout<<"yes6\n";
//  }
//  
//  //reverse iterator
//  if(revcmpitr1==revcmpitr2){
//      std::cout<<"yes7\n";
//  }
//  if(revcmpitr1!=revcmpitr3){
//      std::cout<<"yes8\n";
//  }
//  
////  //=======================MAp compare ===========================================
//  Map<int,int> mymap1;
//  Map<int,int> mymap2;
//  
//  for(int i=1; i<11; i++){
//      mymap1.insert(std::make_pair(i,i*i));
//      mymap2.insert(std::make_pair(i,i*i));
//  }
//  
//  //mymap2.insert(std::make_pair(11,121));
//  
//  auto jc1=mymap1.begin();
//  auto jc2 = mymap2.begin();
//  jc2++;
//  if(jc1.get_current()->value<jc2.get_current()->value){
//      std::cout<<"seems to work";
//  }
//  
//  if(mymap2==mymap1){
//      std::cout<<"MAps are equal\n";
//  }else{
//      std::cout<<"Maps not equal";
//  }
//  
//  // ===========================ITERATE THROUGH MAP=============================
//  for(auto i = mp.begin(); i!=mp.end();i++){
//      std::cout<<i.get_current()->value.first<<"\n";
//  }
//  for(auto &e:mp){
//      std::cout<<e.first;
//  }
//  ///======================================END OF HAPPINESS=======================================================
  
  
  
  
   
}
 