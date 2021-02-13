// Description: Hash Indexing
// Authors: Jackson E and Inez Hernandez F

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <bits/stdc++.h>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::string;
using std::ifstream;

class HashIndex {
	public:
		void create() {
			cout << "  Generating hash index...\n";
		}

		void getAtID(string id) {
			ifstream file;
			file.open("EmployeeIndex.txt");

			// Check if the file opened properly
			if (!file) {
				cout << " x Index file doesn't exist, run \'C\' before \'L\'.\n";
				return;
			}

			cout << "  Looking up " << id << "\n";
			string line;

			// Loop through whole file
			while (!file.eof()) {
				// Find if the id occurs on the current line, print if so
				getline(file, line);
				if (line.find(id, 8))
					cout << line << "\n";
			}
		}
};

int main() {
	bool running = true;
	cout << "Welcome. Available commands: \'C\', \'L\' <id>, and \'E\'.\n";

	// Main loop for user input handling.
	while (running) {
		// Get users input
		HashIndex index;
		string input;
		cout << "> ";
		getline(cin, input);

		// Test for valid input and handle appropriately
		if (input[0] == 'C' || input[0] == 'c') {
			index.create();
		}
		else if (input[0] == 'L' || input[0] == 'l') {
			if (input.length() > 2)
				// getHashIndex(input.substr(2));
				index.getAtID(input.substr(2));
			else
				cout << " x Looking up a hash needs an id.\n";
		} else if (input[0] == 'E' || input[0] == 'e') {
			running = false;
		} else if (input.length() == 0) {
			// skip if nothing is entered
			continue;
		} else {
			cout << " x Error reading command, usage:\n  - C (create hash index)\n  - L <id> (lookup hash index)\n  - E (exit)\n";
		}
	}

	cout << "Goodbye!\n";
    return 0;
}