/**************************************************************************************
*                    
*  Author:           Noah Orta
*  Email:            noahraulscout@gmail.com  
*  Label:            P01
*  Title:            RPSLS
*  Course:           CMPS 2143
*  Semester:         Spring 2022
* 
* Description:
*      This program has a resizable stack and reads in numbers from a file. If the 
*      number is even it pushs to the stack and if it's odd it pops a number off 
*      the stack. Then it adjusts according to how full it is. 
*
* Files: 
*      main.cpp
*      
* 
**************************************************************************************/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*
*   Stack
*
*   Description:
*       A resizeable stack that resizes itself based on how many
*       elements are in it.
*
*   Public:
*       
*                       Stack()
*                      ~Stack()
*
*       void            setCommandCount(int)
*       void            resizeRules(double, double, double, double)
*       void            addToStack()
*       void            push(int)
*       void            pop()
*       void            checkResize()
*       void            containerGrow()
*       void            containerShrink()
*       void            formatPrint()
*       void            print()
*
*   Private:
*
*   Usage:
*
*/
class Stack {
  private:
    ifstream infile;            //  infile and outfile for the program
    ofstream outfile;           // 

    int *stack;                 //  will point to the stack

    int maxSize;                //  the max size the stack ever reached
    int resizedCount;           //  amount of times stack has been resized
    int currentStackSize;       //  current size of the stack
    int numCount;               //  amount of numbers in stack
    int commandCount;           //  amount of numbers read from the file
    double percentFull;         //  percentage of elements compared to current stack size
    double tooFull;             //  percentage of when we need to increase size of stack
    double tooEmpty;            //  percentage of when we need to decrease size of stack 
    double enlarge;             //  the multiplicity we are enlarging the stack size by
    double shrink;              //  the multiplicity we are shrinking the stack size by

  public:

    Stack() {
      infile.open("nums.dat");        //  opening input and output files
      outfile.open("output.txt");     //

      currentStackSize = 15;          
      maxSize = currentStackSize;
      numCount = 0;
      resizedCount = 0;
      percentFull = 0;
      commandCount = 0;

      tooFull = 1.00;           //  at 100% capacity we need to resize
      tooEmpty = 0.15;          //  at or below 15% capacity we need to resize
      enlarge = 2.0;            //  double the size of the stack
      shrink = 0.5;             //  cut the size of the stack in half

      stack = new int[currentStackSize];
    }

    /*
    *   ~Stack
    *
    *   Description: 
    *       Deconstructor for class. Deletes the int array pointer
    *
    */
    ~Stack() {
      delete stack;
    }

    /*
    *
    *
    *
    */
    void setCommandCount(int num) {
      commandCount = num;
    }

    /*
    *   void  :  resizeRules
    *
    *   Description: 
    *       Constructor with parameters.
    *
    *   Params: 
    *       double  :  full
    *       double  :  empty
    *       double  :  larger
    *       double  :  smaller
    *
    */
    void resizeRules(double full, double empty = 0.15, double larger = 2.0, double smaller = 0.5) {
      
      tooFull = full;           //  at ___% capacity we need to resize
      tooEmpty = empty;         //  at or below ___% capacity we need to resize
      enlarge = larger;         //  enlarge the stack by a multiplicity of ___
      shrink = smaller;         //  shrink the stack by a multiplicity of ___

    }

    /*
    *   void  :  addToStack
    *
    *   Description:
    *       Reads the file, pushes/pops numbers to/from the stack
    *
    */
    void addToStack() {
      int num;                  //  the number we're reading in from infile
      int x = 1;

      while(!infile.eof()) {
        infile >> num;          //  get number from infile

        commandCount++;         //  another command/number has been processed

        if(!(num % 2))  {       //  num is even
          push(num);
        }
        else {                  //  num is odd
          pop();
        }

        //  calculates percentage of how full stack is
        percentFull = double(numCount) / double(currentStackSize);  
        checkResize();          //  check if stack needs to be resized

        //cout << "times: " << x << endl;
        x++;

      }
      
    }

    /*
    *   void  :  push
    *
    *   Description:
    *       adds numbers to the stack
    *
    *   params: 
    *       int  :  num
    *
    */
    void push(int num) {
      stack[numCount] = num;                      //  adds the number at the end of the stack
      numCount++;                                 //  increases the size of the stack by one
    }

    /*
    *   void  :  pop
    *
    *   Description:
    *       removes the item at the top of the stack
    */
    int pop() {
      if(numCount > 0)                            //  if the list has something to pop
        numCount--;
        return stack[numCount+1];                 //  returns the number we're popping
      
    }

