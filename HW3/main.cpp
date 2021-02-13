// Description: Hash Indexing

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

// read the file
// while loop for switch statement
// hash index:
// write to file

using std::cout;
using namespace std;


// sources: https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c/16388594
enum Options {
    index_creation,
    lookup,
    Option_Invalid,
};

Options resolveOption(std::string switchInput) {
   if( switchInput == "IC" ) return index_creation;
   if( switchInput == "L" ) return lookup;
   return Option_Invalid;
}

int main() {
  string switchInput;
  cout << "Starting Linear Hash Indexing: \n";
  while(1)
  {
    cout << "Switch between (IC) index creation and (L) look up using ID: ";
    cin >> switchInput;
    switch(resolveOption(switchInput))
    {
      case index_creation: {
        cout << "index_creation\n";
        break;
      }
      case lookup: {
        cout << "lookup\n";
        break;
      }
      // report input error to user.
      default: {
        cout << "Invalid input, try again. (IC) for index creation or (L) for look up using ID: ";
        break;
      }
    }
  }


  return 0;
}
