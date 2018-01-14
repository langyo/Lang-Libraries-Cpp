#include "algorithm-lite.h"
#include <iostream>
#include <cassert>

using std::cout;
using std::cerr;
using std::endl;
using namespace ly::compresser;

inline void print_hex(byte &in){
	long left=in>>4,right=in&0x0F;
	switch(left){
		case 0:cout<<"0";break;
		case 1:cout<<"1";break;
		case 2:cout<<"2";break;			
		case 3:cout<<"3";break;
		case 4:cout<<"4";break;
		case 5:cout<<"5";break;
		case 6:cout<<"6";break;
		case 7:cout<<"7";break;
		case 8:cout<<"8";break;
		case 9:cout<<"9";break;
		case 10:cout<<"A";break;
		case 11:cout<<"B";break;
		case 12:cout<<"C";break;
		case 13:cout<<"D";break;
		case 14:cout<<"E";break;
		case 15:cout<<"F";break;
	}
	switch(right){
		case 0:cout<<"0";break;
		case 1:cout<<"1";break;
		case 2:cout<<"2";break;			
		case 3:cout<<"3";break;
		case 4:cout<<"4";break;
		case 5:cout<<"5";break;
		case 6:cout<<"6";break;
		case 7:cout<<"7";break;
		case 8:cout<<"8";break;
		case 9:cout<<"9";break;
		case 10:cout<<"A";break;
		case 11:cout<<"B";break;
		case 12:cout<<"C";break;
		case 13:cout<<"D";break;
		case 14:cout<<"E";break;
		case 15:cout<<"F";break;
	}
	cout<<" ";
}

int main(){

	
	cout<<"测试压缩数据:全部为F"<<endl;
	data_t testing={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	
	for(long t=1;;++t){
		cout<<"第 "<<t<<" 次压缩完成,结果为";
		encompress_core(testing);
		
		for(auto &i:testing) print_hex(i);
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
