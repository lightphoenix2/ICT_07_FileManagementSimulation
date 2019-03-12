#include <string>
#include <iostream>
#include <vector>
#include "SingleEntry.h"

using namespace std;
// Constant
const int MAX_ENTRIES = 128;

// Prototyping
void fillVector(vector<SingleEntry>&); // FillVector - fill in SingleEntry information
void printVector(const vector<SingleEntry>&); // printVector - prints the information of all memory


int main() {
	vector<SingleEntry> memories;
	fillVector(memories);
	printVector(memories);

	system("pause");
	return 0;
}

void fillVector(vector<SingleEntry>& myMemory){
	cout << "Enter size of number of block size: ";	 //get the size per data block
	int blockSize;
	cin >> blockSize;

	string value = " ";
	int blockCounter = 0;
	for (int i = 1; i <= MAX_ENTRIES; i++) {	// inserts the value such that [0]... [128]
		if (i%blockSize) {
			SingleEntry newEntry(i - 1, blockCounter, value);
			myMemory.push_back(newEntry);
		}
		else {
			SingleEntry newEntry(i - 1, blockCounter, value);
			myMemory.push_back(newEntry);
			blockCounter++;
		}
	}
	cout << "Completed filling of Memory!";
}

void printVector(const vector<SingleEntry>& myMemory){
	unsigned int size = myMemory.size();
	cout << endl << "Sample example of format" << endl;
	for (unsigned int i = 0; i < size; i++) {
		cout << "Index: " << myMemory[i].getPhysical_index();
		cout << "\t Block: " << myMemory[i].getBlock_index();
		cout << "\t File data:" << myMemory[i].getData_value() << endl;
	}
	cout << "Completed initialization of Memory!";
}



// Archival
//-----------------
//int memory[MAX_ENTRIES], block[MAX_ENTRIES], data[MAX_ENTRIES];	// creates an array of 128 elements to simulated 128 entries. [Index/rows]
//int choice, blockSize, numOfBlocks;
//cout << "Enter size of number of block size: ";	 //get the size per data block
//cin >> blockSize;
//
//int blockCounter = 0;
//for (int i = 1; i <= MAX_ENTRIES; i++) {	// inserts the value such that [0]... [128]
//	memory[i - 1] = i - 1;
//	if (i%blockSize) {
//		block[i - 1] = blockCounter;
//	}
//	else {
//		block[i - 1] = blockCounter;
//		blockCounter++;
//	}
//}
//
//cout << endl << "Sample example of format" << endl;
//for (int i = 0; i < MAX_ENTRIES; i++) {
//	cout << "Index: " << memory[i] << "\t Block: " << block[i] << "\t File data:" << endl;
//}