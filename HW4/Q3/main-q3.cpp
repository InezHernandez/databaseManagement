// Description: Sort Merge Join algorithm. Join Emp and Dept.csv, Emp.eid = Dept.eid
// Sources: https://www.geeksforgeeks.org/difference-between-nested-loop-join-and-sort-merge-join/
// gfg: It consists of 2 phases consisting the sort operation and merge operation.
// The first row from the first table and second row from the table is taken,
// if it is not the end then, the selected rows are checked for the merger.
// If they can be merged, the merged row is returned else next rows are taken
// from the tables and the steps are repeated until the rows are exhausted.]
// https://www.youtube.com/watch?v=HyZtBGXLN00 for some pseudo code of sort-merge join

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

// Some other considerations:
// int, double, string are of 4, 8, 40 bytes. Specifically wil need to add in 40 bytes constraint:
const int STRING_SIZE = 40;
// Each block can fit at most one tuple of an input relation.
// There are at most 22 blocks available to the join algorithm in the main memory:
const int MAX_BLOCKS = 22;
const int MAX_VAL = 500; // will be used as an arbitrary value for size of file
int empSize;
// now adding size of dept file:
int deptSize;
// for join:
int joinSize;

// will need to read in the emp and dept file to sort:
struct EmpBlock {
    int eid;
    string ename;
    int age;
    double salary;
};

struct DeptBlock {
    int did;
    string dname;
    double budget;
    int managerid;
};

// We will also now need a new struct to hold the merged information:
struct joinED {
  int did;
  int eid;
  string dname;
  double budget;
  int managerid;
  string ename;
  int age;
  double salary;
};

// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock *grabEmp() {
  fstream empin;
  empin.open("Emp.csv", ios::in);
  string line, word;
  EmpBlock *emp;
  // setting up a temporary struct so we can fill in the sorted values
  EmpBlock *temp = new EmpBlock[MAX_VAL];
  int i = 0, j = 0;
  // flags check when relations are done being read
  bool flag = true;

  // loop through the file and fill in the unsorted date into a temp struct
  // once done we can reassign values to the emp struct
  while (flag) {
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        temp[i].eid = stoi(word);
        getline(s, word, ',');
        temp[i].ename = word;
        getline(s, word, ',');
        temp[i].age = stoi(word);
        getline(s, word, ',');
        temp[i].salary = stod(word);
        i++;
    }
    else
    {
      flag = false; // break the while loop
    }
  }

  empSize = i - 1;
  // in prior step above we set our temp to [MAX_VAL]
  // Now that we know the size of the struct array, lets just return that:
  EmpBlock *returnEmp = new EmpBlock[empSize + 1];
  for(j = 0; j <= empSize; j++)
  {
    // simply copy over to our new struct that is sized correctly
    returnEmp[j] = temp[j];
  }

  // return struct
  return returnEmp;
}

