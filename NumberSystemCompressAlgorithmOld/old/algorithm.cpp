#ifndef __LangCompressAlgorithm__cpp__
#define __LangCompressAlgorithm__cpp__
#include "algorithm.h"
namespace ly{
	/*
		对于涉及多线程的函数的变量统一命名：
		step：			当前已分配到的任务号；
		step_lock：	分配任务对应的互斥体，以保证不会出现跳步骤的情况；
		sum_all：		这个函数过程的数据累加，由各个线程各自的sum累加得来；
		data_lock：	累加数据的互斥体，以保证不会出现数据冲突的情况。
	
		对于lambda内的变量统一命名：
		s：			step，当前线程正在处理的任务号；
		t：			temp，算数上的临时变量，用于累加/减的进/退位；
		i、j：		用于循环的计数；
		sum：	当前线程的数据累加，当线程发觉没有任务要做时，会将此再与外部的sum_all相加。
	*/
	
	void compresser::encompress(package_t &in){
		threads_t threads;
		op_t step=0;
		data_t sum_all;
		std::mutex data_locker,step_locker;
		
		// ——第一部分：净化数据——
		// 使得源输入数据符合压缩要求，能够进行压缩。
		static void_func strainer=[&](){
			op_t s=0,i,j;
			for(;s<flag_size;){
				assert(s<flag_size);
				{
					// 获取任务号。
					locker l(step_locker);
					s=step++;
				}
				// 对本线程处理范围内的数据进行检查，如果有超过system的字节，将此字节缩小至小于system并设置其对应标志位。
				for(i=byte_size*s,j=byte_size-1;j>=0;++i,--j){
					assert(in.data[i]!=0);
					if(in.data[i]>=system){
						in.data[i]-=system;
						in.flag[j]|=(1<<j);
					}
				}
			}
		};
		// 分配线程。
		for(op_t i=0;i<core_thread_count;++i) threads.emplace_back(strainer);
		for(auto &i:threads) i.join();
		
		// 清空线程。
		threads.clear();
		
		// ——第二部分：进制转换——
		// 将源数据的进制当作128进制，再向256进制转换。
		step=0;
		static void_func translater=[&](){
			data_t sum;
			op_t s=0,t,i,j=0;
			for(;s<length;){
				{
					locker l(step_locker);
					s=step++;
				}
				// 如果乘数为零就不用算了，直接过。
				if(in.data[s]==0) continue;
				// 乘法+进位加法。
				t=in.data[s]*system;
				i=s;
				do{
					t+=sum[i];
					sum[i]=static_cast<byte>(t);
					t>>=byte_size;
					++i;
				}while(t!=0);
				if(i>j) j=i;
			}
			t=0;
			{
				// 将此线程的累加结果加至总结果。
				locker l(data_locker);
				do{
					t+=(sum_all[i]+sum[i]);
					sum_all[i]=static_cast<byte>(t);
					t>>=byte_size;
					++i;
				}while((t!=0)||(i<=j));
			}
		};
		// 分配线程。
		for(op_t i=0;i<strain_thread_count;++i) threads.emplace_back(translater);
		for(auto &i:threads) i.join();
		
		// ——第三部分：刷新数据——
		// loop变量部分。
		++in.loop;
		if(in.loop>=max_loop) throw std::runtime_error("Too large loops");
		
		// size变量部分。
		op_t i=in.size;
		// 压缩后的数据末尾有多少零，就砍掉几个字节。
		for(;(i>=0)&&(sum_all[i]==0);--i);
		in.size=(i+flag_size);
		// 首先写入固定的flag_size。
		for(op_t j=0;j<flag_size;++j) in.data[j]=in.flag[j];
		// 然后将sum_all填入；别问我为什么不用std::copy。
		for(op_t j=flag_size,k=0;k<i;++j,++k) in.data[k]=sum_all[j];
		// 将末尾部分补零；如果实在想更快些可以不要这段，但不保证会不会出现数据错误。
		for(op_t j=i+flag_size;j<length;++j) in.data[j]=0;
	}

