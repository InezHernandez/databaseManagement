// Description: Hash Indexing
// Authors: Jackson E and Inez Hernandez F

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <functional>
#include <bits/stdc++.h>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::vector;
using std::stoi;
using std::bitset;
using std::hash;
using std::to_string;


#define MAX_CAPACITY 4096
#define BLOCK_SIZE   716
#define Employees vector<Employee>
#define Buckets vector<Bucket>


// Nodes for the index
struct Employee {
	string id;   // id
	string info; // name, bio, mid
	string binaryID;
	// unsigned long eHash;
	int size;

	Employee(string sentID, string sentInfo, string sentBinary) {
		id       = sentID;
		info     = sentInfo;
		binaryID = sentBinary;
		// eHash    = sentHash;
		size     = id.length() + info.length();
	}
};

struct Block {
	Employees records;
	Block* overflow;
};

struct Bucket { Block data; };


class HashIndex {
	private:
		bool created;
		int next, level, i;
		Employees items;
		Buckets bucketArr;

		// Split the current bucket
		// void splitBucket(Buckets arr, int n) {
		// 	next++;
		// 	if (next == n) {
		// 		level++;
				
		// 	}


		// }

		// test if a specified bucket has room for another record
		bool bucketCapacity(Bucket bucket) {
			int size = 0;

			// tally main block data
			Block block = bucket.data;
			for (int i = 0; i < block.records.size(); i++)
				size += block.records.at(i).size;

			// tally overflow block data
			block = *block.overflow;
			if (&block != NULL) {
				for (int i = 0; i < block.records.size(); i++)
					size += block.records.at(i).size;
			}

			// return true if there's room, false otherwise
			if ((float)size / MAX_CAPACITY >= 0.8 && size + BLOCK_SIZE <= MAX_CAPACITY)
				return true;
			else
				return false;
		}

		// hash a given id based on a specified number of significant bits
		unsigned long hashID(string id, int i) {
			hash<string> hashed;
			return hashed(id.substr(id.length() - i));
		}

		// Add a new bucket to the bucket array
		void addBucket() {
			Employees tEmp;
			Block* tBlockPointer = nullptr;
			Block tBlock(tEmp, tBlockPointer);
			Bucket tBucket(tBlock);
			bucketArr.push_back(tBucket);
		}

		// Insert a given line of data into the bucket system
		void insert(string line) {
			if (!line.length()) return;

			// Parse entry data and store in memory
			string parsedID = line.substr(0, 8);
			string binaryID = bitset<24>(stoi(parsedID)).to_string();
			Employee temp(parsedID, line.substr(9), binaryID);
			items.push_back(temp);

			// insert data into a bucket
			int bucketIndex = stoi(binaryID.substr(binaryID.length() - i), nullptr, 2);
			Bucket selectedBucket = bucketArr.at(bucketIndex);
			if (bucketCapacity(selectedBucket))
				selectedBucket.data.records.push_back(temp);
			else
				return;
				// splitBucket();
		}

	public:
		HashIndex() {
			created = false;
			level = 1;
			next = 0;
			i = 3;
			addBucket();
		}

		// Creation function
		void create() {
			if (created) {
				cout << "  [ERR] Output file already generated.\n";
				return;
			}

			// Check if input file exists
			ifstream file;
			file.open("Employees.csv");

			if (!file) {
				cout << "  [ERR] No input file found, please check your working directory.\n";
				file.close();
				return;
			}

			// loop through file inserting entries
			string line;
			while (!file.eof()) {
				getline(file, line);
				insert(line);
			}

			// print hashes
			// for (int i = 0; i < items.size(); i++)
			// 	cout << items.at(i).id << ": " << items.at(i).eHash << "\n";

			// Test if creation was successful
			file.close();
			file.open("EmployeeIndex.txt");

			if (file) {
				cout << " Generated index, check \'EmployeeIndex.txt\'.\n";
				created = true;
			} else
				cout << "  [ERR] Something went wrong, creation failed.\n";

			file.close();
		}

		// Lookup function
		void get(string id) {
			ifstream file;
			file.open("EmployeeIndex.txt");

			// Check if the file opened properly
			if (!file) {
				cout << "  [ERR] Index file doesn't exist, run \'C\' before \'L\'.\n";
				file.close();
				return;
			}

			cout << "  Looking up " << id << "\n";
			string line;

			// Loop through whole file
			while (!file.eof()) {
				// Find if the id occurs on the current line, print if so
				getline(file, line);
				if (line.rfind(id, 8))
					cout << line << "\n";
			}

			file.close();
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
				index.get(input.substr(2));
			else
				cout << "  [ERR] Looking up a hash needs an id.\n";
		} else if (input[0] == 'E' || input[0] == 'e') {
			running = false;
		} else if (input.length() == 0) {
			// skip if nothing is entered
			continue;
		} else {
			cout << "  [ERR] Unrecognized command, usage:\n  - C (create hash index)\n  - L <id> (lookup hash index)\n  - E (exit)\n";
		}
	}

	cout << "Goodbye!\n";
    return 0;
}