    /*
    *   void  :  checkResize
    *
    *   Description:
    *       Checks to see if the stack needs to be resized and resizes it if need be
    *
    */
    void checkResize() {

      // cout << "percentFull: " << percentFull << endl;
      // cout << "stack size: " << currentStackSize << endl;
      // cout << "number of items: " << numCount << endl;

      if(percentFull >= tooFull) {                //  stack is too full
        containerGrow();                          //  increase size
        resizedCount++;                           //  add to total times it's been resized
        //cout << "GROW\n";
      }
      else if(percentFull <= tooEmpty && currentStackSize > 15) {   //  stack is too empty but > 15
        containerShrink();                                          //  decrease size
        resizedCount++;                                             //  add to total times it's been resized
        //cout << "SHRINK\n";
      }
    }

    /*
    *   void  :  containerGrow
    *
    *   Description:
    *       Increases the size of the stack by the multiplicity rate of 
    *       the variable "enlarge"
    *
    */
    void containerGrow() {
      int *temp = stack;                        //  make a temp int pointer to point to old stack

      currentStackSize *= enlarge;              //  make stack larger by muliplicity rate

      if(currentStackSize > maxSize) {          //  max size of stack is less than current stack size
        maxSize = currentStackSize;             //  change it
      }
      // else {
      //   resizedCount++;                      //  DONT KNOW WHY I PUT THIS HERE
      // }

      stack = new int[currentStackSize];        //  make new stack the correct size

      for(int i = 0; i < numCount; i++) {       //  copy values from old stack to new stack
        stack[i] = temp[i];
      }

      delete temp;                              //  delete the memory that was allocated
    }

    /*
    *   void  :  containerGrow
    *
    *   Description:
    *       Decrease the size of the stack by the multiplicity rate of 
    *       the variable "shrink"
    *
    */
    void containerShrink() {
      
      int *temp = stack;                        //  make a temp int pointer to point to old stack

      currentStackSize *= shrink;               //  make smaller by muliplicity rate

      if(currentStackSize < 15)                //  prevents the stack from getting smaller than 15
        currentStackSize = 15; 

      stack = new int[currentStackSize];        //  make new stack the correct size

      for(int i = 0; i < numCount; i++) {       //  copy values from old stack to new stack
        stack[i] = temp[i];
      }

      delete temp;                              //  delete the memory that was allocated
    }

    /*
    *   void  :  formatPrint
    *
    *   Description:
    *       Prints out the max stack size, current stack size, and the 
    *       number of times it was resized
    *
    */
    void formatPrint() {

      outfile << "########################################################\n";
      outfile << "\t\tAssignment 4 - Resizing the Stack\n";
      outfile << "\t\tCMPS 3013\n";
      outfile << "\t\tNoah Orta\n\n\n";

      outfile << "\t\t\tConfig Params:\n";
      outfile << "\t\t\tFull Threshold: " << tooFull << endl;
      outfile << "\t\t\tShrink Threshold : " << tooEmpty << endl;
      outfile << "\t\t\tGrow Ratio: " << enlarge << endl;
      outfile << "\t\t\tShrink Ratio: " << shrink << "\n\n";

      outfile << "\t\tProcessed " << commandCount << " commands.\n\n";
      
      outfile << "\t\tMax Stack Size: " << maxSize << endl;
      outfile << "\t\tEnd Stack Size: " << currentStackSize << endl;
      outfile << "\t\tStack Resized " << resizedCount << " times.\n\n";
      outfile << "########################################################\n";
    }

    /*
    *   void  :  print
    *
    *   Description:
    *         Prints out the stack from bottom to top
    *
    */
    void print() {
      for(int i = 0; i < numCount; i++) {
        cout << stack[i] << "\n";
      }

      cout << "Num of elements: " << numCount << endl;
      cout << "Index of stack: " << currentStackSize << endl;
    }


};

int main(int argc, char** argv) {

  Stack stack;

  switch(argc) {

    //  One Parameter
    case 2: 
      stack.resizeRules(atof(argv[1]));
      break;

    //  Two Parameters
    case 3: 
      stack.resizeRules(atof(argv[1]), atof(argv[2]));
      break;

    //  Three Parameters
    case 4: 
      stack.resizeRules(atof(argv[1]), atof(argv[2]), atof(argv[3]));
      break;

    //  Four Parameters
    case 5: 
      stack.resizeRules(atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]));    
      break;
    
  }

  stack.addToStack();
  //stack.print();
  stack.formatPrint();

}
