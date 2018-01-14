#include "algorithm-lite.h"
#include <iostream>
#include <cassert>

int main(){
	using std::cout;
	using std::endl;
	using namespace ly::compresser;
	
	cout<<"测试压缩数据:全部为F"<<endl;
	data_t testing={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	assert(testing[0]==0xFF);
	
	for(long t=1;;++t){
		cout<<"第 "<<t<<" 次压缩完成";
		encompress_core(testing);
		
		for(long i=7;i>=0;++i){
			if(testing[i]!=0){
				cout<<"不为0哦"<<endl;
				continue;
			}
		break;
		}
	}
	
	return 0;
}