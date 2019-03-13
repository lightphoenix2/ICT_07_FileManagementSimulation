#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "SingleEntry.h"

using namespace std;
// Constant
const int MAX_ENTRIES = 128;

// Prototyping
void fillVector(vector<SingleEntry>&, int sizeOfBlock); // FillVector - fill in SingleEntry information
void printVector(const vector<SingleEntry>&); // printVector - prints the information of all memory
int findEmptyEntry(const vector<SingleEntry>&, int sizeOfBlock); // findEmptyEntry - search for an empty block for insertion/deletion. Returns the index of empty block
void addEntry(const vector<SingleEntry>&, string); // addEntry - add a data value into a selected entry
void printCSVVector(const vector<vector<string>> &); // print CSVVector - print out the read csv commands
void fillCSV_vector(vector<vector<string>>&); // fillCSV_vector - fills in the information found in the csv file

int main() {
	vector<SingleEntry> memories; // create vector to hold the simulated memory
	vector<vector<string>> csvData; // Create vector to hold the csv lines

	
	fillCSV_vector(csvData);
	printCSVVector(csvData);
	cout << "- - - - - Completed reading of CSV File! - - - - - " << endl;

		
	cout << endl << "Enter size of number of block size: ";	 //get the size per data block
	int blockSize;
	cin >> blockSize;

	fillVector(memories, blockSize);
	printVector(memories);
	// needs to perform checking against simulated memory if there is enough space for file.
	int emptyEntry = findEmptyEntry(memories, blockSize);

	system("pause");
	return 0;
}

void fillVector(vector<SingleEntry>& myMemory, int sizeOfBlock){
	string value = " ";
	int blockCounter = 0;
	cout << endl << "- - - - - Preparing to initalize simulated memory space. - - - - - " << endl;
	int unuseableIndexes = MAX_ENTRIES % sizeOfBlock;
	int usableIndex = MAX_ENTRIES - unuseableIndexes;
	cout << "Useable index(es): " << usableIndex << endl;

	// Init the index that can be utlized.
	for (int i = 1; i <= usableIndex; i++) {	// inserts the value such that [0]... [128]
		if (i%sizeOfBlock) {
			SingleEntry newEntry(i - 1, blockCounter, value);
			myMemory.push_back(newEntry);
		}
		else {
			SingleEntry newEntry(i - 1, blockCounter, value);
			myMemory.push_back(newEntry);
			blockCounter++;
		}
	}
	if (unuseableIndexes > 0) {
		cout << "[IMPORTANT]" << endl;
		cout << "1) Number of index(es) that cannot be utlized due to block size: " << unuseableIndexes << endl;
		cout << "2) Index, " << usableIndex << " to " << MAX_ENTRIES << " cannot be utlized due to block size." << endl;
	}
	cout << "- - - - - Completed filling of Memory! - - - - - " << endl;
}

void printVector(const vector<SingleEntry>& myMemory){
	unsigned int size = myMemory.size(); // gets the size of the vector
	cout << endl << "Simulated memory space: " << endl;
	for (unsigned int i = 0; i < size; i++) {
		cout << "Index: " << myMemory[i].getPhysical_index();
		cout << "\t Block: " << myMemory[i].getBlock_index();
		cout << "\t File data:" << myMemory[i].getData_value() << endl;
	}
	cout << "[IMPORTANT] Block 0 is reserved for volume control." << endl;
	cout << "- - - - - Completed initialization of Memory! - - - - - " << endl;
}

int findEmptyEntry(const vector<SingleEntry>& myMemory, int sizeOfBlock) { // searches for an empty entry inside the simulated memory. Returns vector index of empty block.
	int blockNumber;
	unsigned int size = myMemory.size(); // gets the size of the vector
	for (unsigned int i = 0; i < size; i++) {
		if (myMemory[i].getBlock_index() != 0) {
			if (myMemory[i].getData_value() == " ") {
				blockNumber = i;
				break;
			}
		}
	}
	cout << endl << "Empty Block found at index number: " << blockNumber << endl;
	return blockNumber;
}

void addEntry(const vector<SingleEntry>&, string) {

}

void printCSVVector(const vector<vector<string>>& theCSVvector) {

	cout << "Commands found inside csv file are as follows: " << endl;
	unsigned int size = theCSVvector.size();
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int o = 0; o < theCSVvector[i].size(); o++) {
			cout << theCSVvector[i][o];
		}
		cout << endl;
	}
}

void fillCSV_vector(vector<vector<string>>& csv_Vector) {
	cout << endl << "- - - - - Preparing to read CSV File. - - - - - " << endl;
	ifstream theCSVfile("ICT1007-SampleCSV.csv"); // declare file stream
	if (theCSVfile.is_open()) { //checks if the program is able to open the CSV file
		while (theCSVfile)
		{
			string s;
			if (!getline(theCSVfile, s)) break;

			istringstream ss(s);
			vector <string> record;

			while (ss)
			{
				string s;
				if (!getline(ss, s, ',')) break;
				record.push_back(s);
			}
			csv_Vector.push_back(record);
		}
		theCSVfile.close();
	}
	else {
		cout << "[Error] Unable to access/open csv file." << endl;
	}
}