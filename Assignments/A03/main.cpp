/*****************************************************************************
*                    
*  Author:           Noah Orta
*  Email:            noahraulscout@gmail.com
*  Label:            A04
*  Title:            Linked List Class
*  Course:           CMPS 3013
*  Semester:         Spring 2020
* 
*  Description:
*        This program implements a class that allows a linked list to be used just like 
*        an array. It overloads the "[]" (square brackets) to simulate accessing seperate 
*        array elements, but really it traverses the list to find the specified node using
*        an index value. It also overloads the "+" sign allowing a user to "add"
*        or "push" items onto the end of the list. This class is not meant to replace 
*        the STL vector library, its simply a project
*        to introduce the beginnings of creating complex / abstract data types. 
*        
*  Usage: 
*       - $ ./main filename
*       
*       
*  Files:            
*       main.cpp    : driver program 
*
*****************************************************************************/

#include <iostream>

using namespace std;

int A[100];

/*
*   Node
*
*   Description:
*       A node with next pointer that holds an int
*
*   Methods:
*       Node()
*       Node(int)
*/
struct Node {
    int x;
    Node *next;
    Node() {
        x = -1;
        next = NULL;
    }
    Node(int n) {
        x = n;
        next = NULL;
    }
};

/**
 * List
 * 
 * Description:
 *      This class implements a compressions algorithm called Huffman Coding.
 *      Huffman coding assigns codes to characters such that the length of the 
 *      code depends on the relative frequency or weight of the corresponding 
 *      character. Huffman codes are of variable-length, and prefix-free
 * 
 * Public Methods:
 *                          List()                               
 *      void                Push(int val)
 *      void                Insert(int val)
 *      void                PrintTail()
 *      string              Print()
 *      int                 Pop()
 *      List                operator+(const List &Rhs)
 *      int                 operator[](int index)
 *      friend              ostream &operator<<(ostream &os, List L)
 * 
 * Private Methods:
 *     
 * 
 * Usage: 
 * 
 *      List L();                                   // Create Instance of List
 *                                            
 *      L.Push(2);                                  // adds "2" to the end of the list
 *      
 */
class List {
private:
    Node *Head;                     // front of the list
    Node *Tail;                     // end of the list
    int Size;                       // size of the list

public:

    // Sets the head and tail equal to NULL, and size equal to zero
    List() {
        Head = Tail = NULL;
        Size = 0;
    }

    /*
    *   void : Push
    *
    *   Description: 
    *       Adds a node to the end of the list. If there are no nodes
    *       in the list then it will set head and tails equal to the first node.
    *
    *   Params: 
    *       int  :  val  -  the number being added to the end of the list
    *
    *   Returns: 
    *       void
    */
    void Push(int val) {
        // allocate new memory and init node
        Node *Temp = new Node(val);

        if (!Head && !Tail) {       // list is empty
            Head = Tail = Temp;
        } else {                    // add node to the end
            Tail->next = Temp;
            Tail = Temp;
        }
        Size++;                     // increase size by one
    }

    /*
    *   void : Insert
    *
    *   Description: 
    *       Adds a node to the beginning of the list. If list was empty, it 
    *       starts the list.
    *
    *   Params: 
    *       int  :  val  -  the number being added to the beginning of the list
    *
    *   Returns:
    *       void
    */
    void Insert(int val) {
        // allocate new memory and init node
        Node *Temp = new Node(val);

        // figure out where it goes in the list

        Temp->next = Head;    // set temp's next to the current head
        Head = Temp;          // make temp the new head
        if (!Tail) {          // if the list was empty then tail also equals the new node
            Tail = Head;
        }
        Size++;               // increase size by one
    }

    /*
    *   void : PrintTail
    *
    *   Description:
    *       prints the number at the tail
    *
    *   Params: 
    *       none
    *
    *   Returns:
    *       void
    */
    void PrintTail() {
        cout << Tail->x << endl;
    }

    /*
    *   string : Print
    *
    *   Description: 
    *       returns a string of the whole list
    *
    *   Params: 
    *       none
    *  
    *   Returns:
    *       string : list - the list in string format
    */
    string Print() {
        Node *Temp = Head;
        string list;              // var to hold the whole list

        while (Temp != NULL) {    // goes through whole list
            list += to_string(Temp->x) + "->";
            Temp = Temp->next;
        }

        return list;              // returns the list as a string
    }

    // NOT IMPLEMENTED
    int Pop() {
        Size--;       // decreases size by one 
        return 0; 
    }

    /*
    *   List : operator+
    *
    *   Description:
    *       Overloading the "+" operator to make a new list from 
    *       two other lists
    *
    *   Params: 
    *       const List : &Rhs
    *
    *   Returns:
    *       List : NewList - the combination of 2 lists
    *
    */
    List operator+(const List &Rhs) {
        // Create a new list that will contain both when done
        List NewList;

        // Get a reference to beginning of local list
        Node *Temp = Head;

        // Loop through local list and Push values onto new list
        while (Temp != NULL) {
            NewList.Push(Temp->x);
            Temp = Temp->next;
        }

        // Get a reference to head of Rhs
        Temp = Rhs.Head;

        // Same as above, loop and push
        while (Temp != NULL) {
            NewList.Push(Temp->x);
            Temp = Temp->next;
        }

        // Return new concatenated version of lists
        return NewList;
    }

    /*
    *   int : operator[]
    *
    *   Description: 
    *       Implementation of [] operator.  This function returns an
    *       int value as if the list were an array.
    *
    *   Params: 
    *       int : index - the subscript of the value wanting to be retrieved
    *
    *   Returns: 
    *       int : Temp->x  -  the value at that subscript
    */
    int operator[](int index) {
        Node *Temp = Head;           // sets temp to head

        if (index >= Size) {         // index is out of bounds
            cout << "Index out of bounds, exiting";
            exit(0);
        } else {                     // retrieve the value at that subscript

            for (int i = 0; i < index; i++) {
                Temp = Temp->next;
            }
            return Temp->x;          // return the value at the subscript
        }
    }

    /*
    *   friend ostream : &operator<<
    *
    *   Description: 
    *       prints out the list
    *
    *   Params:
    *       ostream  :  &os
    *       Lists    :  L
    *
    *   Returns: 
    *       ostream  :  os
    */
    friend ostream &operator<<(ostream &os, List L) {
        os << L.Print();
        return os;
    }
};


int main(int argc, char **argv) {
    List L1;                       //  Two list objects
    List L2;                       // 

    // Push 25 items onto the first list
    for (int i = 0; i < 25; i++) {
        L1.Push(i);
    }

    // Push 50 items onto the second list
    for (int i = 50; i < 100; i++) {
        L2.Push(i);
    }

    //cout << L1 << endl;
    L1.PrintTail();               // Print out the tail of the first list
    L2.PrintTail();               // Print out the tail of the second list

    List L3 = L1 + L2;
    cout << L3 << endl;           // Prints out the third list

    cout << L3[5] << endl;        // Prints out the 5th node value of the third list
    return 0; 
}
