#include "bigNumber.h"

#ifndef __ly__bigNumber__cpp__
#define __ly__bigNumber__cpp__

namespace ly{
	namespace bigNumber{
		bigIntUnsigned &bigIntUnsigned::operator+=(const bigIntUnsigned &n){
			auto &lhs=this->data;
			auto &rhs=n.data;
			
			lhs.resize(max(lhs.size(),rhs.size())+1);
			
			long t=0;
			auto i=lhs.begin();
			for(auto j=rhs.cbegin();i!=lhs.end()&&j!=rhs.cend();++i,++j){
				t+=(*i)+(*j);
				(*i)=static_cast<byte>(t);
				t>>=byte_size;
			}
			for(;i!=lhs.end()&&t!=0;++i){
				t+=(*i);
				(*i)=static_cast<byte>(t);
				t>>=byte_size;
			}
			if(t>0) throw logic_error("大数加法运算时错误:临时变量溢出。");
			this->deleteUselessBytes();
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator-=(const bigIntUnsigned &n){
			if((*this)<n) throw invalid_argument("大数减法检查时错误:无符号下被减数比减数小。");
			auto &lhs=this->data;
			auto &rhs=n.data;
			
			long t=0;
			auto i=lhs.begin();
			for(auto j=rhs.cbegin();i!=lhs.end()&&j!=rhs.cend();++i,++j){
				t+=(*i)-(*j);
				(*i)=static_cast<byte>(t<0?(t+max_byte):t);
			}
			for(;i!=lhs.end()&&t<0;++i){
				t+=(*i);
				(*i)=static_cast<byte>(t<0?(t+max_byte):t);
			}
			if(t<0) throw logic_error("大数减法运算时错误:临时变量溢出。");
			this->deleteUselessBytes();
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator*=(const bigIntUnsigned &n){
			this->data=((*this)*n).data;
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator/=(const bigIntUnsigned &n){
			this->data=div(*this,n).quotient.data;
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator%=(const bigIntUnsigned &n){
			this->data=div(*this,n).surplus.data;
			return *this;
		}
		
		bigIntUnsigned operator+(bigIntUnsigned l,const bigIntUnsigned &r){
			return l+=r;
		}
		
		bigIntUnsigned operator-(bigIntUnsigned l,const bigIntUnsigned &r){
			return l-=r;
		}
		
		bigIntUnsigned operator*(const bigIntUnsigned &l,const bigIntUnsigned &r){
			auto &lhs=l.data,&rhs=r.data;
			
			bigIntUnsigned ret;
			long pos=0;
			for(auto i=lhs.cbegin();i!=lhs.cend();++i,++pos){
				bigIntUnsigned mulling(pos,0);
				long t=0;
				for(auto j=rhs.cbegin();j!=rhs.cend();++j){
					t+=(*i)+(*j);
					mulling.data.push_back(static_cast<byte>(t));
					t>>=byte_size;
				}
				while(t>0){
					mulling.data.push_back(static_cast<byte>(t));
					t>>=byte_size;
				}
				ret+=mulling;
			}
			return ret;
		}
		
		bigIntUnsigned operator/(bigIntUnsigned l,const bigIntUnsigned &r){
			return div(l,r).quotient;
		}
		
		bigIntUnsigned operator%(bigIntUnsigned l,const bigIntUnsigned &r){
			return div(l,r).surplus;
		}
		
		divResultUnsigned div(bigIntUnsigned l,const bigIntUnsigned &r,cacheType cache=nullptr){
			if(r.empty()) throw invalid_argument("大数除法检查时错误:除数为零。");
			if(l.empty()){
				divResultUnsigned ret(l.begin(),l.end());
				return ret;
			};
			auto &lhs=this->data,&rhs=r.data;
			
			vector<byte> sum;
			
			for(auto i=lhs.rbegin(),beginPos=i;i!=lhs.rend();++i){
				long left=0,right=max_byte-1;
				bigIntUnsigned k(beginPos,i);
				while(left!=right){
					long middle=(left+right)/2;
					if(cache->at(middle).data.empty()) cache->at(middle)l.data=(r*static_cast<bigIntUnsigned>(middle)).data;
					if(k<cache->at(middle)) right=middle;
					else left=middle;
				}
				k-=cache->at(left);
				copy(k.data.rbegin(),k.data.rend(),beginPos);
				for(;(*beginPos)==0;++beginPos);
				sum.push_back(static_cast<byte>(left);
			}
			
			this->deleteUselessBytes();
			divResultUnsigned ret(sum.rbegin(),sum.rend(),lhs.begin(),lhs.end(),cache);
			
			return ret;
		}
		
		bigIntUnsigned &bigIntUnsigned::deleteUselessBytes(){
			while(!this->data.empty() && this->data.back()==0) this->data.pop_back();
			return *this;
		}
		
		bool bigIntUnsigned::operator<(bigIntUnsigned &n){
			this->deleteUselessBytes(),n.deleteUselessBytes();
			auto &lhs=this->data,&rhs=n.data;
			if(lhs.size()<rhs.size()) return true;
			if(lhs.size()>rhs.size()) return false;
			for(auto i=lhs.rbegin(),j=rhs.rbegin();i!=lhs.rend();++i,++j) if((*i)<(*j)) return true;
			return false;
		}
		
		inline bool bigIntUnsigned::operator>(const bigIntUnsigned &n){
			return !(*this)<=n;
		}
		
		bool bigIntUnsigned::operator==(const bigIntUnsigned &n){
			auto &lhs=this->data,&rhs=n.data;
			if(lhs.size()!=rhs.size()) return false;
			for(auto i=lhs.begin(),j=rhs.begin();i!=lhs.end();++i,++j) if((*i)!=(*j)) return false;
			return true;
		}
		
		inline bool bigIntUnsigned::operator<=(const bigIntUnsigned &n){
			return (*this)<n||(*this)==n;
		}
		
		inline bool bigIntUnsigned::operator>=(const bigIntUnsigned &n){
			return (*this)>n||(*this)==n;
		}
		
		inline bool bigIntUnsigned::operator!=(const bigIntUnsigned &n){
			return !(*this)==n;
		}
		
		bigIntUnsigned bigIntUnsigned::operator~(){
			for(auto *i:this->data) i=~i;
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator|=(const bigIntUnsigned &n){
			auto &lhs=this->data,&rhs=n.data;
			auto j=rhs.cbegin();
			for(auto i=lhs.begin();i!=lhs.end()&&j!=rhs.cend();++i,++j) (*i)|=(*j);
			for(;j!=rhs.end();lhs.push_back((*j)++));
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator^=(const bigIntUnsigned &n){
			auto &lhs=this->data,&rhs=n.data;
			auto j=rhs.cbegin();
			for(auto i=lhs.begin();i!=lhs.end()&&j!=rhs.cend();++i,++j) (*i)^=(*j);
			for(;j!=rhs.end();lhs.push_back((*j)++));
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator&=(const bigIntUnsigned &n){
			auto &lhs=this->data,&rhs=n.data;
			auto j=rhs.cbegin();
			for(auto i=lhs.begin();i!=lhs.end()&&j!=rhs.cend();++i,++j) (*i)&=(*j);
			for(;j!=rhs.end();lhs.push_back((*j)++));
			return *this;
		}
		
		bigIntUnsigned operator<<(const bigIntUnsigned &n,long pos){
			// 这里的左移相当于每移一位乘以2。
			long subPos=pos%byte_size,mainPos=pos/byte_size+subPos==0?0:1;
			bigIntUnsigned ret(mainPos,0);
			auto &bytes=ret.data;
			unsigned long t=0;
			for(auto &i:n.data){
				t|=i<<subPos;
				bytes.push_back(static_cast<byte>(t));
				t>>=byte_size;
			}
			if(t!=0) bytes.push_back(static_cast<byte>(t));
			return ret.deleteUselessBytes();
		}
		
		bigIntUnsigned operator>>(const bigIntUnsigned &n,long pos){
			// 这里的右移相当于每移一位除以2。
			long subPos=pos%byte_size,mainPos=pos/byte_size+subPos==0?0:1;
			bigIntUnsigned ret;
			auto &bytes=ret.data;
			
			auto i=n.data.begin();
			for(;mainPos>0;--mainPos,++i);
			unsigned long t=(*(i++))>>subPos;
			for(;i!=n.data.end();++i){
				t|=(*i)<<subPos;
				bytes.push_back(static_cast<byte>(t));
				t>>=byte_size;
			}
			bytes.push_back(static_cast<byte>(t));
			return ret.deleteUselessBytes();
		}
		
		inline bigIntUnsigned operator|(bigIntUnsigned l,const bigIntUnsigned &r){
			return l|=r;
		}
		
		inline bigIntUnsigned operator^(bigIntUnsigned l,const bigIntUnsigned &r){
			return l^=r;
		}
		
		inline bigIntUnsigned operator&(bigIntUnsigned l,const bigIntUnsigned &r){
			return l&=r;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator<<=(long pos){
			this->data=((*this)<<pos).data;
			return *this;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator>>=(long pos){
			this->data=((*this)>>pos).data;
			return *this;
		}
		
		explicit bigIntUnsigned::operator unsigned long long(){
			constexpr size=sizeof(unsigned long long);
			unsigned long long ret=0;
			auto i=this->data.cbegin();
			for(long t=0;t<data && i!=this->data.cend();ret|=(*i)<<(t*byte_size));
			return ret;
		}
		
		explicit bigIntUnsigned::operator bool(){
			this->deleteUselessBytes();
			if(this->data.empty()) return true;
			return false;
		}
		
		bigIntUnsigned::bigIntUnsigned(unsigned long long n){
			constexpr size=sizeof(unsigned long long);
			while(n!=0) this->data.push_back(static_cast<byte>(n)),n>>=byte_size;
		}
		
		bigIntUnsigned &bigIntUnsigned::operator=(unsigned long long n){
			this->data=static_cast<bigIntUnsigned>(n).data;
			return *this;
		}
	}
}

#endif