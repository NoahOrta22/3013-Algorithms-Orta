/**************************************************************************************
*                    
*  Author:           Noah Orta
*  Email:            noahraulscout@gmail.com  
*  Label:            P02
*  Title:            Linear Dictionary Lookup
*  Course:           CMPS 2143
*  Semester:         Spring 2022
* 
* Description:
*      Lets the user type in a substring a letter at a time, then searches a 
*      dictionary of words for possible matches. 
*
* Files: 
*      main.cpp
*      tempcolor.hpp
*      timer.hpp
*      dictionary.txt
*      
* 
**************************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <termios.h>             //
#include <thread>                // for getch()
#include <unistd.h>              //
#include "timer.hpp"
#include "tempcolor.hpp"

using namespace std;

char getch(void);                // Gets a character from the terminal without the user
                                 // having to press enter, also keeps letter on terminal

/*
*   Stack
*
*   Description:
*       A resizeable stack that resizes itself based on how many
*       elements are in it.
*
*   Private:
*       struct          wordNode
*
*   Public:
*       
*                       Dictionary()
*                      ~Dictionary()
*
*       void            makeDictionary()
*       void            push(string)
*       void            search(string const &)
*       void            print
*       
*
*   Usage:
*       Dictionary dic                - makes a Dictionary object
*       dic.search()                  - uses the search function
*
*/
class Dictionary {
  private:

    //  The nodes of the dictionary
    struct wordNode {

      string word;            //  holds the dictionary word

      wordNode* next;         //  points to the next node in the list
      wordNode* prev;         //  points to the prev node in the list (just incase I wanna do it)

      wordNode(string w = "", wordNode* n = nullptr, wordNode* p = nullptr) {

        word = w;
        next = n;
        prev = p;
      }
    };

    wordNode* head;           //  points to the head of the linked list
    wordNode* tail;           //  points to the tail of the linked list

    wordNode* placeholder;    //  holds a place in the dictionary so we don't 
                              //  have to start in the beginning to search every letter

    ifstream infile;
    ofstream outfile;

  public:

    /*
    *   The constructor
    */
    Dictionary() {
      head = tail = nullptr;

      infile.open("dictionary.txt");        // the dictionary words
    }

    /*
    *   Deletes the allocated memory
    */
    ~Dictionary() {
      wordNode* temp = head;                // start at the head
      wordNode* garbage;                    // the node we're deleting

      while(temp) {
        garbage = temp;                     // assign garbage to temp
        temp = temp->next;                  // move temp down
        delete garbage;                     // delete garbage
      }


    }

    /*
    *   void  :  makeDictionary
    *
    *   Description:
    *       Read all the words in the input file, create a node, and append it to the linked list
    *
    */
    void makeDictionary() {
      
      string word;

      while(!infile.eof()) {                        // while there's still words to be read in
        infile >> word;                             // read in the word

        push(word);                                 // push the word onto the linked list
      }
    }


    /*
    *   void  :  push
    *
    *   Description:
    *       Adds a node to the back of the list.
    *
    *   Params:
    *       string  :  word  - the word in the node being added to the dictionary
    *
    */
    void push(string word) {

      wordNode* temp = new wordNode(word);        //  create the node being added

      if(head == nullptr) {                       //  the list is empty
        head = tail = temp;                       //  make head & tail point to the temp node
      }
      else {                                      //  there is at least one node in the list
        tail->next = temp;                        //  adds the node to the back of the list
        tail = tail->next; 
      }
    }

