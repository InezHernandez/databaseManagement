// Description: External Memory Sorting
// Authors: Jackson E and Inez Hernandez F

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

static int MAX_BLOCKS = 22;

struct EmpBlock {
	int eid;
	string ename;
	int age;
	double salary;
};

// Forward declaration
void printEmployees(vector<EmpBlock> employees);


///////////////////////
// Sorting functions //
///////////////////////

// uses the two pointer method to sort a given block
void mergeBlock(vector<EmpBlock>& block, int left, int middle, int right) {
	if (block.size() == 1) return; // no need to sort one element

	int i = 0, j = 0;
	int k = left;
	int lLen = middle - left + 1;
	int rLen = right - middle;

	// make temp arrays and copy data
	vector<EmpBlock> lArr, rArr;
	for (int x = 0; x < lLen; x++)
		lArr.push_back(block.at(left + x));
	for (int y = 0; y < rLen; y++)
		rArr.push_back(block.at(middle + 1 + y));

	// merge temp arrays
	while (i < lLen && j < rLen) {
		if (lArr.at(i).eid <= rArr.at(j).eid) {
			block[k] = lArr[i];
			i++;
		} else {
			block[k] = rArr[j];
			j++;
		}
		k++;
	}

	// copy remainder
	while (i < lLen) {
		block[k] = lArr[i];
		i++;
		k++;
	}
	while (j < rLen) {
		block[k] = rArr[j];
		j++;
		k++;
	}
}

// uses merge sort to sort a given block
// used (https://www.tutorialspoint.com/cplusplus-program-to-implement-merge-sort) as a reference
void mergeSort(vector<EmpBlock>& block, int left, int right) {
	if (left < right) {
		int middle = left + (right - left) / 2;
		mergeSort(block, left, middle);
		mergeSort(block, middle + 1, right);
		mergeBlock(block, left, middle, right);
	}
}

// returns a block of employees given a start and end point
vector<EmpBlock> fillBlock(vector<EmpBlock> employees, int start, int end) {
	vector<EmpBlock> block;

	if (start + end > employees.size())
		end = employees.size() - start;

	for (int i = start; i < start + end; i++)
		block.push_back(employees.at(i));

	return block;
}

// main sorting function wrapper. manages runs
vector<EmpBlock> sortEmployees(vector<EmpBlock> employees) {
	int numRuns = 1;
	vector<EmpBlock> sorted;

	// Outer loop goes through every "run", or pass
	while ((numRuns * MAX_BLOCKS) <= employees.size()) {
		int index = 0;
		sorted.clear();

		// run through specific blocks in a run
		while (index < employees.size()) {
			// grab a block
			vector<EmpBlock> block = fillBlock(employees, index, numRuns * MAX_BLOCKS);

			// merge sort the block
			mergeSort(block, 0, block.size() - 1);

			// add block to sorted
			for (int i = 0; i < block.size(); i++)
				sorted.push_back(block.at(i));

			// increment starting index
			index += (numRuns * MAX_BLOCKS);
		}

		employees.clear();
		employees = sorted;
		numRuns++;
	}

	return sorted;
}


/////////////////////////////////
// File IO and debug functions //
/////////////////////////////////

// returns the index of failure of a "sorted" list, -1 otherwise
int verifySorted(vector<EmpBlock> employees) {
	for (int i = 1; i < employees.size(); i++) {
		if (employees.at(i).eid < employees.at(i - 1).eid)
			return i;
	}

	return -1; // passed test
}

// Prints employees to the console for debugging
void printEmployees(vector<EmpBlock> employees) {
	for (int i = 0; i < employees.size(); i++) {
		EmpBlock emp = employees.at(i);
		cout << emp.eid << ", " << emp.ename << ", " << emp.age << ", " << emp.salary << "\n";
	}
}

// write every employee to a file (EmpSorted.csv)
void writeEmployees(vector<EmpBlock> employees) {
	// Verify employees were sorted, write if so
	int index = verifySorted(employees);
	if (index != -1) {
		cout << "Employee list failed sort at entry #" << index + 1 << ".\n";
		return;
	}
	
	ofstream file;
	file.open("EmpSorted.csv", ios::trunc);

	for (int i = 0; i < employees.size(); i++) {
		EmpBlock emp = employees.at(i);
		file << emp.eid << "," << emp.ename << "," << emp.age << "," << emp.salary << "\n";
	}

	file.close();
}

// returns a vector of employees
vector<EmpBlock> getEmployees() {
	vector<EmpBlock> employees;
	string line, word;
	
	ifstream empFile;
	empFile.open("Emp.csv");

	if (!empFile) {
		empFile.close();
		throw invalid_argument("Input file doesn't exist.\n");
	}

	while (!empFile.eof()) {
		EmpBlock emp;
		getline(empFile, line);
		stringstream s(line);

		getline(s, word, ',');
		emp.eid = stoi(word);
		getline(s, word, ',');
		emp.ename = word;
		getline(s, word, ',');
		emp.age = stoi(word);
		getline(s, word, ',');
		emp.salary = stod(word); // for some reason stod() messes up on salaries > 999999??? no clue

		employees.push_back(emp);
	}

	empFile.close();
	return employees;
}

int main() {
	vector<EmpBlock> employees = getEmployees();
	vector<EmpBlock> sorted = sortEmployees(employees);
	writeEmployees(sorted);
	return 0;
}