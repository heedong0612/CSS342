//  Pair: Donghee Lee & Camila Valdebenito
//  CSS 342 Assignment #3
//  04/22/2019
//
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <fstream>
using namespace std;

//--------------------------  class List  ------------------------------------
// ADT List: finite, ordered collection of zero or more items.
//           The ordering is determined by operator< of T class.
//
// Assumptions:
//   -- Control of <, printing, etc. of T information is in the T class.
//   -- There is no dummy head node, head points to first node.
//      If the list is empty, head is NULL.
//   -- The insert allocates memory for a Node, ptr to the data is passed in.
//      Allocating memory and setting data is the responsibility of the caller.
//   --
//   --
//
//----------------------------------------------------------------------------

template <typename T>
class List {
    // output operator for class List, print data,
    // responsibility for output is left to object stored in the list
    friend ostream& operator<<(ostream& output, const List<T>& thelist) {
        typename List<T>::Node* current = thelist.head;
        while (current != NULL) {
            output << *current->data;
            current = current->next;
        }
        return output;
    }

public:
    List();                            // default constructor
    ~List();                           // destructor
    List(const List&);                 // copy constructor
    bool insert(T*);                   // insert one Node into list
    bool isEmpty() const;              // is list empty?
    void buildList(ifstream&);         // build a list from datafile

    void intersect(List<T> &, List<T> &);
    void merge(List<T> &, List<T> &);
    
    List<T>& operator=(const List<T> &);
    void makeEmpty();
    bool operator==(const List<T> &) const;
    bool operator!=(const List<T> &) const;
    bool remove(const T &, T *&);
    bool retrieve(const T & , T *&) const;

private:
    struct Node {             // the node in a linked list
        T* data;              // pointer to actual data, operations in T
        Node* next;
    };
    
    Node* head;              // pointer to first node in list
    void copy(const List<T> &);
};

//----------------------------------------------------------------------------
// Constructor
// Default constructor for class List
template <typename T>
List<T>::List() {
    head = nullptr;
}

//----------------------------------------------------------------------------
// Destructor
// Destructor for class List
template <typename T>
List<T>::~List() {
    makeEmpty();
}

//----------------------------------------------------------------------------
// Copy Constructor
// Copies a List and construct a copy of it
template <typename T>
List<T>::List(const List& other) {
    copy(other);
}

//----------------------------------------------------------------------------
// isEmpty
// Checks and returns true if the List is empty as defined by a nullptr head
template <typename T>
bool List<T>::isEmpty() const {
    return head == nullptr;
}

//----------------------------------------------------------------------------
// Copy
// Copies a List and construct a copy of it
template <typename T>
void List<T>::copy(const List<T> & other) {
    
    if (other.head != nullptr) {
        
        Node* temp = other.head;
        
        head = new Node;
        head->data = new T(*temp->data);
        
        Node* curr = head;
        temp = temp->next;
        
        while (temp != nullptr) {
            curr->next = new Node;
            curr->next->data = new T(*temp->data);
            curr = curr->next;
            temp = temp->next;
        }
        curr->next = nullptr;
        
        temp = nullptr;
        curr = nullptr;
    } else {
        head = nullptr;
    }
}

//----------------------------------------------------------------------------
// insert
// insert an item into list; operator< of the T class
// has the responsibility for the sorting criteria
template <typename T>
bool List<T>::insert(T* dataptr) {
    
    Node* ptr= new Node;
    if (ptr == nullptr) return false;                 // out of memory, bail
    ptr->data = dataptr;                           // link the node to data
    
    // if the list is empty or if the node should be inserted before
    // the first node of the list
    if (isEmpty() || *ptr->data < *head->data) {
        ptr->next = head;
        head = ptr;
    }
    
    // then check the rest of the list until we find where it belongs
    else {
        Node* current = head->next;          // to walk list
        Node* previous = head;               // to walk list, lags behind
        
        // walk until end of the list or found position to insert
        while (current != nullptr && *current->data < *ptr->data) {
            previous = current;                  // walk to next node
            current = current->next;
        }
        
        // insert new node, link it in
        ptr->next = current;
        previous->next = ptr;
    }
    return true;
}

//----------------------------------------------------------------------------
// buildList
// continually insert new items into the list
template <typename T>
void List<T>::buildList(ifstream& infile) {
    T* ptr;
    bool successfulRead;                            // read good data
    bool success;                                   // successfully insert
    for (;;) {
        ptr = new T;
        successfulRead = ptr->setData(infile);       // fill the T object
        if (infile.eof()) {
            delete ptr;
            break;
        }
        
        // insert good data into the list, otherwise ignore it
        if (successfulRead) {
            success = insert(ptr);
        }
        else {
            delete ptr;
        }
        if (!success) break;
    }
}

//----------------------------------------------------------------------------
// retrieve
// Takes an object of class T and a reference to a T pointer. Looks for the
// object in this List and let the pointer point to the target object
// if found. Returns false if there is no such object in the List, otherwise
// returns true.
template <typename T>
bool List<T>::retrieve(const T & elem, T *& pickitup) const {
    Node* curr = head;
    while (curr != nullptr) {
        if (elem == *curr->data) {
            pickitup = curr->data;
            curr = nullptr;
            return true;
        }
        curr = curr->next;
    }
    curr = nullptr;
    return false;
}