    /*
    *   void  :  search
    *
    *   Description:
    *       looks to see if the letters inputted match any of the words 
    *       in the dictionary
    *
    *   Params:
    *       string  :  letters  - the letters we're searching for
    *
    */
    void search(string const &letters) {

      Timer T;                                          // make a timer
      bool match = true;                                // all letters match this is true
      vector<string> matches;                           // the matching words

      T.Start();                                        // starts a timer

      //if(letters.size() == 1)                         // we're starting a new letter
      placeholder = head;                               // start at the beginning of the list
      

      for(int i = 0; i < letters.size(); i++) {          // goes through the letters of the word passed in
        while(placeholder->next && placeholder->word[i] != letters[i]) {      // finds the first match of the word
          placeholder = placeholder->next;
        }
        if(i > 0 && placeholder->word[i-1] != letters[i-1]) {
          match = false;
        }
      }

      int output = 0;                                  // number of matchces found
      while(placeholder && match) {                    // finds all the words that match the letter

        output++;                                      // add to the number of matches

        //cout << placeholder->word << " ";

        if(output < 10)                                // there are < 10 suggested words
          matches.push_back(placeholder->word);        // add to suggestions

        if(!placeholder->next)
          break;
        else
          placeholder = placeholder->next;

        for(int i = 0; i < letters.size(); i++) {      // checks to see if letters match

          if(placeholder->word[i] != letters[i]) {     // will go to false if they don't match
            match = false;
          }
        }                                
      }

      //cout << "Im at the word: " << placeholder->word << endl;

      T.End();                                         // stops the timer

      cout << "Matches found: " << termcolor::magenta << output << termcolor::reset << endl;
      cout << "Time taken: " << termcolor::cyan << T.Seconds() << termcolor::reset << " seconds\n\n";

      //cout << output << " matches found in " << (T.Seconds()) << " seconds.\n\n";


      for(int i = 0; i < matches.size(); i++) {                   //runs through the words in the string that match
        for(int j = 0; j < letters.size(); j++) {                 //runs through the letters in the string that match
          cout << termcolor::green << matches[i][j];              //prints out the letters as green
        }
        cout << termcolor::reset;
        for(int k = letters.size(); k < matches[i].size(); k++) { //runs throught the rest of the letters that dont match
          cout << matches[i][k];                                  //prints out the letters regular color
        }
        cout << " ";
      }
      cout << endl;

      // // prints out all the words that match
      // for(int i = 0; i < matches.size(); i++) {
      //   cout << termcolor::green << matches[i] << " ";
      // }
      // cout << termcolor::reset << endl;



    }

    /*
    *   void  :  print
    *
    *   Description: 
    *       Prints out the whole linked list
    *
    */
    void print() {
      wordNode* temp = head;
      while(temp) {
        cout << temp->word << endl;
        temp = temp->next;
      }
    }


};

/*
*   char  :  getch
*
*   Description:
*       returns the char of the keystroke that was typed
*
*   returns:
*       char  :   buf   - the char of the keystroke that was typed
*/
char getch(void) {
  char buf = 0;
  struct termios old = {0};
  fflush(stdout);
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return buf;
}

/*
*   Main Driver for the program
*/
int main() {

  Dictionary dic;                                         // A linked list of dictionary words
  Timer T;                                                // The timer for the program
  char n;                                                 // letter being typed in

  T.Start();                                              // Start the timer
  dic.makeDictionary();                                   // Read words into linked list dictionary
  T.End();                                                // End Timer

  cout << "(Press 'Enter' at any time to exit)\n\n";

  cout << "Took " << (T.Seconds()) << " seconds to read in dictionary. " << endl;
  cout << "Please type a word...\n";

  
  string word = "";                                        // the word the user is typing in
  while((n = getch()) != 10) {                             // while the user doesn't type enter
    system("clear");                                       // clears the screen

    n = tolower(n);                                        //  makes sure the char is lowercase
    if(n == 127) {                                         //  it's a backspace
      if(word.size()) {                                    //  the word has at least one letter
        word.erase(word.size()-1);
      }
    }
    else if(n < 97 || n > 122){                            //  it's not a letter                                 
      cout << "Please type in a letter\n\n";
    }
    else {
      word += n;
    }
    
    //cout << "Last character input: " << n << endl;         // prints out the letter they just typed in

    if(word.size()) {                                        // the word has at least one letter
      cout << "Current Substring: " << termcolor::blue << word << termcolor::reset << "\n\n"; // prints out the word so far
      T.Start();
      dic.search(word);
      T.End();
    }
    else {
      cout << "Please type a word...\n";
    }
  }

}
