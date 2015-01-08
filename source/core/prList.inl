// ------------------------------------------------------------------------------------------------
//
//  prList.inl
//
//  Copyright 2014 Paul Michael McNab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// ------------------------------------------------------------------------------------------------


// Constructor
template<typename T>
prList<T>::prList()
{
    count  = 0;        
    head   = 0;
    tail   = 0;
    method = LIST_METHOD_NONE;
}


// Destructor
template<typename T>
prList<T>::~prList()
{
    Clear();
}


// The list is completely cleared by this call and is then free to be reused with either method.
template<typename T>
void prList<T>::Clear()
{
    // Only delete nodes if we created them.
    if (method == LIST_METHOD_CREATE)
    {
        prNode* curr = head;
        prNode* next;

        while (curr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }
    }

    count  = 0;        
    head   = 0;
    tail   = 0;
    method = LIST_METHOD_NONE;
}


// Adds a node at the head of the linked list.
template<typename T>
void prList<T>::AddHead(prNode* node)
{
    PRASSERT(node);        

    if (method == LIST_METHOD_NONE)
        method =  LIST_METHOD_ADD;

    PRASSERT(method == LIST_METHOD_ADD);

    AddNodeToHead(node);
}


// Adds a node at the tail of the linked list.
template<typename T>
void prList<T>::AddTail(prNode* node)
{
    PRASSERT(node);        

    if (method == LIST_METHOD_NONE)
        method =  LIST_METHOD_ADD;

    PRASSERT(method == LIST_METHOD_ADD);
    
    AddNodeToTail(node);
}


// Adds an item at the head of the linked list.
template<typename T>
void prList<T>::AddHead(T item)
{
    if (method == LIST_METHOD_NONE)
        method =  LIST_METHOD_CREATE;

    PRASSERT(method == LIST_METHOD_CREATE);
    
    prNode* node = new prNode(item);
    PRASSERT(node);
    
    AddNodeToHead(node);
}


// Adds a node at the tail of the linked list.
template<typename T>
void prList<T>::AddTail(T item)
{
    if (method == LIST_METHOD_NONE)
        method =  LIST_METHOD_CREATE;

    PRASSERT(method == LIST_METHOD_CREATE);
    
    prNode* node = new prNode(item);
    PRASSERT(node);
    
    AddNodeToTail(node);
}


// Removes an items if it is held within the list.
template<typename T>
void prList<T>::Remove(const T &item)
{
    prNode* node = Find(item);
    Remove(node);
}


// Removes an items if it is held within the list.
template<typename T>
void prList<T>::Remove(prNode* node)
{
    if (node)
    {
        // Keep a pointer to the current node, so we can delete it.
        prNode* temp = node;

        // Remove the head node?
        if (node == head)
        {
            // Are there any nodes left?
            if (head->next)
            {
                // Remove old head node.
                head  = head->next;                
                head->prev = 0;
            }
            else
            {
                head = 0;
                tail = 0;
            }
        }

        // Remove the tail node?
        else if (node == tail)
        {
            // Are there any nodes left?
            if (tail->prev)
            {
                // Remove old tail node.
                tail = tail->prev;                
                tail->next = 0;
            }
            else
            {
                head = 0;
                tail = 0;
            }
        }

        // Remove inbetween node.
        else
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

    
        // Now we can call delete on the node that has been unlinked.
        if (method == LIST_METHOD_CREATE)
        {   
            delete temp;
        }
    
    
        // Reduce count.
        if (--count == 0)
        {
            method = LIST_METHOD_NONE;
        }
    }
}


// Inserts an item into the list.
template<typename T>
void prList<T>::Insert(prNode* curr, const T &item, const prInsertPos insert)
{
    if (method == LIST_METHOD_NONE)
        method =  LIST_METHOD_CREATE;

    PRASSERT(method == LIST_METHOD_CREATE);

    prNode* node = new prNode(item);

    PRASSERT(node);
    PRASSERT(curr);

    InsertNode(curr, node, insert);
}


