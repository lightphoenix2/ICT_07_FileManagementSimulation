using namespace std;
#include <string>

class SingleEntry {
private:
	int physical_index;
	int block_index;
	string data_value;
public:
	SingleEntry(int, int, string);
	int getPhysical_index() const;
	int getBlock_index() const;
	string getData_value() const;
	void setData_value(string);
};