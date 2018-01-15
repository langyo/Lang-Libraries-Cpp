#include "algorithm-lite.h"
#include <iostream>
#include <cassert>

using std::cout;
using std::cerr;
using std::endl;
using namespace ly::compresser;

int main(){
	cout<<"测试压缩数据:全部为F"<<endl;
	data_t testing={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	
	for(long t=1;;++t){
		cout<<"第 "<<t<<" 次压缩完成,结果为";
		encompress_core(testing);
		
		print_data(testing);
		cout<<endl;
		
		for(long i=7;i>3;++i){
			if(testing[i]!=0){
				continue;
			}
			break;
		}
	}
	
	return 0;
}