// Inserts a node into the list.
template<typename T>
void prList<T>::Insert(prNode* curr, prNode* node, const prInsertPos insert)
{
    if (method == LIST_METHOD_NONE)
        method =  LIST_METHOD_ADD;

    PRASSERT(method == LIST_METHOD_ADD);
    PRASSERT(node);
    PRASSERT(curr);

    InsertNode(curr, node, insert);
}


// The list is completely cleared by this call and is then free to be reused with either method.
template<typename T>
void prList<T>::Destroy()
{
    // If we created the nodes, then delete the nodes and the associated item.
    if (method == LIST_METHOD_CREATE)
    {
        prNode* curr = head;
        prNode* next;
        T     item;

        while (curr)
        {
            next = curr->next;
            item = curr->item;

            if (item)
            {
                delete item;
            }

            delete curr;

            curr = next;
        }
    }
    // Else just delete the items.
    else
    {
        prNode* curr = head;
        T     item;

        while (curr)
        {
            item = curr->item;

            if (item)
            {
                delete item;
            }

            curr = curr->next;
        }
    }

    count  = 0;        
    head   = 0;
    tail   = 0;
    method = LIST_METHOD_NONE;
}


// Removes an current item from the list.
template<typename T>
void prList<T>::RemoveAt(int index)
{
    PRASSERT(index < count);
    PRASSERT(head);

    prNode* node = head;
    int i = 0;
    while(node)
    {
        if (i++ == index)
        {
            Remove(node);
            return;
        }

        node = node->next;
    }
}


// Removes all the items at the specified indices
template<typename T>
void prList<T>::RemoveRange(int start, int end)
{
    PRASSERT(start < count);
    PRASSERT(end   < count);
    PRASSERT(start > -1);
    PRASSERT(end   > -1);

    if (start > end)
    {
        Proteus::Math::prSwap<int&>(start, end);
    }

    int amount = (end - start) + 1;

    for (int i=0; i<amount; i++)
    {
        RemoveAt(start); // Removes same index *count* times as the links shift down
    }
}


// ------------------------------------------------------------------------------------------------
//
//  Private methods.
//
// ------------------------------------------------------------------------------------------------

// Adds a new node at the lists beginning.
template<typename T>
void prList<T>::AddNodeToHead(prNode* node)
{
    if (0 == head)
    {
        head = node;
        tail = node;

        node->prev = 0;
        node->next = 0;
    }
    else
    {
        node->prev = 0;         // As this is the new head node, there will be no previous.
        node->next = head;      // New head nodes next will be old head node.
        head->prev = node;      // Old head nodes previous must now point to the new head.
        head       = node;      // Set new head node.
    }

    count++;
}


// Adds a new node at the lists end.
template<typename T>
void prList<T>::AddNodeToTail(prNode* node)
{
    if (0 == tail)
    {
        head = node;
        tail = node;

        node->prev = 0;
        node->next = 0;
    }
    else
    {
        tail->next = node;      // Old tails next must be set to point at the new node.
        node->prev = tail;      // New nodes previous must be the current tail.
        node->next = 0;         // New nodes next is 0.
        tail       = node;      // Now set tail node to point to the new node.
    }

    count++;
}


// Inserts a node into the list.
template<typename T>
void prList<T>::InsertNode(prNode* curr, prNode* node, const prInsertPos insert)
{
    if (insert == INSERT_POS_AFTER)
    {
        node->next = curr->next;
        node->prev = curr;

        if (curr->next)
        {
            curr->next->prev = node;
        }

        curr->next = node;

        // Is inserted node the new tail?
        if (curr == tail)
        {
             tail = node;
        }
    }
    else
    {
        node->prev = curr->prev;
        node->next = curr;

        if (curr->prev)
        {
            curr->prev->next = node;
        }

        curr->prev = node;

        // Is inserted node the new head?
        if (curr == head)
        {
             head = node;
        }
    }

    count++;
}
