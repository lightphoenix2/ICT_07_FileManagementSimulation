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
21/3 1.52pm - Add continous file allocation working.
25/3 4.17pm - Add indexing file allocation working.
	 6.00pm - Completed with READING and DELETING for continous and indexed.
	 7.42pm - COMPLETED

Left with :
(1) Additioanl File management algorithm

Note(s) :

*/

// Constant
const int MAX_ENTRIES = 128;

// Prototyping
void fillVector(vector<SingleEntry>&, int sizeOfBlock); // FillVector - fill in SingleEntry information
void printVector(const vector<SingleEntry>&); // printVector - prints the information of all memory
int findEmptyEntry(const vector<SingleEntry>&, int sizeOfBlock, int requiredBlocks); // findEmptyEntry - search for an empty block for insertion/deletion. Returns the index of empty block
void printCSVVector(const vector<vector<string>>&); // print CSVVector - print out the read csv commands
void fillCSV_vector(vector<vector<string>>&); // fillCSV_vector - fills in the information found in the csv file
bool checkVolumeControl(const vector<SingleEntry>&); // checkVolumeControl - checks if there is enough space for a new file to be added into the system
void checkMemorySpaceNadd(vector<SingleEntry>&, const vector<vector<string>>&, int, int); // checkMemorySpace - checks if there is sufficent space in the memory, if true, insert file from csv command
void readMemory(vector<SingleEntry>&, const vector<vector<string>>&, int, int); //readMemory - Reads the file based on the memory's block 0 starting file header
void deleteMemory(vector<SingleEntry>&, vector<vector<string>>&, int, int); //deleteMemory - wipes the memory of a selected file back to " " including the VCB entry.
// - - - - - - - Adding of entries into the simulated memory
void addEntryContinous(vector<SingleEntry>&, const vector<vector<string>>&, int, int);	// For continous allocation [Does not utilize blocks]
void addEntryLList(vector<SingleEntry>&, const vector<vector<string>>&, int, int, int);		// For linked list allocation [An entry will contain the file an point to another entry]
void addEntryIndex(vector<SingleEntry>&, const vector<vector<string>>&, int, int, int);		// For index allocation [The index  points to all the blocks allocated to the file]


int main() {
	vector<SingleEntry> memories;	// create vector to hold the simulated memory
	vector<vector<string>> csvData; // Create vector to hold the csv lines

	fillCSV_vector(csvData); // populate CSV vector with the information found in the csv file
	printCSVVector(csvData); // print out the information found in the CSV vector
		
	cout << endl << "Enter size of number of block size: ";	 // Requesting user for the size of per block
	int blockSize;											 // blockSize to hold the user input.
	cin >> blockSize;										 // This will be used to calculate the total number of block(s) in the simulated memory

	fillVector(memories, blockSize); // fill the simulated memory with mock information such as [index][block index][data value]
	printVector(memories);			 // print out the information found in memories vector

	cout << endl << "Enter your preferred choice of file allocation:" << "\n[1] Continous\n[2] Indexing\n[3] Linked List" << endl;
	int choice;
	cin >> choice;

	// - - - - - - - - - - Adding 
	bool haveSpace = checkVolumeControl(memories);		// checks against volume control if there is avaliable space for a new file to be placed.
	if (haveSpace == true) {
		checkMemorySpaceNadd(memories, csvData, blockSize, choice); // checks how many blocks the new add file will take.
	}
	else {
		cout << "[ALERT] Simulated memory is already populated and CANNOT insert any more files." << endl; // Self-explainatory, file system full and can only perform deletes
	}
	printVector(memories); // print out the information found in memories vector

	// - - - - - - - - - - Reading
	readMemory(memories, csvData, blockSize, choice);

	// - - - - - - - - - - Deleting
	deleteMemory(memories, csvData, blockSize, choice);
	printVector(memories); // print out the information found in memories vector

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
		for (int i = usableIndex; i <= MAX_ENTRIES; i++) {	// inserts the value such that [0]... [128]
			if (i%sizeOfBlock) {
				SingleEntry newEntry(i - 1, int("Unassigned"), value);
				myMemory.push_back(newEntry);
			}
		}
		cout << "[IMPORTANT]" << endl;
		cout << "1) Number of index(es) that cannot be utlized due to block size: " << unuseableIndexes << endl;
		cout << "2) Index, " << usableIndex << " to " << MAX_ENTRIES -1  << " are 'Unassigned' due to block size." << endl;
	}
	cout << "[SUCCESS] Completed filling of Memory! - - - - - " << endl;
}

