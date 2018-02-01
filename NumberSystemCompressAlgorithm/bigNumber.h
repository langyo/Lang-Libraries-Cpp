#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <utility>
#include <stdexcept>

#ifndef __ly__bigNumber__h__
#define __ly__bigNumber__h__

namespace ly{
	namespace bigNumber{
		using namespace std;
		
		struct bigIntUnsigned;
		struct divResultUnsigned;
		
		constexpr auto byte_size=8,max_byte=256;
		
		typedef unsigned char byte;
		typedef shared_ptr<array<bigIntUnsigned,max_byte>> cacheType;
		
		struct bigIntUnsigned{
			vector<byte> data;
			
			bigIntUnsigned &operator+=(const bigIntUnsigned &);
			bigIntUnsigned &operator-=(const bigIntUnsigned &);
			bigIntUnsigned &operator*=(const bigIntUnsigned &);
			bigIntUnsigned &operator/=(const bigIntUnsigned &);
			bigIntUnsigned &operator%=(const bigIntUnsigned &);
			
			friend bigIntUnsigned operator+(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator-(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator*(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator/(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator%(const bigIntUnsigned &,const bigIntUnsigned &);
			
			bigIntUnsigned &deleteUselessBytes();
			inline bool empty(){	return this->data.empty();	}
			
			friend divResultUnsigned div(bigIntUnsigned,const bigIntUnsigned &);
			
			bool operator<(const bigIntUnsigned &);
			bool operator>(const bigIntUnsigned &);
			bool operator==(const bigIntUnsigned &);
			bool operator<=(const bigIntUnsigned &);
			bool operator>=(const bigIntUnsigned &);
			bool operator!=(const bigIntUnsigned &);
			
			bigIntUnsigned &operator~();
			bigIntUnsigned &operator|=(const bigIntUnsigned &);
			bigIntUnsigned &operator^=(const bigIntUnsigned &);
			bigIntUnsigned &operator&=(const bigIntUnsigned &);
			bigIntUnsigned &operator<<=(long);
			bigIntUnsigned &operator>>=(long);
			
			friend bigIntUnsigned operator|(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator^(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator&(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator<<(const bigIntUnsigned &,const bigIntUnsigned &);
			friend bigIntUnsigned operator>>(const bigIntUnsigned &,const bigIntUnsigned &);
			
			explicit operator unsigned long long();
			explicit operator bool();
			
			bigIntUnsigned()=default;
			bigIntUnsigned(const bigIntUnsigned &)=default;
			bigIntUnsigned(unsigned long long);
			template<typename I> bigIntUnsigned(I beg,I end):data(beg,end){	}
			bigIntUnsigned(long length,byte input):data(length,input){	}
			
			bigIntUnsigned &operator=(const bigIntUnsigned &)=default;
			bigIntUnsigned &operator=(unsigned long long);
			
			~bigIntUnsigned()=default;
		};
		
		struct divResultUnsigned{
			bigIntUnsigned quotient;
			bigIntUnsigned surplus;
			cacheType divisionCache;
			
			template<typename I,typename I2>divResultUnsigned(I beg,I end,I2 beg2,I2 end2,cacheType cache):quotient(beg,end),surplus(beg2,end2),divisionCache(cache){	}
		};
		
		struct bigInt:public bigIntUnsigned{
			// 暂时放这里不写a.a...
		};
	}
	
	using bigNumber::bigIntUnsigned;
}

#endif