	void compresser::decompress(package_t &in){
		threads_t threads;
		op_t step;
		data_t sum_all;
		std::mutex data_locker,step_locker;
	
		// ——第一部分：取标志位——
		op_t i=0;
		for(;i<flag_size;++i){
			in.flag[i]=in.data[i];
		}
		
		// ——第二部分：进制转换——
		step=i;
		static void_func translater=[&](){
			data_t sum;
			op_t s,t=0,i;
			for(;s>=flag_size;){
				{
					locker l(step_locker);
					s=step--;
				}
				// 这里的并行除法算法是根据每一数位的除数与余数关系而写的。
				i=s;
				do{
					t<<=byte_size;
					t+=(in.data[i]+sum[i]);
					sum[i]=static_cast<byte>(t/system);
					t%=system;
					--i;
				}while((t!=0)&&(i>=flag_size));
			}
			// 将此线程的累加结果加至总结果。
			t=0;
			{
				locker l(data_locker);
				do{
					t+=(sum_all[i]+sum[i]);
					sum_all[i]=static_cast<byte>(t);
					t>>=byte_size;
					++i;
				}while((i<s)||(t!=0));
			}
		};
		// 分配线程。
		for(op_t i=0;i<core_thread_count;++i) threads.emplace_back(translater);
		for(auto &i:threads) i.join();
		// 存入结果。
		in.data=std::move(sum_all);
		
		// 清空线程。
		threads.clear();
		
		// ——第三部分：数据还原——
		step=0;
		static void_func strainer=[&](){
			op_t s,i,j;
			for(;s<flag_size;){
				{
					locker l(step_locker);
					s=step++;
				}
				// 根据原本flag_size里记录的结果来还原数据。
				for(i=byte_size*s,j=byte_size-1;j>=0;++i,--j){
					if((in.flag[s]&(1<<j))!=0) in.data[i]+=system;
				}
			}
		};
		// 分配线程
		for(op_t i=0;i<strain_thread_count;++i) threads.emplace_back(strainer);
		for(auto &i:threads) i.join();
		
		// ——第四部分：刷新数据——
		// loop变量部分。
		--in.loop;
		// size变量部分。
		for(i=length-1;(i>=0)&&(in.data[i]==0);--i);
		in.size=i;
	}
	
	compresser &compresser::encompress(){
		// 整体压缩算法，具体细节不用太过在意。
		threads_t threads;
		op_t step=0;
		std::mutex step_locker;
		static void_func encompresser=[&](){
			op_t s;
			for(;s<p.size();){
				{
					locker l(step_locker);
					s=step++;
				}
				do{
					encompress(p[s]);
				}while(p[s].size>output_size);
			}
		};
		for(op_t i=0;i<package_thread_count;++i) threads.emplace_back(encompresser);
		for(auto &i:threads) i.join();
		return *this;
	}
	
	compresser &compresser::decompress(){
		// 整体解压算法，具体细节不用太过在意。
		threads_t threads;
		op_t step=0;
		std::mutex step_locker;
		static void_func decompresser=[&](){
			op_t s;
			for(;s<p.size();){
				{
					locker l(step_locker);
					s=step++;
				}
				do{
					decompress(p[s]);
				}while(p[s].loop>0);
			}
		};
		for(op_t i=0;i<package_thread_count;++i) threads.emplace_back(decompresser);
		for(auto &i:threads) i.join();
		return *this;
	}
	
	std::istream &operator>>(std::istream &is,compresser &n){
		// 输入流重载。
		while(!is.eof()){
			n.p.emplace_back();
			n.p.back().size=n.input_size;
			if(!n.is_encompress){
				// 如果是解压的话，先读头部存储的已压缩次数。
				if(!is>>n.p.back().loop) throw std::runtime_error("Readed failed");
			}
			for(op_t i=0;i<n.input_size;++i){
				if(!is>>n.p.back().data[i]){
					if(is.fail()) throw std::runtime_error("Readed failed");
					break;
				}
			}
		}
		return is;
	}
	
	std::ostream &operator<<(std::ostream &os,compresser &n)noexcept{
		// 输出流重载。
		for(auto &i:n.p){
			// 如果为压缩后数据，先写入循环次数头。
			if(n.is_encompress) os<<i.loop;
			for(op_t j=0;j<n.output_size;++j){
				os<<i.data[j];
			}
		}
		return os;
	}
}
#endif