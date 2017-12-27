#ifndef __LangCompressAlgorithm__cpp__
#define __LangCompressAlgorithm__cpp__

void ly::compresser::encompress(package_t &in){
	threads_t threads;
	step_locker locker;
	data_t sum_all;
	std::mutex data_locker;
	
	static void_func strainer=[&](){
		op_t s,i,j;
		for(;;){
			s=locker.get();
			if(s>=flag_size) break;
			for(i=byte_size*s,j=byte_size-1;j>=0;++i,--j){
				if(in.data[i]>=system){
					in.data[i]-=system;
					in.flag[j]|=1<<j;
				}
			}
		}
	};
	for(op_t i=0;i<core_thread_count;++i) threads.emplace_back(strainer);
	for(auto &i:threads) i.join();
	
	threads.empty();
	
	locker.step=0;
	static void_func translater=[&](){
		data_t sum;
		op_t s,t,i,j;
		for(;;){
			s=locker.get();
			if(s>=length) break;
			if(in[s]==0) continue;
			t=in[s]*system;
			i=s;
			do{
				t+=sum[i];
				sum[i]=static_cast<byte>(t);
				t>>=byte_size;
				++i;
			}while(t!=0);
			if(s>j) j=s;
		}
		t=0;
		data_locker.lock();
		do{
			t+=sum_all[i]+sum[i];
			sum_all[i]=static_cast<byte>(t);
			t>>=byte_size;
			++i;
		}while(t!=0||i<j);
		data_locker.unlock();
	};
	for(op_t i=0;i<strain_thread_count;++i) threads.emplace_back(translater);
	for(auto &i:threads) i.join();
	in.data=sum_all;
	
	++in.loop;
	if(in.loop>=max_loop) throw std::runtime_error("Too large loops");
	
	op_t i=in.size;
	for(;i>=0&&in.data[i]==0;--i);
	++i;
	for(op_t j=0;j<flag_size;in.data[j++]=in.flag[i++]);
	for(;i>=0&&in.data[i]==0;--i);
	in.size=i;
}

void ly::compresser::decompress(package_t &in){
	threads_t threads;
	step_lock locker;
	data_t sum_all;
	std::mutex data_locker;
	
	op_t i=in.size-flag_size;
	for(op_t j=0;j<flag_size;++i,++j){
		in.flag[j]=in.data[i];
		in.data[i]=0;
	}
	
	locker.size=i;
	static void_func translater=[&](){
		data_t sum;
		op_t s,t=0,i;
		for(;;){
			s=locker.get(true);
			if(s<0) break;
			i=s;
			do{
				t<<=byte_size;
				t+=in.data[i]+sum[i];
				sum[i]=static_cast<byte>(t/system);
				t%=system;
				--i;
			}while(t!=0);
		}
		t=0;
		data_locker.lock();
		do{
			t+=sum_all[i]+sum[i];
			sum_all[i]=static_cast<byte>(t);
			t>>=byte_size;
			++i;
		}while(t<s||t!=0);
		data_locker.unlock();
	};
	for(op_t i=0;i<core_thread_count;++i) threads.emplace_back(translater);
	for(auto &i:threads) i.join();
	in.data=sum_all;
	
	threads.empty();
	
	locker.step=0;
	static void_func strainer=[&](){
		op_t s,i,j;
		for(;;){
			s=locker.get();
			if(s>=flag_size) break;
			for(i=byte_size*s,j=byte_size-1;j>=0;++i,--j){
				if(in.flag[s]&(1<<j)!=0) in.data[i]+=system;
			}
		}
	};
	for(op_t i=0;i<strain_thread_count;++i) threads.emplace_back(strainer);
	for(auto &i:threads) i.join();
	
	--in.loop;
	for(i=length-1;i>=0&&in.data[i]==0;--i);
	in.size=i;
}

compresser &ly::compresser::encompress(){
	threads_t threads;
	step_lock locker;
	static void_func encompresser=[&](){
		op_t s;
		for(;;){
			s=locker.get();
			if(s>=p.size()) break;
			do{
				encompress(p[s]);
			}while(p[s].size>output_size);
		}
	};
	for(op_t i=0;i<package_thread_count;++i) threads.emplace_back(encompresser);
	for(auto &i:threads) i.join();
	return *this;
}

compresser &ly::compresser::decompress(){
	threads_t threads;
	step_lock locker;
	static void_func decompresser=[&](){
		op_t s;
		for(;;){
			s=locker.get();
			if(s>=p.size()) break;
			do{
				decompress(p[s]);
			}while(p[s].loop>0);
		}
	};
	for(op_t i=0;i<package_thread_count;++i) threads.emplace_back(decompresser);
	for(auto &i:threads) i.join();
	return *this;
}

std::istream &ly::compresser::operator>>(std::istream &is,compresser &n){
	while(!is.eof()){
		n.p.push_back();
		n.p.back().size=input_size;
		if(!is_encompress){
			if(!is>>n.p.back().loop) throw std::runtime_error("Readed failed");
		}
		for(op_t i=0;i<input_size;++i){
			if(!is>>n.p.back().data[i]){
				if(is.fail()) throw std:runtime_error("Readed failed");
				break;
			}
		}
	}
	return is;
}

std::ostream &ly::compresser::operator<<(std::ostream &os,compresser &n)noexcept{
	for(auto i=n.p.begin();i!=n.p.end();++i){
		if(is_encompress) os<<(*i).loop;
		for(op_t j=0;j<output_size;++j){
			os<<(*i)[j];
		}
	}
	return os;
}
#endif