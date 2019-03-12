#include"SingleEntry.h"
#include <iostream>
#include <vector>
using namespace std;


SingleEntry::SingleEntry(int index, int b_index, string d_value) {
	physical_index = index;
	block_index = b_index;
	data_value = d_value;
}

int SingleEntry::getPhysical_index() const {
	return physical_index;
}

int SingleEntry::getBlock_index() const {
	return block_index;
}

string SingleEntry::getData_value() const {
	return data_value;
}