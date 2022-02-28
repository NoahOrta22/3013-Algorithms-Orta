/**************************************************************************************
*                    
*  Author:           Noah Orta
*  Email:            noahraulscout@gmail.com  
*  Label:            P03
*  Title:            Trie Tree Dictionary Lookup
*  Course:           CMPS 2143
*  Semester:         Spring 2022
* 
* Description:
*      Lets the user type in a substring a letter at a time, then searches a 
*      dictionary of words for possible matches using a trie tree.
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
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <termios.h>             //
#include <thread>                // for getch()
#include <unistd.h>              //
#include "timer.hpp"
#include "tempcolor.hpp"

using namespace std;

char getch(void);


/*
*   Tries
*
*   Description:
*       Holds a Trie Tree of dictionary words that can be searched up
*       in log n time.
*
*   Private:
*       struct          node
*
*   Public:
*       
*                       Tries()
*                      ~Dictionary()
*
*       void            makeTree()
*       node*           newChild(char const, node* &)
*       void            findSubstring(string)
*       void            trueFind(node* &, char, vector<string> &, string)
*       void            printMatches(vector<string>, string)
*`      bool            isChild(char const , node* &)
*       bool            isWord(node* const &)
*       bool            inVector(vector<string>, string)
*
*   Usage:
*       Tries tree                    - makes a Tries object
*       tree.findSubstring()          - uses the search function
*
*/
class Tries {

  struct node {

    char letter;                                // the letter that the node holds
    bool isWord;                                // is true if that node is a word
    unordered_map<char, node*> child;           // key is the letter, value is a node*

    node(char l = 'a', bool word = false) {
      letter = l;
      isWord = word;
    }
  };

  public:

  ifstream infile;               // file object

  node* root;                    // the root of the tree
  int numMatches;                // number of word matches to the current substring

  Tries() {
    root = new node('0');

    numMatches = 0;

    infile.open("dictionary.txt");
  }

/*
*    Description: 
*        Makes the trie tree from the words in the dictionary.
*        
*/
  void makeTree() {
    string word;

    while(!infile.eof()) {
      infile >> word;                             // gets the word from the file
      node* temp = root;                          // starts at the root of the tree

      for(int i = 0; i < word.size(); i++) {      // loops through letters of the word
        char letter = word[i];                    // grabs the letter
        temp = newChild(letter, temp);            // goes to the child node
      }
      temp->isWord = true;
    }

  }

  /*
  *   Description: 
  *       looks to see if a parent has a curtain child (letter), if so, 
  *       it returns the child. If the parent doesn't already have that child
  *       then it created a child for that parent and returns the node
  *       of the child.
  *
  *   Params: 
  *       char   :   letter  - the letter we're looking for
  *       node*  :  &parent  - the parent of that letter
  *
  *   Returns:
  *       The child node.
  *
  */
  node* newChild(char const letter, node* &parent) {

    if(!isChild(letter, parent)) {                            // if the letter is not a child
      parent->child.insert({letter, new node(letter)});      // add the letter to it's children
    }

    return parent->child[letter];                            // return the child
  }


  /*
  *    Description:
  *        Finds the substring of the word being passed in if it's in the tree
  *
  */
  void findSubstring(string word) {

    numMatches = 0;                                            // reset the number of matches found
    
    node* parent = root;                                       //  start the search at the root
    vector<string> suggestions = {word};                       //  holds a list of word suggestions
    bool inMap = true;                                         //  the lookup subscript is in the tree
    Timer T;

    T.Start();                                                 //  starts the timer
  
    //  *THIS JUST SEES IF IT'S IN THE TREE OR NOT*
    for(int i = 0; i < word.size(); i++) {                     //  go through the characters of the substring
      if(isChild(word[i], parent))                             //  if the substring exists in the tree
        parent = parent->child[word[i]];                       //  go to the parents child    
      else {
        inMap = false;                                         // if the substring doesn't exist
        break;                                                 // break from the loop
      }  
    }
    
    if(inMap) {                                                //  If it's in the tree
      numMatches++;                                            //  add to the number of matches
      trueFind(parent, 'a', suggestions, word);                //  go look for it's kids                       
    }
    else                                                       //  it's not in the tree
      suggestions.empty();                                     //  take the current substring out of the vector

    T.End();                                                   //  Stops the timer
    
    cout << "Matches found: " << termcolor::magenta << numMatches << termcolor::reset << endl;
    cout << "Time taken: " << termcolor::cyan << T.Seconds() << termcolor::reset << " seconds\n\n";
    printMatches(suggestions, word);
  }

