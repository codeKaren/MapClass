#include "Map.h"

Map::Map()                      // Create an empty map (i.e., one with no key/value pairs)
: head(nullptr), tail(nullptr), m_numNodes(0)
{
}

bool Map::empty() const         // Return true if the map is empty, otherwise false.
{
   if (head == nullptr && tail == nullptr && m_numNodes == 0)
      return true;
   
   return false;
}

int Map::size() const           // Return the number of key/value pairs in the map.
{
   return m_numNodes;
}

bool Map::insert(const KeyType& key, const ValueType& value)    // insert everything to the end of the list
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
{
   // CASE 0: Can't insert because the key is already in the map
   if (contains(key))
      return false;
   
   // CASE 1: The map is empty
   if (empty())
   {
      Node* newNode = new Node;   // create a new node
      
      // initialize the values in the new node
      newNode->m_key = key;
      newNode->m_value = value;
      newNode->m_prev = nullptr;
      newNode->m_next = nullptr;
      
      // update the head and tail pointers
      head = newNode;
      tail = newNode;
      
      // increment the number of nodes
      m_numNodes++;
      
      return true;
   }
   
   // CASE 2: There is one or more elements in the map already
   else
   {
      Node* newNode = new Node;  // create a new node and initialize the key and value
      newNode->m_key = key;
      newNode->m_value = value;
      
      // attach the new node to the current last node and fix the pointers
      if (tail != nullptr)
      {
         tail->m_next = newNode;
         newNode->m_prev = tail;
         newNode->m_next = nullptr;
         tail = newNode;
         m_numNodes++;
         return true;
      }
      
      return false;   // this line of code should never be called unless there is an error in my implementation
   }
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
   // iterate through the list to find the node with the right key
   Node* iterator = head;
   while (iterator != nullptr)
   {
      if (iterator->m_key == key)
      {
         iterator->m_value = value;     // change the value of the node 
         return true;
      }
      iterator = iterator->m_next;     // move on to the next node
   }
   return false;      // key not found
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// This function should always return true.
{
   // key is already in the map, so we update its value
   update(key, value);
   
   // key is not in the map, so we insert it
   insert(key, value);
   
   return true;
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
   // CASE 0: The key is not found in the map
   if (!contains(key))
      return false;
   
   // CASE 1: The map only has one node and we want to erase it
   else if (m_numNodes == 1)
   {
      delete head;
      head = nullptr;
      tail = nullptr;
      m_numNodes--;
      return true;
   }
   
   // CASE 2: We want to erase the first node in the map (must update head pointer)
   else if(head != nullptr && head->m_key == key)
   {
      if (head->m_next != nullptr)
      {
         Node* nextNode = head->m_next;               // save the node after
         nextNode->m_prev = nullptr;
         delete head;
         head = nextNode;                             // assign the head pointer to the new first node
         m_numNodes--;
         return true;
      }
   }
   
   // CASE 3: We want to erase the last node in the map (must update tail pointer)
   else if(tail != nullptr && tail->m_key == key)
   {
      if (tail->m_prev != nullptr)
      {
         Node* prevNode = tail->m_prev;               // save the node before
         prevNode->m_next = nullptr;
         delete tail;
         tail = prevNode;                             // assign the tail pointer to the new last node
         m_numNodes--;
         return true;
      }
   }
   
   // CASE 4: We want to erase a node in the middle of the map
   else
   {
      Node* iterator = head;
      while (iterator != nullptr)
      {
         if (iterator->m_key == key)                           // find the position of the node to delete
         {
            Node* prevNode = iterator->m_prev;                 // save the nodes before and after
            Node* nextNode = iterator->m_next;
            if (prevNode != nullptr && nextNode != nullptr)
            {
               prevNode->m_next = nextNode;     // reassign the pointers for the nodes around the one being deleted
               nextNode->m_prev = prevNode;
               delete iterator;
               m_numNodes--;
               return true;
            }
         }
         iterator = iterator->m_next;       // keep iterating if the node isn't found yet
      }
   }
   
   return false;            // this line of code should never be executed unless there is an error in my implementation
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
   Node* p = head;
   while (p != nullptr)           // iterate through all of the nodes in the map
   {
      if (p->m_key == key)       // check if key is equal to a key currently in the map
         return true;
      p = p->m_next;             // move the iterator to the next element of the map
   }
   return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
   // find the node with the key
   Node* iterator = head;
   while (iterator != nullptr)
   {
      if (iterator->m_key == key)
      {
         value = iterator->m_value;     // set value parameter to the key's corresponding value
         return true;
      }
      iterator = iterator->m_next;
   }
   return false;     // key not found
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
   if (i < 0 || i >= size())      // i is not valid
      return false;
   
   // iterate i times through the list
   Node* iterator = head;
   while (i > 0 && iterator != nullptr)     // make iterator point to the (i+1)th node in the list
   {
      iterator = iterator->m_next;
      i--;
   }
   
   if (iterator != nullptr)            // get the values from the node
   {
      key = iterator->m_key;
      value = iterator->m_value;
   }
   
   return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
   // exchange the Map sizes
   int temp = m_numNodes;
   m_numNodes = other.m_numNodes;
   other.m_numNodes = temp;
   
   // exchange the head and tail pointers to the maps
   Node* tempHead = head;
   head = other.head;
   other.head = tempHead;
   
   Node* tempTail = tail;
   tail = other.tail;
   other.tail = tempTail;
}

Map::~Map()       // custom destructor
{
   // iterate through all of the nodes and delete them one by one
   Node* iterator = head;
   while (iterator != nullptr)
   {
      Node* nextNode = iterator->m_next;      // save the pointer to the next node
      delete iterator;
      iterator = nextNode;
   }
}

Map::Map(const Map& source)         // custom copy constructor
: Map()
{
   KeyType tempKey;              // create temporary values to use in the get() function with 3 parameters
   ValueType tempValue;
   
   for (int i = 0; i < source.m_numNodes; i++)
   {
      source.get(i, tempKey, tempValue);    // get the values from the source nodes and insert them into the map
      insert(tempKey, tempValue);
   }
   
   m_numNodes = source.m_numNodes;        // initialize the number of nodes
}

Map& Map::operator=(const Map& source)         // custom assignment operator
{
   if (this == &source)                 // check to see if trying to assign a map to itself
      return *this;
   
   // iterate through all of the nodes and delete them one by one
   Node* iterator = head;
   while (iterator != nullptr)
   {
      Node* nextNode = iterator->m_next;
      delete iterator;
      iterator = nextNode;
   }
   head = nullptr;             // reinitialize the map so it's empty
   tail = nullptr;
   m_numNodes = 0;
   
   // recreate the map based on the old map by creating all new nodes and initializing them
   
   KeyType tempKey;              // create temporary values to use in the get() function with 3 parameters
   ValueType tempValue;
   
   for (int i = 0; i < source.m_numNodes; i++)
   {
      source.get(i, tempKey, tempValue);         // get items from the top down
      insert(tempKey, tempValue);                // insert items from the top down
   }
   
   m_numNodes = source.m_numNodes;        // initialize the number of nodes in the list
   
   return *this;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
   // create a new empty map to hold the pairs and variables to serve as parameters to get()
   Map tempMap;
   KeyType tempKey;
   ValueType tempValue;
   ValueType mapValue;
   bool allValidPairs = true;
   
   // loop through all of the pairs in m1
   for (int i = 0; i < m1.size(); i++)
   {
      m1.get(i, tempKey, tempValue);
      if (m2.contains(tempKey))
      {
         m2.get(tempKey, mapValue);
         if (tempValue == mapValue)
            tempMap.insert(tempKey, tempValue);    // same value for key in m1 and m2, so insert
         else
            allValidPairs = false;                 // different values, so don't insert
      }
      else
      {
         tempMap.insert(tempKey, tempValue);   // insert key into tempMap because key only found in m1
      }
   }
   
   // loop through all of the pairs in m2 and insert keys unique to m2
   for (int i = 0; i < m2.size(); i++)
   {
      m2.get(i, tempKey, tempValue);
      if (m1.contains(tempKey))
         continue;
      tempMap.insert(tempKey, tempValue);   // insert key into tempMap because key only found in m2
   }
   
   result = tempMap;     // initialize result
   
   return allValidPairs;    // returns true unless the same key has diff. values in m1 and m2
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
   // create a new empty map to hold the pairs and variables to serve as parameters to get()
   Map tempMap;
   KeyType tempKey;
   ValueType tempValue;
   
   // iterate through all of the nodes in m1
   for (int i = 0; i < m1.size(); i++)
   {
      m1.get(i, tempKey, tempValue);          // find the key and value of a node in m1
      if (m2.contains(tempKey))
         continue;                  // key is found in m2, so go on to next node
      tempMap.insert(tempKey, tempValue);
   }
   
   result = tempMap;    // assign to result the correct pairs
}