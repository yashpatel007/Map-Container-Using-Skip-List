#include<iostream>
#include<utility>
#include<string.h>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<stdexcept>

//========================================= MYIMPLEMENTATION  =============================================
 namespace cs540{
// Class to implement node 
template <typename key_T, typename val_T>
class Node
{ 
public:   
    //friend class Map<key_T,val_T>;
    typedef std::pair<const key_T, val_T> ValueType; 
    ValueType *value ;
  // Array to hold pointers to node of different level 
  Node **forward;
  Node *previous; 
  Node(const ValueType&,int);
  Node(int);
//  bool operator==(const Node<key_T,val_T>& n1){
//    return (this == n1)? true : false;
//  }
//  bool operator!=(const Node<key_T,val_T>& n1){
//    return (this != n1)? true : false;
//  }
  ~Node(){
  delete [] forward;
  delete value;
  //delete previous;// i guess its deleating twice!!
  }
  
}; 
template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(const ValueType& p ,int level) 
{ 
  this->value = new std::pair<const key_T,val_T>(p);
  // Allocate memory to forward 
  forward = new Node<key_T, val_T>*[level+1]; 
  // Fill forward array with 0(NULL) 
  memset(forward, 0, sizeof(Node<key_T, val_T>*)*(level+1));
  //pointer to previous node
  previous = 0; 
}

template <typename key_T, typename val_T>
Node<key_T, val_T>::Node(int level) 
{ 
  //this is for making centinal nodes
  // Allocate memory to forward 
  forward = new Node<key_T, val_T>*[level+1]; 
  // Fill forward array with 0(NULL) 
  memset(forward, 0, sizeof(Node<key_T, val_T>*)*(level+1));
  //pointer to previous node  
  this->value = NULL;
  previous = NULL; 
} 

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
      Node<key_T, val_T>* get_header() const{
          return header;
      }
      Node<key_T, val_T>* get_tail() const {
      return tail;}

  //map ctors    
  Map<key_T, val_T>();
  Map<key_T, val_T>(int, float);
  Map(const Map& map){
  //ctor code   
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
  header->previous = NULL; 
  tail->forward[0] =NULL;

      Node<key_T, val_T> *temp = map.get_header()->forward[0];
      while(temp != map.get_tail()){
            ValueType& val = *(temp->value);
            insert(val);
            temp = temp->forward[0];
      }
  }
  
  Map(std::initializer_list<std::pair<const key_T, val_T>>);
  
  // map destructor
 ~Map(){
    Node<key_T, val_T> *tem_header = header;
    Node<key_T, val_T> *temp;
    while(tem_header != NULL){
    temp = tem_header->forward[0];
    delete tem_header;
    tem_header = temp;
    }
 }  
  
  
  
  int randomLevel(); 
  //Node<key_T, val_T>* createNode(const std::pair <key_T, val_T>, int); 
  
  //
  typedef std::pair<const key_T, val_T> ValueType;
 
   
  //Just an extra function 
  void displayList();
  
  std::size_t size() const ;
  bool empty() const{return slsize==0;}
  
  // forward decl of classes
  class ConstIterator;
  class ReverseIterator;

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
      return *cur->value;
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
    return *cur->value;
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
  

//map coparison functions
bool operator==(Map<key_T, val_T>& map1){
    std::cout<<this->size()<<map1.size()<<"\n";
    if(this->size() != map1.size()) return false;
  auto itr1 = this->begin();
        auto itr2 = map1.begin();
  while(itr1 != this->end() && itr2 != map1.end()){
            std::cout<<itr1.get_current()->value->first<<itr2.get_current()->value->first<<"\n";
            if(*itr1 != *itr2){
                //std::cout<<" i am here in if in map containor\n";
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

val_T &operator[](const key_T & key){
    Node<key_T, val_T> *temp = lookupkey(key);
    if(temp== NULL){
    Map<key_T, val_T>::Iterator itr = insert(std::make_pair(key, val_T())).first;
    return itr.get_current()->value->second;
    }
    return temp->value->second;
};
  
Map &operator=(const Map<key_T,val_T> & map){;  
    if(this == &map){
        return *this;
    }
    Node<key_T, val_T> *ret = header;
    Node<key_T, val_T> *temp;
    while(ret!= NULL){
    temp = ret->forward[0];
    delete ret;//****************************************************************
    ret = NULL;
    ret = temp;
  }
//=====================================CTOR==========================
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
  header->previous = NULL; 
  tail->forward[0] =NULL;
  //===============================================================
    Node<key_T, val_T> *first = map.get_header()->forward[0];
    if(first == map.get_tail()) return *this;
    while(first != map.get_tail()){
      insert(*(first->value));
      first = first->forward[0];
    }
    return *this;
}
  // iterator functions
  Iterator begin();
  Iterator end();
  ConstIterator begin() const{return ConstIterator(header->forward[0]);}
  ConstIterator end() const{return Map<key_T, val_T>::ConstIterator(tail);}
  ReverseIterator rbegin();
  ReverseIterator rend();

//
  
  
  //map function for element access 
  std::pair<Iterator,bool> insert(const ValueType&);
  template <typename IT_T>
  void insert(IT_T range_beg, IT_T range_end);

  //at function
  val_T& at(const key_T&);
  const val_T& at(const key_T&)const;
  
  //erase
  void erase(const key_T&); 
  void erase(Iterator);
  
  //clear
  void clear();

  //find
  Iterator find(const key_T&);
  ConstIterator find(const key_T& key) const{
    Node<key_T, val_T> *temp = lookupkey(key);
    if(temp == NULL){
    return Map<key_T, val_T>::ConstIterator(tail);
    }
    return Map<key_T, val_T>::ConstIterator(temp);
  };// find ends

  Node<key_T, val_T>* lookupkey(const key_T& key) const{
      
    Node<key_T, val_T> *current = header; 
  
    for(int i = level; i >= 1; i--) 
    { 
        while(current->forward[i] !=NULL && 
               current->forward[i]->value->first < key) 
            current = current->forward[i]; 
    }   
    
    while(current->forward[0] != tail && current->forward[0]->value->first < key){
      current = current->forward[0];
    }
    
    
   current = current->forward[0]; 
  
    // If current node have key equal to 
    // search key, we have found our target node
   if(current==tail){ return NULL;}
    if(current !=NULL){
        if( current->value->first == key){
            //std::cout<<"Found key: "<<key<<"\n";
            return current;
        }
    }
    return NULL;
  };

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
  header->previous = NULL; 
  tail->forward[0] =NULL;
}

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
  header->previous = NULL; 
  tail->forward[0] =NULL;
}

template <typename key_T, typename val_T>
void Map<key_T, val_T>::clear(){
      Node<key_T, val_T> *temp_head = header;
      Node<key_T, val_T> *temp;
      if(temp != NULL){std::cout<<"yes";}
      while(temp != NULL){
        temp = temp_head->forward[0];
        //std::cout<<temp_head->value->first<<std::endl;
        delete temp_head;
        //std::cout<<"deleted"<<std::endl;
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
}

// create new node========================================================// 
//template <typename key_T, typename val_T>
//Node<key_T, val_T>* Map<key_T, val_T>::createNode(const std::pair <key_T, val_T> p,int level) 
//{ 
//  //cout<<"node created with"<<p.first<<"and lev:"<<level<<endl;
//  Node<key_T, val_T> *n = new Node<key_T, val_T>(p,level); 
//  return n; 
//}; ================================================================//

// Insert given key in skip list 
template <typename key_T, typename val_T>
std::pair<typename Map<key_T,val_T>::Iterator,bool> Map<key_T, val_T>::insert(const std::pair<const key_T, val_T>& p) 
{ 
    const key_T& key=p.first;
  // make temp uiterator as Iterator temp = Iterator(n<-newly created node); and return the pointer
  Node<key_T, val_T> *current = header; 

  // create update array and initialize it 
    Node<key_T, val_T> **update = new Node<key_T, val_T>* [MAXLVL+1];
    memset(update, '\0', sizeof(Node<key_T, val_T>*)*(MAXLVL+1)); 
  
  for (int i = level; i >= 1; i--) 
  {
    while (current->forward[i] != NULL && 
      current->forward[i]->value->first < key) {
      current = current->forward[i]; }
    //fill update 
      update[i] = current; 
  }

  //========JUST EXPERIMENTING ,AYE================================
      while(current->forward[0] != tail && current->forward[0]->value->first < key){
          
        current = current->forward[0];
        }
  //=============================================================== 
  
   
  
    update[0] = current;
    Node<key_T, val_T> *n = update[0];
    n = n->forward[0];
    //current = current->forward[0];
    if(n->value != NULL && (n->value->first == p.first)){
    Map<key_T,val_T>::Iterator retit = Iterator(n);
    //std::cout<<"key already there"<<std::endl;
    delete[] update;//************************************************************************
    return std::make_pair(retit, false);
    }else{ 
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
    n = new Node<key_T, val_T>(p,rlevel);; 

    // insert node by rearranging pointers 
    for(int i=0;i<=rlevel;i++) 
    { 
      n->forward[i] = update[i]->forward[i]; 
      update[i]->forward[i] = n; 
    }

    
    n->previous = update[0];

    //std::cout<< n->previous->value.first<<std::endl;
    
        if(!(n->forward[0] == tail)){
            //std::cout<<"inif";
            n->forward[0]->previous = n;
        }
        else{
            tail->previous = n;
        } 
    
        //std::cout << "Inserted key\n";
        this->slsize++; 
        delete[] update;
        Iterator retit = Iterator(n);
        return std::make_pair(retit, true);
    } 

} 

template <typename key_T, typename val_T>
template <typename IT_T>
void Map<key_T, val_T>::insert(IT_T range_beg, IT_T range_end){
    auto itr = range_beg;
    while(itr != range_end){
      insert(*itr);
      ++itr;
    }
}

template<typename key_T, typename val_T>
Map<key_T,val_T>::Map(std::initializer_list<std::pair<const key_T, val_T>> l){
      //create an empty container
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
  header->previous = NULL; 
  tail->forward[0] =NULL;
      for(auto i=l.begin(); i!=l.end();i++){
      insert(*i);
      }
}


template <typename key_T, typename val_T>
 void Map<key_T, val_T>::erase(const key_T& key) 
{ 
    Node<key_T, val_T> *current = header; 
    // create update array and initialize it 
    Node<key_T, val_T> **update = new Node<key_T, val_T>* [MAXLVL+1];
    memset(update, '\0', sizeof(Node<key_T, val_T>*)*(MAXLVL+1));
 
    for(int i = level; i >= 1; i--) 
    { 
        while(current->forward[i] != NULL  && 
              current->forward[i]->value->first < key){ 
            current = current->forward[i]; }
        update[i] = current; 
    } 
    
    while(current->forward[0] != tail && current->forward[0]->value->first < key){    
  current = current->forward[0];
    }
    
    
        update[0] = current;
        Node<key_T, val_T> *n = update[0];
        n = n->forward[0];
        //current = current->forward[0]; 
  
    // If current node is target node 
    if(n->value->first == key) 
    { 
        
        for(int i=0;i<=level;i++) 
        { 
            
            if(update[i]->forward[i] != n) 
                break; 
  
            update[i]->forward[i] = n->forward[i]; 
        } 
       // displayList();
        
        //======================================
         if(!(n->forward[0] == tail)){
            n->forward[0]->previous = n->previous;
        }
        else{
            tail->previous = n->previous;
            
            n->previous->forward[0] = tail;
            
            }
        //====================================== 
     // Remove levels having no elements
     delete n;//********************************************************  
        while(level>0 && 
              header->forward[level] == NULL) 
            level--; 
         //std::cout<<"deleted key\n";
         delete[] update;//*********************************************
         slsize--;
         
    }else{
        throw std::out_of_range("Key Not in Map");
    } 
}
  
template <typename key_T, typename val_T>
void Map<key_T, val_T>::erase(Iterator it) 
{ 
    const key_T& key = it.get_current()->value->first;
    Node<key_T, val_T> *current = header; 
    // create update array and initialize it 
    Node<key_T, val_T> **update = new Node<key_T, val_T>* [MAXLVL+1];
    memset(update, '\0', sizeof(Node<key_T, val_T>*)*(MAXLVL+1));
 
    for(int i = level; i >= 1; i--) 
    { 
        while(current->forward[i] != NULL  && 
              current->forward[i]->value->first < key){ 
            current = current->forward[i]; }
        update[i] = current; 
    } 
    
    while(current->forward[0] != tail && current->forward[0]->value->first < key){    
        current = current->forward[0];
    }
    
    
    update[0] = current;
    Node<key_T, val_T> *n = update[0];
    n = n->forward[0];
    //current = current->forward[0]; 
  
    // If current node is target node 
    if(n->value->first == key) 
    { 
        
        for(int i=0;i<=level;i++) 
        { 
            
            if(update[i]->forward[i] != n) 
                break; 
  
            update[i]->forward[i] = n->forward[i]; 
        } 
        // displayList();
        
        //======================================
         if(!(n->forward[0] == tail)){
            n->forward[0]->previous = n->previous;
          }
          else{
              tail->previous = n->previous;
              n->previous->forward[0] = tail;
          }
        delete n;//****************************************************SF
        //======================================
  
   
     // Remove levels having no elements  
        while(level>0 && 
              header->forward[level] == NULL) 
            level--; 
         //std::cout<<"deleted key\n";
         delete[] update;//***************************************************
         slsize--;
         
    }else{
        throw std::out_of_range("Key Not in Map");
    } 
}

// Search for element in skip list 
template <typename key_T, typename val_T>
typename Map<key_T, val_T>::Iterator Map<key_T, val_T>::find(const key_T& key)
{ 
  Node<key_T, val_T> *temp = Map<key_T,val_T>::lookupkey(key);
    if(temp == NULL){
            return Map<key_T, val_T>::Iterator(tail);
    }
    return Map<key_T, val_T>::Iterator(temp);   
}

template <typename key_T, typename val_T>
val_T& Map<key_T, val_T>::at(const key_T& key) 
{ 
    Node<key_T, val_T> *temp_head = Map<key_T,val_T>::lookupkey(key);
    if(temp_head == NULL){
            throw std::out_of_range("out of range");
    }
    else return temp_head->value->second; 
}

template <typename key_T, typename val_T>
const val_T& Map<key_T, val_T>::at(const key_T& key) const
{ 
    Node<key_T, val_T> *temp_head = Map<key_T,val_T>::lookupkey(key);
    if(temp_head == NULL){
            throw std::out_of_range("out of range");
    }
    else return temp_head->value->second;  
}

// Display skip list level wise 
template <typename key_T, typename val_T>
void Map<key_T, val_T>::displayList() 
{ // basically iterating over the map
  std::cout<<"\n*****List*****"<<"\n";
  for(auto i=this->begin();i!=this->end();i++){
      std::cout<<i.get_current()->value->first<<"  ";
  }
  std::cout<<"\n";
  std::cout<<"********************\n";
  
}

template <typename key_T, typename val_T>
std::size_t Map<key_T, val_T>::size() const
{ 
  //std::cout<<"Size is: "<<this->slsize<<std::endl;
  return this->slsize;
}


}//scope ends