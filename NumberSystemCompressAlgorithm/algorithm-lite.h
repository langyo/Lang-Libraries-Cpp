#include <array>
#include <utility>

// 调试使用的库 
#include <cassert>
#include <iostream>

#ifndef __LangCompressAlgorithm__Lite__cpp__
#define __LangCompressAlgorithm__Lite__cpp__

namespace ly{
	namespace compresser{
		constexpr auto length=8;
		constexpr auto base=0x80;
		constexpr auto byte_size=8;
		
		typedef unsigned char byte;
		typedef std::array<byte,length> data_t; 
		
		using std::cout;
		using std::cerr;
		using std::endl;
		
		// 调试用代码，正式版会删除。 
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

		// 调试用代码，正式版会删除。
		inline void print_data(data_t &in){
			for(auto &i:in) print_hex(i);
			cout<<endl;
		}
		
		data_t encompress_core(data_t &n){
			data_t sum={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
			byte flag;
			
			// *设置Flag位*
			for(long i=0,k=length-1;i<length;++i,--k){
				if(n[i]>=base){
					n[i]-=base;
					flag|=(1<<k);
				}
			}
			
			// *乘法计算*
			// mulling为base n次方的临时存储变量。
			data_t mulling={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
			// k为被乘数下标。 
			for(auto k=n.begin();k!=n.end();++k){
				// mulled=mulling*n[k]
				// t为累加数字，用于实现进位。
				long t=0; 
				data_t mulled;
				for(auto i=mulling.begin(),j=mulled.begin();i!=mulling.end();++i,++j){
					t+=(*i)*(*k);
					(*j)=static_cast<byte>(t);
					t>>=byte_size;
				}
				
				// sum+=mulled
				// t为累加数字，用于实现进位。
				t=0;
				for(auto i=mulled.begin(),j=sum.begin();i!=mulled.end();++i,++j){
					t+=(*i)+(*j);
					(*j)=static_cast<byte>(t);
					t>>=byte_size;
				} 
				
				// mulling*=base
				// t为累加数字，用于实现进位。
				t=0;
				for(auto &i:mulling){
					t+=i*base;
					i=static_cast<byte>(t);
					t>>=byte_size;
				}  
			}
			
			// *将flag写入sum* 
			// 将sum的前7位向前移动，给flag腾出位置（大雾）。
			for(auto i=sum.rbegin(),j=++sum.rbegin();j!=sum.rend();(*i)=(*j));
			// 然后当然是把flag写进sum开头啦。
			sum.front()=flag;
			
			// 返回处理后的数据。 
			return sum;
		}
		
		data_t decompress_core(data_t &n){
			data_t sum;
			
			// *备份flag位*
			// （其实这也不是备份，而是创建一个引用……） 
			byte &flag=n.front();
			 
			// *除法运算*
			// k为被除数。 
			for(auto k=n.rbegin();k!=n.rend();++k){
				long t=0;
				for(auto j=n.rbegin();j!=n.rend();--j){
					t<<=byte_size;
					t+=(*j);
					(*j)=static_cast<byte>(t/base);
					t%=base;
				}
				(*k)=static_cast<byte>(t);
			}
			
			// *设置Flag位* 
			for(long i=0,k=length-1;i<length;++i,--k){
				if((sum.front()&(1<<k))!=0){
					sum[i]+=base;
				}
			}
			
			// 返回处理后的数据。
			return sum; 
		}
	}
}

#endif