  /*
  *    Description:
  *        This is the RECURSIVE FUNCTION that is RESPONSIBLE FOR FINDING POSSIBLE WORD SUGGESTIONS
  *
  *    Params:
  *        node*          :  &currentNode - the current parent of the substring that we are looking at
  *        char           :   best        - the current best letter in alphabetical order
  *        vector<string> :  &suggestions - a vector of suggestion words based off the inputted substring
  *        string         :   currentSub  - the current substring of a words we are looking at
  *
  */
  void trueFind(node* &currentNode, char best, vector<string> &suggestions, string currentSub) {

    // if the letter is <= 'z', vector < 10, parent has kids
    if(currentNode->child.size() && best < 123){ // && suggestions.size() < 10) {
      // if the "root" (current substring) has a child for the current letter we're looking for
      if(isChild(best, currentNode)) {   

        if(isWord(currentNode->child[best])) {                                  // if its a word                              
          numMatches++;                                                         // add to the number of matches
          if(suggestions.size() < 10)                                           // if there are < 10 words in vector
            suggestions.push_back(currentSub+best);                             // add word to vector of suggestions
        }

        trueFind(currentNode->child[best], 'a', suggestions, currentSub+best);  // call function again with new "root"
      }
                                                                                // go to the next letter in alphabet and look to
        trueFind(currentNode, ++best, suggestions, currentSub);                 // see if the parent has that letter for a child
      
    }
  }

  /*
  *    Description:
  *        Prints out top 10 matches of the current substring. When printing out the matches the function
  *        highlights the part of the word that is the current substring
  *
  *    Params:
  *        vector<string> : matches  -  top 10 matches for the substring
  *        string         : letters  -  the current substring 
  *
  */
  void printMatches(vector<string> matches, string letters) {
    for(int i = 0; i < matches.size(); i++) {                     //runs through the words in the string that match
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
  }


  /*
  *    Description:
  *       Returns true if the parent has a child with the letter
  *       being passed in.
  *
  *    Params:
  *       char const  :  letter  - the letter we're looking for
  *       node*       : &parent  - the parent of the character we're looking for
  *                                (the previous letter in the string) 
  *
  *    Returns:
  *       True if the parent has a child with the letter being passed in.
  */
  bool isChild(char const letter, node* &parent) {
    return parent->child.find(letter) != parent->child.end();
  }

  /*
  *    Description:
  *        Returns whether the node in the trie tree is a word or not
  *
  *    Params:
  *        node* const& - word  :  the node in question
  * 
  *    Returns:
  *        bool  -  word->isWord
  */
  bool isWord(node* const &word) {
    return word->isWord;
  }

  /*
  *    Description:
  *        Sees if a curtain word is in a vector of words
  *
  *    Params:
  *        vector<string> : list  -  list of strings
  *        string         : word  -  the word we're looking for
  *   
  *    Returns:
  *        bool
  */
  bool inVector(vector<string> list, string word) {
    return find(list.begin(), list.end(), word) != list.end();
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

    Main Driver for the program

*/
int main() {

  Tries graph;                                            //  Create a Tries object
  Timer T;                                                //  Create a Timer object
  

  T.Start();                                              // Start the timer
  graph.makeTree();                                       // Read words into linked list dictionary
  T.End();                                                // End Timer

  cout << "(Press 'Enter' at any time to exit)\n\n";

  cout << "Took " << termcolor::blue << (T.Seconds()) << " seconds ";
  cout << termcolor::reset << "to read in dictionary. " << endl;
  cout << "Please type a word...\n";

  char n;                                                  // holds the last char that the user typed
  string word;                                             // the current substring
  
  while((n = getch()) != 10) {                             // while the user doesn't type enter
    system("clear");                                       // clears the screen

    n = tolower(n);                                        //  makes sure the char is lowercase
    if(n == 127) {                                         //  the input is a backspace
      if(word.size()) {                                    //  the word has at least one letter
        word.erase(word.size()-1);
      }
    }
    else if(n < 97 || n > 122){                            //  the input is not a letter                                 
      cout << "Please type in a letter\n\n";
    }
    else {
      word += n;                                           //  add to the letter typed in to the end of the word
    }
    
    //cout << "Last character input: " << n << endl;       // prints out the letter they just typed in

    if(word.size()) {                                      // the word has at least one letter
      cout << "Current Substring: " << termcolor::blue << word << termcolor::reset << "\n\n";                                // prints out the word so far
      T.Start();                                           //  start timer
      graph.findSubstring(word);                           //  find matches
      T.End();                                             //  end timer
    }
    else {
      cout << "Please type a word...\n";
    }
  }
  
}
