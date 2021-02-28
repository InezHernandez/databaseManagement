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

using namespace std;

// Some other considerations:
// int, double, string are of 4, 8, 40 bytes. Specifically wil need to add in 40 bytes constraint:
const int STRING_SIZE = 40;
// Each block can fit at most one tuple of an input relation.
// There are at most 22 blocks available to the join algorithm in the main memory:
const int MAX_BLOCKS = 22;

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


// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin) {
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream &deptin) {
    string line, word;
    DeptBlock dept;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
        return dept;
    } else {
        dept.did = -1;
        return dept;
    }
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream &fout) {
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ','
        << emp.salary << ',' << dept.did << ',' << dept.dname << ','
        << dept.budget << "\n";
}

int main() {
  // open file streams to read and write
  fstream empin;
  fstream joinout;
  empin.open("Emp.csv", ios::in);
  joinout.open("Join.csv", ios::out | ios::app);
  // flags check when relations are done being read
  bool flag = true;
  while (flag) {
      // FOR BLOCK IN RELATION EMP

      // grabs a block
      EmpBlock empBlock = grabEmp(empin);
      // checks if filestream is empty
      if (empBlock.eid == -1) {
          flag = false;
      }
      bool iflag = true;
      // opens new filestream for dept relation (needs to read in a new one each time a new emp block is seen)
      fstream deptin;
      deptin.open("Dept.csv", ios::in);
      while (iflag) {
          // FOR BLOCK IN RELATION DEPT
          DeptBlock deptBlock = grabDept(deptin);

          // in theory these would iterate through the two blocks: empBlock and deptBlock
          // but since both only contain one tuple, no iteration is needed
          if (deptBlock.did == -1) {
              iflag = false;
          } else {
              // check join condition and print join to output file
              if (deptBlock.managerid == empBlock.eid) {
                  printJoin(empBlock, deptBlock, joinout);
              }
          }
      }
  }

  return 0;
}