void printVector(const vector<SingleEntry>& myMemory) {
	unsigned int size = myMemory.size(); // gets the size of the vector
	cout << endl << "Simulated memory space: " << endl;
	for (unsigned int i = 0; i < size; i++) {
		cout << "Index: " << myMemory[i].getPhysical_index();
		if ((myMemory[i].getBlock_index() > 127) || (myMemory[i].getBlock_index() < 0)) {	// this checks if the block has been assigned the 
			cout << "\t Block: Unassigned";													// int("Unassigned") value.
		}
		else {
			cout << "\t Block: " << myMemory[i].getBlock_index();
		}
		cout << "\t File data:" << myMemory[i].getData_value() << endl;
	}
	cout << "[IMPORTANT] Block 0 is reserved for volume control." << endl;
	cout << "[SUCCESS] Completed initialization of Memory! - - - - - " << endl;
}


void printCSVVector(const vector<vector<string>>& theCSVvector) {		// prints out the commands found in the CSV file
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
			if (myMemory[i].getData_value() == " ") { // if the volume control is empty, return true
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
			//cout << "Index: " << tempIndexHolder;
			myqueue.pop();
		}
	}
	cout << endl;
	return blockNumber;
}


void checkMemorySpaceNadd(vector<SingleEntry>& myMemory, const vector<vector<string>>& csv_Vector, int blockSize , int choice) {
	cout << endl << "- - - - - Checking if there is sufficent memory space - - - - - " << endl;
	unsigned int size = csv_Vector.size();
	int numOfBlocksRequiredByFile;
	int volControlFileIndex;

	for (unsigned int i = 0; i < size; i++) {
		if (csv_Vector[i][0] == "add") {

			for (unsigned int i = 1; i <= blockSize; i++) {
				string value = myMemory[i].getData_value();
				if (value == " ") {
					volControlFileIndex = i;
					break;
				}
			}
			int vectorSizeMin2 = csv_Vector[i].size() - 2;
			numOfBlocksRequiredByFile = (vectorSizeMin2 / blockSize) + 1; // [cmd][file name][]..[] We don't need the [cmd][file name] hence - 2
			cout << "Block(s) required by command" << "[" << i << "]" << csv_Vector[i][0] << " : " << numOfBlocksRequiredByFile << endl;
			cout << "File Entry/Entries: ";
			for (int o = 1; o < csv_Vector[i].size(); o++) {
				cout << csv_Vector[i][o];
			}
			cout << endl;
			int emptyEntry = findEmptyEntry(myMemory, blockSize, numOfBlocksRequiredByFile-1); // searches and returns the index value of an empty memory block
			if (emptyEntry > 0) {
				// [1] - - - For continous allocation
				if (choice == 1) {
					string vcbValue = csv_Vector[i][1] + "," + to_string(emptyEntry) + "," + to_string(vectorSizeMin2);
					myMemory[volControlFileIndex].setData_value(vcbValue);
					addEntryContinous(myMemory, csv_Vector, i, emptyEntry);
				}
				// [2] - - - For Index allocation
				if (choice == 2) {
					addEntryIndex(myMemory, csv_Vector, i, emptyEntry, volControlFileIndex);
				}
				// [3] - - - For Linked List allocation
				if (choice == 3) {
					addEntryLList(myMemory, csv_Vector, i, emptyEntry, volControlFileIndex);
				}
			}
		}
	}
}

void addEntryContinous(vector<SingleEntry>&myMemory, const vector<vector<string>>& csv_Vector, int comdNum,int emptyEntry) { // For continous allocation [Does not utilize blocks]
	for (int o = 2; o < csv_Vector[comdNum].size(); o++) {
		myMemory[emptyEntry].setData_value(csv_Vector[comdNum][o]);
		emptyEntry += 1;
	}
}

void addEntryIndex(vector<SingleEntry>&myMemory, const vector<vector<string>>& csv_Vector, int comdNum, int emptyEntry, int vcbIndex) {
	queue<int> arrayIndexes; // queue to hold the indexes where file was placed.

	for (int o = 2; o < csv_Vector[comdNum].size(); o++) {
		if (myMemory[emptyEntry].getData_value() == " ") {
			myMemory[emptyEntry].setData_value(csv_Vector[comdNum][o]);
			arrayIndexes.push(emptyEntry); // able to enqueue item to the arrayIndexes queue
		}
		emptyEntry += 1;
	}

	string arrIndex2String; // prep the queue to string 
	while (!arrayIndexes.empty()) {		// Not able to dequeue queue elements to a string array
		arrIndex2String = arrIndex2String + "," + to_string(arrayIndexes.front());
		//cout << arrIndex2String << endl;
		arrayIndexes.pop();
	}
	string vcbValue = csv_Vector[comdNum][1] + arrIndex2String; // [File][Start index]....[end index]
	myMemory[vcbIndex].setData_value(vcbValue);
}

