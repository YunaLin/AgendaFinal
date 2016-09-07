#include <vector>
#include <string>
#include <iostream>
using namespace std;

int main() {
	vector<string> test;
	test.push_back("1");
	test.push_back("2");
	test.push_back("3");
	vector<string>::iterator it = test.begin();
	vector<string>::iterator jit = it + 1;
	while (jit != test.end()) {
		cout << *jit << endl;
	}
	return 0;
}