//----------------------------------------------------------------------------
// remove
// Takes an object of class T and a reference to a T pointer. Looks for the
// object in this List and let the pointer point to the target object
// if found. From thisList, removes the object and the node in the List
// that points to the object. After execution, the pointer points to the object
// found. Returns false if there is no such object in the List, otherwise
// returns true.
template <typename T>
bool List<T>::remove(const T & elem, T *& holdit) {
    if (head != nullptr) {
        Node* previous = head;
        
        if (*previous->data == elem) {
            head = head->next;
            holdit = previous->data;
            previous = nullptr;
            return true;
        }
        Node* current = head->next;

        while (current != nullptr) {
            if (*current->data == elem) {
                previous->next = current->next;
                holdit = current->data;
                delete current;
                previous = nullptr;
                current = nullptr;
                return true;
            }
            current = current->next;
            previous = previous->next;
        }
    }
    holdit = nullptr;
    return false;
}

//--------------------------------------------------------------------------
// makeEmpty
// makes this List empty and delets all nodes and head pointer
template <typename T>
void List<T>::makeEmpty() {
    if(head != nullptr) {
        Node* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
            delete head->data;
            delete head;
            head = curr;
        }
        delete curr->data;
        delete head;

        curr = nullptr;
        head = nullptr;
    }
}

//---------------------------------------------------------------------------
// operator==
// returns true if two Lists are equal by checking the equality of each
// objects in the nodes by order, defined by their class T.
template <typename T>
bool List<T>::operator==(const List<T> & right) const {
    if (this == &right) {
        return true;
    }
    Node* a = head;
    Node* b = right.head;
    
    while(a != nullptr && b != nullptr) {
        if(*a->data != *b->data) {
            return false;
        }
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

//----------------------------------------------------------------------------
// operator!=
// returns false if two Lists are equal by checking the equality of each
// objects in the nodes by order, defined by their class T.
template <typename T>
bool List<T>::operator!=(const List<T> & right) const {
    return !(*this == right);
}

//----------------------------------------------------------------------------
// operator=
// assigns this List to a given List by making a deep copy of it
template <typename T>
List<T>& List<T>::operator=(const List<T> & right) {
    if (this != &right) {
        makeEmpty();
        copy(right);
    }
    return *this;
}

//----------------------------------------------------------------------------
// merge
// takes two sorted Lists and set this List to be the merge of those nodes.
template <typename T>
void List<T>::merge(List<T> & first, List<T> & second) {
    
    if(first.head == nullptr && second.head == nullptr) {
        makeEmpty();
        return;
    } else if(first.head == nullptr) {
        if (&second != this) {
            makeEmpty();
        }
        head = second.head;
        if (&second != this) {
            second.head =nullptr;
        }
        
    } else if(second.head == nullptr) {
        if (&first != this) {
            makeEmpty();
        }
        head = first.head;
        if (&first != this) {
            first.head =nullptr;
        }
    } else {
        if (!(&first == this) && !(&second == this)) {
            makeEmpty();
        }
    
        Node* temp = nullptr;
        Node* ptr1 = first.head;
        Node* ptr2 = second.head;
        
        if (*ptr1->data < *ptr2->data) {
            temp = ptr1;
            ptr1 = ptr1->next;
        } else {
            temp = ptr2;
            ptr2 = ptr2->next;
        }
        Node* curr = temp;
        Node* prev = nullptr;
        
        while (ptr1 != nullptr && ptr2 != nullptr) {
            if (*ptr1->data < *ptr2->data) {
                curr->next = ptr1;
                ptr1 = ptr1->next;
            } else {
                curr->next = ptr2;
                ptr2 = ptr2->next;
            }
            prev = curr;
            curr = curr->next;
        }
        
        if (ptr1 == nullptr && ptr2 == nullptr) {
            prev->next = nullptr;
        } else if (ptr1 == nullptr) {
            curr->next = ptr2;
        } else if (ptr2 == nullptr) {
            curr->next = ptr1;
        }
        head = temp;
        
        if (&first == this) {
            second.head = nullptr;
        } else if (&second == this) {
            first.head = nullptr;
        } else {
            second.head = nullptr;
            first.head = nullptr;
        }
        prev = nullptr;
        curr = nullptr;
        temp = nullptr;
        ptr1 = nullptr;
        ptr2 = nullptr;
    }
}

//------------------------------- intersect -----------------------------------
// intersect
// Takes two sorted lists and finds the items in common in both lists,
// set this List to be made up of new Nodes with the common items.
template <typename T>
void List<T>::intersect(List<T> & first, List<T> & second) {
  
    Node* ptr1 = first.head;
    Node* ptr2 = second.head;
    
    if (&first != this && &second != this) {
        makeEmpty();
    }
    
    head = new Node;
    Node* curr = head;
    Node* last = nullptr;
    bool atLeastOne = false;
    
    while(ptr1 != nullptr && ptr2 != nullptr) {
        if(*ptr1->data == *ptr2->data) {
            atLeastOne = true;
            curr->data = new T(*ptr1->data);
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
            curr->next = new Node;
            last = curr;
            curr = curr->next;

        } else if (*ptr1->data < *ptr2->data) {
            ptr1 = ptr1->next;
        } else {
            ptr2 = ptr2->next;
        }
    }
    
    if (atLeastOne) {
        last->next = nullptr;
        delete curr;
        curr = nullptr;
    } else {
        delete head;
        head = nullptr;
    }
}

#endif