void addEntryLList(vector<SingleEntry>&myMemory, const vector<vector<string>>& csv_Vector, int comdNum, int emptyEntry, int vcbIndex) { // For continous allocation [Does not utilize blocks]
	int vectorSizeMin2 = csv_Vector[comdNum].size() - 2;
	int startingIndex = emptyEntry;
	int tempIndex = emptyEntry;
	int endingIndex = emptyEntry;
	for (int o = 2; o < csv_Vector[comdNum].size(); o++) {
		if (myMemory[tempIndex].getData_value() == " ") {
			endingIndex = tempIndex + 1;
			if (myMemory[endingIndex].getData_value() == " ") {
				string values = csv_Vector[comdNum][o] + ","+ to_string(endingIndex);
				myMemory[tempIndex].setData_value(values);
				tempIndex = endingIndex;
			}
		}
		emptyEntry += 1;
	}	
	string vcbValue = csv_Vector[comdNum][1] + "," + to_string(startingIndex) + "," + to_string(endingIndex); // [File][Next Index][Ending index]
	myMemory[vcbIndex].setData_value(vcbValue);
}

void readMemory(vector<SingleEntry>& myMemory, const vector<vector<string>>& csv_Vector, int sizeOfBlock, int choice) {
	cout << endl << "- - - - - Reading - - - - - " << endl;
	unsigned int csv_size = csv_Vector.size(); // size of read csv file
	unsigned int size = myMemory.size(); // gets the size of the vector

	for (int i = 0; i < csv_size; i++) {
		if (csv_Vector[i][0] == "read") {
			if (choice == 1) { // [1] Continous
				for (unsigned int s = 0; s < size; s++) {
					if (myMemory[s].getBlock_index() == 0) {
						if (myMemory[s].getData_value() != " ") {
							// strip the line of the comma
							vector<string> result;
							string str = myMemory[s].getData_value();
							stringstream data(str);
							string line;
							while (getline(data, line, ',')) {
								result.push_back(line);
							}
							// end of delimitter
							if ((stoi(result[0]) + stoi(result[2])) == stoi(csv_Vector[i][1])) {
								cout << "Reading file " << csv_Vector[i][1] << " : ";
								cout << result[0];
								int indexPtr = stoi(result[1]);
								int max = (indexPtr + stoi(result[2]));
								for (indexPtr; indexPtr < max; indexPtr++) {
									cout << myMemory[indexPtr].getData_value();
								}
								cout << endl;
							}
						}
					}
				}
			}
			if (choice == 2) { // [2] index
				for (unsigned int s = 0; s < size; s++) {
					if (myMemory[s].getBlock_index() == 0) {
						if (myMemory[s].getData_value() != " ") {
							// strip the line of the comma
							vector<string> result;
							string str = myMemory[s].getData_value();
							stringstream data(str);
							string line;
							while (getline(data, line, ',')) {
								result.push_back(line);
							}
							// end of delimitter
							int sizeOfResult = result.size() - 1;
							if ((stoi(result[0]) + sizeOfResult == stoi(csv_Vector[i][1]))) {
								cout << "Reading file " << csv_Vector[i][1] << " : ";
								cout << result[0];
								for (int i = 1;i < sizeOfResult; i++) {
									int tempIndex = stoi(result[i]);
									cout << myMemory[tempIndex].getData_value();
								}
								cout << endl;
							}
						}
					}
				}
			}
			if (choice == 3) { // [3] linked list
				for (unsigned int s = 0; s < size; s++) {
					if (myMemory[s].getBlock_index() == 0) {
						if (myMemory[s].getData_value() != " ") {
							// strip the line of the comma
							vector<string> result;
							string str = myMemory[s].getData_value();
							stringstream data(str);
							string line;
							while (getline(data, line, ',')) {
								result.push_back(line);
							}
							// end of delimitter
							if ((((stoi(result[2]) - (stoi(result[1]))) + (stoi(result[0])))) == stoi(csv_Vector[i][1])) { // 14 - 8 + 100 = 106
								cout << "Reading file " << csv_Vector[i][1] << " : ";
								cout << result[0];
								int temp = stoi(result[1]);
								for (int i = stoi(result[1]); i < stoi(result[2]); i++) {
									// strip the line of the comma // Reason for another delimitter here is to read the [File entry][next index]
									vector<string> entryResult;
									if (myMemory[temp].getData_value() == " ") {
										break;
									}
									string entryStr = myMemory[temp].getData_value();
									stringstream data(entryStr);
									string entryLine;
									while (getline(data, entryLine, ',')) {
										entryResult.push_back(entryLine);
									}
									// end of delimitter
									cout << entryResult[0];
									temp = stoi(entryResult[1]);
								}
								cout << endl;
							}
						}
					}
				}
			}
		}
	}
}


