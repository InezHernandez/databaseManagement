// Description: External Memory Sorting
// Authors: Jackson E and Inez Hernandez F

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;


struct EmpBlock {
	int eid;
	string ename;
	int age;
	double salary;
};


// Using the multi-pass multi-way sorting algorithm
// sorting based on EmpBlock.eid
void sortEmployees(vector<EmpBlock>* employees) {
	// magic
}

// returns the index of failure of a "sorted" list, -1 otherwise
int verifySorted(vector<EmpBlock> employees) {
	for (int i = 1; i < employees.size(); i++) {
		if (employees.at(i).eid < employees.at(i - 1).eid)
			return i;
	}

	return -1; // passed test
}

// write every employee to a file (EmpSorted.csv)
void writeEmployees(vector<EmpBlock> employees) {
	// Verify employees were sorted, write if so
	int index = verifySorted(employees);
	if (index) {
		cout << "Employee list failed sort @ index " << index << "\n";
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

// Prints employees to the console for debugging
void printEmployees(vector<EmpBlock> employees) {
	for (int i = 0; i < employees.size(); i++) {
		EmpBlock emp = employees.at(i);
		cout << emp.eid << ", " << emp.ename << ", " << emp.age << ", " << emp.salary << "\n";
	}
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
	// Load employees into memory
	vector<EmpBlock> employees = getEmployees();

	// Sort employees
	sortEmployees(&employees);

	// Verify employees were sorted, write if so
	writeEmployees(employees);
}