// Description: Hash Indexing
// Authors: Jackson E and Inez Hernandez F

#include <iostream>
#include <string>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::string;

void getHashIndex(string id) {
	cout << "   Looking up " << id << "\n";
}

void createHashIndex() {
	cout << "   Generating hash index...\n";
}

int main() {
	bool running = true;
	cout << "Welcome. Available commands: \'C\', \'L\' <id>, and \'E\'.\n";

	// Main loop for user input handling.
	while (running) {
		// Get users input
		string input;
		cout << " > ";
		getline(cin, input);

		if (input[0] == 'C' || input[0] == 'c') {
			createHashIndex();
		}
		else if (input[0] == 'L' || input[0] == 'l') {
			if (input.length() > 2)
				getHashIndex(input.substr(2));
			else
				cout << " x Looking up a hash needs an id.\n";
		} else if (input[0] == 'E' || input[0] == 'e') {
			running = false;
		} else {
			cout << " x Error reading command, usage:\n  - C (create hash index)\n  - L <id> (lookup hash index)\n  - E (exit)\n";
		}
	}

	cout << "Goodbye!\n";

    return 0;
}