// File: prList.h
// About:
//          This linked list can be used in two distinct ways. Each way has its merits and downsides.
//
// Method 1:
//          Adding the item.
//
//          A node is created for the item when it is added. This is the same as using the STL list.
// 
//          Method 1 is slower than method 2, but it does not rely on external data.
// 
//          Heavy use of method 1 can cause memory fragmentation.
// 
//
// Method 2:
//          Adding a previously created node.
//
//          Method 2 is faster and helps prevent memory fragmentation.
// 
//          Method 2 is intended to be used in conjunction with node memory pools.
// 
//          Nodes added to a list using method 2 need to have their item data set manually by the user.
// 
//          Method 2 requires knowledge up front of how many nodes required.
//
// Warning:
//          Do not mix methods when adding items to a list.
//
/*
    Copyright 2014 Paul Michael McNab

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef __PRLIST_H
#define __PRLIST_H


#include "../prConfig.h"
#include "../debug/prAssert.h"
#include "../core/prDefines.h"
#include "../math/prMathsUtil.h"
#include "prContainers.h"


template <typename T>
class prList
{
public:
    // forward declarations
    class prIterator;

    // Class: prNode
    //      This class represents a node in the linked list.
    class prNode
    {
    public:
        // Friends
        friend class prList;
        friend class prIterator;

        // Method: prNode
        //      Constructor
        prNode(T a) : item(a)
        {
            prev = 0;
            next = 0;
        }

		// Method: prNode
        //      Uses template constructor. For simple types this results in 0.
        prNode() : item(T())
        {
            prev = 0;
            next = 0;
        }

    public:
        T     item;

    private:
        prNode* prev;
        prNode* next;
    };


    // Class: prIterator
    //      This class is used to step through a linked list.
    class prIterator
    {
    public:
        // Method: prIterator
        //      Constructor.
        prIterator()
        {
            head = 0;
            tail = 0;
            curr = 0;
        }

        // Method: prIterator
        //      Constructor.
        prIterator(prNode* first_node, prNode* last_node, prNode* start_node)
        {
            head = first_node;
            tail =  last_node;
            curr = start_node;
        }

        // Method: prIterator
        //      Copy constructor.
        prIterator(const prIterator& it)
        {
            head = it.head;
            tail = it.tail;
            curr = it.curr;
        }
        
        // Method: Begin
        //      Returns the node at the start of the linked list.
        //
        // Notes:
        //       The returned node will be null if the associated list is empty.
        const prNode* Begin() const { return head; }

        // Method: Begin
        //      Returns the node at the start of the linked list.
        //
        // Notes:
        //       The returned node will be null if the associated list is empty.
        prNode* Begin() { return head; }        
        
        // Method: End
        //      Returns the node at the end of the linked list.
        //
        // Notes:
        //       The returned node will be null if the associated list is empty.
        const prNode* End() const { return tail; }
        
        // Method: End
        //      Returns the node at the end of the linked list.
        //
        // Notes:
        //       The returned node will be null if the associated list is empty.
        prNode* End() { return tail; }
        
        // Method: Curr
        //      Returns the current node.
        //
        // Notes:
        //      The returned node will be null if the associated list is empty.
        //
        //      The returned node will also be null if the iterator has been used to move it past the end of the list.
        const prNode* Curr() const { return curr; }
        
        // Method: Curr
        //      Returns the current node.
        //
        // Notes:
        //      The returned node will be null if the associated list is empty.
        //
        //      The returned node will also be null if the iterator has been used to move it past the end of the list.
        prNode* Curr() { return curr; }

        // Method: Next
        //      Moves to the next item in the list.
        void Next()
        {
            if (curr)
            {
                curr = curr->next;
            }
        }

        // Method: Prev
        //      Moves to the previous item in the list.
        void Prev()
        {
            if (curr)
            {
                curr = curr->prev;
            }
        }        

        // Method: Okay
        //      Returns true if the iterator points to a valid entry in the list.
        bool Okay() const { return (curr != 0); }
        

        // Operators
        const T& operator * () const { PRASSERT(curr); return curr->item; }
              T& operator * ()       { PRASSERT(curr); return curr->item; }
        
        prIterator& operator ++ ()    { Next(); return *this; }
        prIterator& operator ++ (int) { static prIterator tmp(*this); Next(); return tmp; }

        prIterator& operator -- ()    { Prev(); return *this; }
        prIterator& operator -- (int) { static prIterator tmp(*this); Prev(); return tmp; }

        friend bool operator == (const prIterator& a, const prIterator& b) { return a.curr == b.curr; }
        friend bool operator != (const prIterator& a, const prIterator& b) { return a.curr != b.curr; }


    private:
        prNode* head;
        prNode* tail;
        prNode* curr;
    };

    
    // Class: prList
    //      Linked list class.

    // Method: prList
    //      Constructor.
    prList();

    // Method: ~prList
    //      Destructor.
    ~prList();

    // Method: Clear
    //      The list is cleared by this call and is then free to be reused with either method.
    void Clear();

    // Method: AddHead
    //      Adds a node at the head of the linked list.
    //
    // Notes:
    //       This method relies on the node being created elsewhere.
    void AddHead(prNode* node);

    // Method: AddTail
    //      Adds a node at the tail of the linked list.
    //
    // Notes:
    //       This method relies on the node being created elsewhere.
    void AddTail(prNode* node);

    // Method: AddHead
    //      Adds an item at the head of the linked list.
    void AddHead(T item);

    // Method: AddTail
    //      Adds an item at the tail of the linked list.
    void AddTail(T item);

    // Method: Begin
    //      Returns an iterator where the current node is the first node.
    prIterator Begin() { return prIterator(head, tail, head); }

    // Method: End
    //      Returns an iterator where the current node is the last node.
    prIterator End() { return prIterator(head, tail, tail); }

    // Method: Find
    //      Finds an items node if it is held within the list.
    prNode* Find(const T &item)
    {
        prNode* node = head;
        while (node)
        {
            if (node->item == item) { return node; }

            node = node->next;
        }
        return 0;
    }
    
    // Method: Remove
    //      Removes an items if it is held within the list.
    //
    // Warning:
    //      The current node pointer will be rendered invalid by this method.
    void Remove(const T &item);

    // Method: Remove
    //      Removes an items if it is held within the list.
    //
    // Warning:
    //       The current node pointer will be rendered invalid by this method.
    void Remove(prNode* node);

    // Method: Size
    //      Returns the number of items in the list.
    int Size() const { return count; }

    // Method: Insert
    //      Inserts an item into the list.
    //
    // curr    - This is the node you want to insert the item before or after.
    // item    - The item to insert.
    // insert  - INSERT_POS_BEFORE or INSERT_POS_AFTER.
    //
    void Insert(prNode* curr, const T &item, const prInsertPos insert = INSERT_POS_AFTER);

    // Method: Insert
    //      Inserts a node into the list.
    //
    // curr    - This is the node you want to insert the new node before or after.
    // node    - The node to insert.
    // insert  - INSERT_POS_BEFORE or INSERT_POS_AFTER.
    //
    void Insert(prNode* curr, prNode* node, const prInsertPos insert = INSERT_POS_AFTER);

    // Method: Destroy
    //      This function clears the heap, however it also calls delete on every item in the list.
    //
    // Notes:
    //      This function is designed to stop code duplication.
    //      This function works with either list method.
    //
    // Warning:
    //		Only use with lists whose items are pointers created by calling new.
    void Destroy();

    // Method: RemoveAt
    //      Removes an current item from the list.
    void RemoveAt(int index);

    // Method: RemoveRange
    //      Removes all the items at the specified indices
    void RemoveRange(int start, int end);

    // Array access.
    T& operator [] (unsigned int index)
    {
        PRASSERT(index < (unsigned int)count);
        PRASSERT(count > 0);

        prNode* node = head;
        PRASSERT(node);
        unsigned int i = 0;
        while(node)
        {
            if (i++ == index)
            {
                break;
            }

            node = node->next;
        }

        if (node == NULL)
        {
            PRPANIC("Exceeded array bounds");
        }
        else
        {
            return node->item;
        }
    }


private:
    // Adds a new node at the lists beginning.
    void AddNodeToHead(prNode* node);
    
    // Adds a new node at the lists end.
    void AddNodeToTail(prNode* node);

    // Inserts a item into the list.
    void InsertNode(prNode* curr, prNode* node, const prInsertPos insert);


private:
    // Stop passing by value and assignment.
    prList(const prList&);
    const prList& operator = (const prList&);


private:
    // List usage method.
	enum ListMethod
    {
         LIST_METHOD_NONE,                   // List is empty and no usage method has been set.
         LIST_METHOD_CREATE,                 // Create and add a node.
         LIST_METHOD_ADD                     // Add a pre-created node.        
    };

    ListMethod  method;           
    prNode*     head;
    prNode*     tail;
    int         count;
};


#include "prList.inl"


#endif//__PRLIST_H
