#include <string>
#include <iostream>
#include <vector>
#include "SingleEntry.h"

using namespace std;
// Constant
const int MAX_ENTRIES = 128;

// Prototyping
void fillVector(vector<SingleEntry>&, int sizeOfBlock); // FillVector - fill in SingleEntry information
void printVector(const vector<SingleEntry>&); // printVector - prints the information of all memory
int findEmptyEntry(const vector<SingleEntry>&); // search for an empty block for insertion/deletion
void addEntry(const vector<SingleEntry>&, string); //add a data value into a selected entry

int main() {
	vector<SingleEntry> memories;

	cout << "Enter size of number of block size: ";	 //get the size per data block
	int blockSize;
	cin >> blockSize;

	fillVector(memories, blockSize);
	printVector(memories);
	int emptyEntry = findEmptyEntry(memories);

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
		cout << "Number of index(es) that cannot be utlized due to block size: " << unuseableIndexes << endl;
		cout << "Index, " << usableIndex << " to " << MAX_ENTRIES << " cannot be utlized due to block size." << endl;
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

int findEmptyEntry(const vector<SingleEntry>& myMemory) { // searches for an empty entry inside the simulated memory. Returns vector index of empty block.
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