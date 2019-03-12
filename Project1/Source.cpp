#include <string>
#include <iostream>
#include <vector>

using namespace std;
// Constant
const int MAX_ENTRIES = 128;

int main() {
	int memory[MAX_ENTRIES], block[MAX_ENTRIES], data[MAX_ENTRIES];	// creates an array of 128 elements to simulated 128 entries. [Index/rows]
	//vector<vector <int>> mem; // creates a 2D vector to hold Block and File data.[block/columns][File data]
	int choice, blockSize, numOfBlocks;
	cout << "Enter size of number of block size: ";	 //get the size per data block
	cin >> blockSize;
	
	int blockCounter = 0;
	for (int i = 1; i <= MAX_ENTRIES; i++) {	// inserts the value such that [0]... [128]
		memory[i-1] = i-1;
		if (i%blockSize) {
			block[i-1] = blockCounter;
		}
		else {
			block[i-1] = blockCounter;
			blockCounter++;
		}
	}

	cout << endl << "Sample example of format" << endl;
	for (int i = 0; i < MAX_ENTRIES; i++) {
		cout << "Index: " << memory[i] << "\t Block: " << block[i] << "\t File data" << endl;
	}

	system("pause");
	return 0;
}