// Grab all of dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock *grabDept() {
  fstream deptin;
  deptin.open("Dept.csv", ios::in);
  string line, word;
  DeptBlock dept;
  // setting up a temporary struct so we can fill in the sorted values
  DeptBlock *temp = new DeptBlock[MAX_VAL];
  int i = 0, j = 0;
  // flags check when relations are done being read
  bool flag = true;

  // loop through the file and fill in the unsorted date into a temp struct
  // once done we can reassign values to the emp struct
  while (flag) {
    // grab entire line
    if (getline(deptin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        temp[i].did = stoi(word);
        getline(s, word, ',');
        temp[i].dname = word;
        getline(s, word, ',');
        temp[i].budget = stod(word);
        getline(s, word, ',');
        temp[i].managerid = stoi(word);
        i++;
    }
    else
    {
      flag = false; // break the while loop
    }
  }

  deptSize = i - 1;
  // in prior step above we set our temp to [MAX_VAL]
  // Now that we know the size of the struct array, lets just return that:
  DeptBlock *returnDept = new DeptBlock[deptSize + 1];
  for(j = 0; j <= deptSize; j++)
  {
    // simply copy over to our new struct that is sized correctly
    returnDept[j] = temp[j];
  }

  // return struct
  return returnDept;
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(joinED *join, fstream &fout) {
  int j = 0;
  for(j = 0; j < joinSize; j++)
  {
    fout << join[j].did << ',' << join[j].dname << ',' << join[j].budget << ','
    << join[j].managerid << ',' << join[j].eid << ',' << join[j].ename << ','
    << join[j].age << join[j].salary << "\n";
  }
}

// Starting by sorting the emp struct, sort by eid
// pass in our unsorted struct and returns sorted struct
// Sources: https://www.geeksforgeeks.org/structure-sorting-in-c/
bool sortEmp(EmpBlock lhs, EmpBlock rhs)
{
  return lhs.eid < rhs.eid;
}

// Same as before, will simply sort by dept.managerid
// already checked functionality with emp so simply duplicating
// pass in our unsorted struct and returns sorted struct
// Sources: https://www.geeksforgeeks.org/structure-sorting-in-c/
bool sortDept(DeptBlock lhs, DeptBlock rhs)
{
  return lhs.managerid < rhs.managerid;
}


int main()
{
  // variables for merge sort
  int outerI = 0, outerJ = 0, i, j;
  int x; // Join index
  int blockCheck; // for keeping track of 22 block size restriction
  bool check = false;

  // getting join file information ready
  fstream joinout;
  joinout.open("join.csv", ios::out | ios::trunc);
  joinSize = 0;

  // get structs
  EmpBlock *empBlock = grabEmp();
  DeptBlock *deptBlock = grabDept();
  joinED *join = new joinED[MAX_VAL]; // we also need to add struct to hold join

  // now that both files have been read and structs filled, we can sort
  // will want to pass in our unsorted strcut and sort by the dept.managerid and the Emp.eid for our merge
  // https://www.cplusplus.com/forum/general/97555/
  std::sort(empBlock, empBlock + empSize + 1, sortEmp);
  // success! Currently orders from least to greatest
  // same deal as with emp:
  std::sort(deptBlock, deptBlock + deptSize + 1, sortDept);

  // now that everything is sorted we can merge:
  // pseudo code: https://www.youtube.com/watch?v=HyZtBGXLN00 for some pseudo code of sort-merge join
	do
  {
		i = outerI;
    j = outerJ;

    // while we are within the MAX_BLOCKS allocated
		for(blockCheck = 0; blockCheck <= MAX_BLOCKS; blockCheck++)
    {

      // from pseudo code, if PR.x == PS.x
      // where R = dept and S = emp
      if(deptBlock[i].managerid == empBlock[j].eid)
      {

        // found a match, add to join
        // incriment join size, will be used when writing to file join.csv
        joinSize++;

        // now simply assign the values of to our join struct
        join[x].eid = empBlock[j].eid;
        join[x].ename = empBlock[j].ename;
        join[x].age = empBlock[j].age;
        join[x].salary = empBlock[j].salary;
        join[x].did = deptBlock[i].did;
        join[x].dname = deptBlock[i].dname;
        join[x].budget = deptBlock[i].budget;
        join[x].managerid = deptBlock[i].managerid;
        //cout << "match: " << joinSize << " i: " << i << " j: " << j << "\n";

        // move array counts forward
        x++;
        i++;
      }

      else if(deptBlock[i].managerid < empBlock[j].eid)
      {
        // else if PR.x <= PS.x, then PR++
        // if the emp.eid is less than dept.mangerid then we move forward
        // the emp pointer since we always move forward the smallest one
        i++;
      }

      else if(deptBlock[i].managerid > empBlock[j].eid)
      {
        // else if PR.x > PS.x, then PS++
        // otherwise if the dept.managerid is less than the emp.eid move dept pointer
        j++;
      }

		} // end for loop

    // update outer loop count
		outerI = outerI + i;
		outerJ = outerJ + j;

    // loop control check, had to to +1 because of off indexing
		if(outerI + 1 > deptSize || outerJ + 1 > empSize)
    {
      //cout << "outerI: " << outerI << " OuterJ: " << outerJ << "\n";
      // if we reached the end of either struct, return true
			check = true;
		}

  } while(check != true);

  // sending to be printed to file
  printJoin(join, joinout);
  return 0;
}