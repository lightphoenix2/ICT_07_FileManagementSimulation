#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue> 

#include "SingleEntry.h"
using namespace std;

/*
_____                _ __  __
|  __ \              | |  \/  |
| |__) |___  __ _  __| | \  / | ___
|  _  // _ \/ _` |/ _` | |\/| |/ _ \
| | \ \  __/ (_| | (_| | |  | |  __/
|_|  \_\___|\__,_|\__,_|_|  |_|\___|

Produced by : Abner, Deepak, Jon'wick, Wei Qing, Wei Qi
For			: ICT1007 File Management project.

Changelog :
13/3 3:06pm - method findEmptyEntry, line 187, needs to have the addEntry algor/method placed here to insert said file command into the simulated memory



*/

// Constant
const int MAX_ENTRIES = 128;

// Prototyping
void fillVector(vector<SingleEntry>&, int sizeOfBlock); // FillVector - fill in SingleEntry information
void printVector(const vector<SingleEntry>&); // printVector - prints the information of all memory
int findEmptyEntry(const vector<SingleEntry>&, int sizeOfBlock, int requiredBlocks); // findEmptyEntry - search for an empty block for insertion/deletion. Returns the index of empty block
void addEntry(const vector<SingleEntry>&, string); // addEntry - add a data value into a selected entry
void printCSVVector(const vector<vector<string>>&); // print CSVVector - print out the read csv commands
void fillCSV_vector(vector<vector<string>>&); // fillCSV_vector - fills in the information found in the csv file
bool checkVolumeControl(const vector<SingleEntry>&); // checkVolumeControl - checks if there is enough space for a new file to be added into the system
void checkMemorySpace(const vector<SingleEntry>&, const vector<vector<string>>&, int blockSize); // checkMemorySpace - checks if there is sufficent space in the memory, if true, insert file from csv command

int main() {
	vector<SingleEntry> memories; // create vector to hold the simulated memory
	vector<vector<string>> csvData; // Create vector to hold the csv lines

	fillCSV_vector(csvData); // populate CSV vector with the information found in the csv file
	printCSVVector(csvData); // print out the information found in the CSV vector
		
	cout << endl << "Enter size of number of block size: ";	 // Requesting user for the size of per block
	int blockSize;											 // blockSize to hold the user input.
	cin >> blockSize;										 // This will be used to calculate the total number of block(s) in the simulated memory

	fillVector(memories, blockSize); // fill the simulated memory with mock information such as [index][block index][data value]
	printVector(memories);			 // print out the information found in memories vector

	bool haveSpace = checkVolumeControl(memories);		  // checks against volume control if there is avaliable space for a new file to be placed.
	if (haveSpace == true) {
		checkMemorySpace(memories, csvData, blockSize); // checks how many blocks the new add file will take.
	}
	else {
		cout << "[ALERT] Simulated memory is already populated and CANNOT insert any more files." << endl; // Self-explainatory, file system full and can only perform deletes
	}
	
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
	cout << "[SUCCESS] Completed filling of Memory! - - - - - " << endl;
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
	cout << "[SUCCESS] Completed initialization of Memory! - - - - - " << endl;
}

void printCSVVector(const vector<vector<string>>& theCSVvector) {

	cout << endl << "Commands found inside csv file are as follows: " << endl;
	unsigned int size = theCSVvector.size();
	for (unsigned int i = 0; i < size; i++) {
		cout << "[" << i << "]";
		for (unsigned int o = 0; o < theCSVvector[i].size(); o++) {
			cout << theCSVvector[i][o];
		}
		cout << endl;
	}
}

void fillCSV_vector(vector<vector<string>>& csv_Vector) {
	cout << "- - - - - Preparing to read CSV File. - - - - - " << endl;
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
		cout << "[SUCCESS] Completed reading of CSV File! - - - - - " << endl;
	}
	else {
		cout << "[Error] Unable to access/open csv file." << endl;
	}
}

bool checkVolumeControl(const vector<SingleEntry>& myMemory) {
	int blockNumber = 0; // Control volume is always located at block 0
	unsigned int size = myMemory.size(); // gets the size of the vector
	for (unsigned int i = 0; i < size; i++) {
		if (myMemory[i].getBlock_index() == 0) {
			if (myMemory[i].getData_value() == " ") {
				return true;
			}
		}
	}
	return false;
}

int findEmptyEntry(const vector<SingleEntry>& myMemory, int sizeOfBlock, int requiredBlocks) { // searches for an empty entry inside the simulated memory. Returns vector index of empty block.
	int blockNumber;
	queue<int> myqueue; // holds the block number that can host the file based on the command
	unsigned int size = myMemory.size(); // gets the size of the vector
	for (unsigned int i = 0; i < size; i += sizeOfBlock) {
		if (myMemory[i].getBlock_index() != 0) {
			if (myMemory[i].getData_value() == " ") {
				blockNumber = i;
				if (requiredBlocks == 0) {
					myqueue.push(blockNumber);
					break;
				}
				else {
					myqueue.push(blockNumber);
					requiredBlocks -= 1;
				}
			}
		}
	}
	if (myqueue.empty()) { // since queue is empty, there is no space on the memory left for adding of data
		cout << "[ALERT]There is not space left available on the memory for 'add' operation" << endl;
	}
	else {
		while (!myqueue.empty()) { // print out the entire queue of the empty block index required by the file
			int tempIndexHolder = myqueue.front();
			// Perform the adding algorithm here
			cout << ' ' << tempIndexHolder;
			myqueue.pop();
		}
	}
	cout << endl;
	return blockNumber;
}

void checkMemorySpace(const vector<SingleEntry>& myMemory, const vector<vector<string>>& csv_Vector, int blockSize) {
	cout << endl << "- - - - - Checking if there is sufficent memory space - - - - - " << endl;
	unsigned int size = csv_Vector.size();
	int numOfBlocksRequiredByFile;
	for (unsigned int i = 0; i < size; i++) {
		if (csv_Vector[i][0] == "add") {
			numOfBlocksRequiredByFile = ((csv_Vector[i].size() - 2) / blockSize) + 1; // [cmd][file name][]..[] We don't need the [cmd][file name] hence - 2
			cout << "Block(s) required by command" << "[" << i << "]" << csv_Vector[i][0] << " : " << numOfBlocksRequiredByFile << endl;
			int emptyEntry = findEmptyEntry(myMemory, blockSize, numOfBlocksRequiredByFile-1); // searches and returns the index value of an empty memory block
		}
	}
}