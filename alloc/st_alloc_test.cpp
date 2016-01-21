#include "ST_Allocate.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
	int a[5] = {1,2,3,4,5};

	vector<int> allo_vec;

	for(int i = 0; i < 5; ++i ) {
		allo_vec.push_back(a[i]);
		cout<<allo_vec[i]<<"\t";
	}
	cout<<endl;

	return 0;
}