void deleteMemory(vector<SingleEntry>& myMemory, vector<vector<string>>& csv_Vector, int sizeOfBlock, int choice) {
	cout << endl << "- - - - - DELETING - - - - - DELETING - - - - - DELETING - - - - - " << endl;
	unsigned int csv_size = csv_Vector.size(); // size of read csv file
	unsigned int size = myMemory.size(); // gets the size of the vector
	for (int i = 0; i < csv_size; i++) {
		if (csv_Vector[i][0] == "delete") {
			if (choice == 1) { // [1] Continous
				for (unsigned int s = 0; s < size; s++) {
					if (myMemory[s].getBlock_index() == 0) {
						if (myMemory[s].getData_value() != " ") {
							// strip the line of the comma
							vector<string> result;
							string str = myMemory[s].getData_value();
							stringstream data(str);
							string line;
							while (getline(data, line, ',')) {
								result.push_back(line);
							}
							// end of delimitter
							if (stoi(result[0]) == stoi(csv_Vector[i][1])) { // example is 200 in memory = 200 in csv 
								cout << "Deleting file " << csv_Vector[i][1];
								int indexPtr = stoi(result[1]);
								int max = (indexPtr + stoi(result[2]));
								for (indexPtr; indexPtr < max; indexPtr++) { // go to individual entries and wipe to " " values
									myMemory[indexPtr].setData_value(" ");
								}
								myMemory[s].setData_value(" "); // lastly wipe the VCB entry to " " value
								cout << endl;
							}
						}
					}
				}
			}
			if (choice == 2) { // [2] index
				for (unsigned int s = 0; s < size; s++) {
					if (myMemory[s].getBlock_index() == 0) {
						if (myMemory[s].getData_value() != " ") {
							// strip the line of the comma
							vector<string> result;
							string str = myMemory[s].getData_value();
							stringstream data(str);
							string line;
							while (getline(data, line, ',')) {
								result.push_back(line);
							}
							// end of delimitter
							int sizeOfResult = result.size();
							if (stoi(result[0]) == stoi(csv_Vector[i][1])) { // example is 200 in memory = 200 in csv 
								cout << "Deleting file " << csv_Vector[i][1];
								for (int i = 1; i < sizeOfResult; i++) {
									int tempIndex = stoi(result[i]);
									myMemory[tempIndex].setData_value(" ");
								}
								myMemory[s].setData_value(" "); // lastly wipe the VCB entry to " " value
								cout << endl;
							}
						}
					}
				}
			}
			if (choice == 3) { // [3] linked list
				for (unsigned int s = 0; s < size; s++) {
					if (myMemory[s].getBlock_index() == 0) {
						if (myMemory[s].getData_value() != " ") {
							// strip the line of the comma
							vector<string> result;
							string str = myMemory[s].getData_value();
							stringstream data(str);
							string line;
							while (getline(data, line, ',')) {
								result.push_back(line);
							}
							// end of delimitter
							if (stoi(result[0]) == stoi(csv_Vector[i][1])) {
								cout << "HERE" << endl;
								cout << "Deleting file " << csv_Vector[i][1] << " : ";
								int temp = stoi(result[1]);
								int tempHolder;
								myMemory[s].setData_value(" "); //wipe the VCB entry to " " value
								for (int i = stoi(result[1]); i < stoi(result[2]); i++) {
									// strip the line of the comma // Reason for another delimitter here is to read the [File entry][next index]
									vector<string> entryResult;
									string entryStr = myMemory[temp].getData_value();
									stringstream data(entryStr);
									string entryLine;
									while (getline(data, entryLine, ',')) {
										entryResult.push_back(entryLine);
									}
									// end of delimitter
									cout << entryResult[0];
									myMemory[temp].setData_value(" ");
									temp = stoi(entryResult[1]);
								}
								cout << endl;
							}
						}
					}
				}
			}
		}
	}
}