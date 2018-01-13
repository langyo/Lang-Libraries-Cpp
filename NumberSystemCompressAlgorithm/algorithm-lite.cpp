#ifndef __LangCompressAlgorithm__Lite__cpp__
#define __LangCompressAlgorithm__Lite__cpp__

namespace ly{
	namespace compresser{
		constexpr auto length=8;
		constexpr auto base=0x80;
		constexpr auto byte_size=8;
		
		typedef unsigned char byte;
		typedef std::array<byte,length> data_t;
		
		void encompress_core(data_t &n){
			data_t sum;
			for(long i=0,k=length-1;i<length;++i,--k) if((*i)>=base) (*i)-=base,sum[0]|=(1<<k);
			sum[1]=n[0];
			for(long k=base,i=0;i<length-1;k/=2,++i) for(long j=i,t=k;t!=0;t*=n[i],t+=sum[++i],sum[i]=static_cast<byte>(t),t>>=byte_size);
			n=std::move(sum);
		}
		
		void decompress_core(data_t &n){
			data_t sum;
			for(long k=length-1;k>=0;--k){
				for(long t=0,j=length-1;j>0;t<<=base_size,t+=n[j],n[j--]=static_cast<byte>(t/base),t%=base);
				sum[k]=static_cast<byte>(t);
			}
			for(long i=0,k=length-1;i<length;++i,--k) if((sum[0]&(1<<k))!=0) sum[i]+=base;
		}
	}
}