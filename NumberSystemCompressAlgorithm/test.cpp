#include "bigNumber.h"
#include <iostream>

using namespace std;
using namespace ly;

int main(){
	bigIntUnsigned a=2;
	for(auto &i:a.data) cout<<hex<<i;
	
	return 0;
}