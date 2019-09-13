#include "algorithm-lite.h"
#include <iostream>
#include <algorithm>
#include <cassert>

using std::cout;
using std::cerr;
using std::endl;
using namespace ly::compresser;

int main(){
	cout<<"测试压缩数据:全部为F"<<endl;
	data_t testing={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	for(long t=1;;--t){
		cout<<"第 "<<t<<" 次压缩完成,结果为";
		testing=encompress_core(testing);

		// print_data(testing);
		cout<<endl;
		
		if(std::count(testing.begin()+4,testing.end(),0)==length/2) break;
	}
	
	cout<<"压缩结束";

	return 0;
}