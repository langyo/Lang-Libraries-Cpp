#include "algorithm.h"
using namespace ly;

int main(){
	cout<<"输入部分长度为256"<<endl;
	compresser c;
	c.p.push_back(random_data());
	try{
		c.encompress();
		cout<<"输出部分长度为"<<c.p.front().size<<endl;
		cout<<"输出部分循环次数为"<<c.p.front().loop<<endl;
	}catch(std::runtime_error e){
		cout<<"[错误]"<<e.what();
		return 1;
	}
	return 0;
}