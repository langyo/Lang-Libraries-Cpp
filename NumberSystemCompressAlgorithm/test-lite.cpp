#include "algorithm-lite.h"

int main(){
	cout<<"[信息]输入部分长度为256"<<endl;
	package_t p;
	p.data=random_data();
	long a=1;
	try{
		cout<<"[信息]第"<<a++<<"次压缩测试，";
		encompress(p);
		cout<<"输出部分长度为"<<p.size<<"，";
		cout<<"输出部分循环次数为"<<p.loop<<endl;
	}catch(std::runtime_error e){
		cout<<"[错误]"<<e.what();
		return 1;
	}
	return 0;
}
