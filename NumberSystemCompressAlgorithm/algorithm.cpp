#include <array>
#include <function>
#include <thread>
#include <mutex>
#include <utility>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include <stdexcept>

#ifndef __LangCompressAlgorithm__h__
#define __LangCompressAlgorithm__h__

namespace ly{
	typedef unsigned char byte;
	typedef long op_t;
	constexpr auto length=256;
	constexpr auto system=128;
	constexpr auto byte_size=sizeof(byte)*8;
	constexpr auto flag_size=length/byte_size;
	constexpr auto max_byte=256;
	constexpr auto max_loop=max_byte-2;
	typedef std::array<byte,length> data_t;
	typedef std::array<byte,flag_size> flag_t;
	struct package_t{
		data_t data;
		flag_t flag;
		op_t size;
		byte loop=0;
	};
	typedef std::vector<std::thread> threads_t;
	typedef std::function<void()> void_func;
	typedef std::vector<package_t> packages_t;
	struct step_lock{
		op_t step=0;
		std::mutex lock;
		op_t get(bool opposition=false){
			op_t m;
			lock.lock();
			m=step++;
			lock.unlock();
			return m;
		}
	};
	struct compresser{
		friend std::istream &operator>>(std::istream &is,compresser &n);
		friend std::ostream &operator<<(std::istream &os,compresser &n);
		
		packages_t p;
		op_t input_size,output_size;
		op_t core_thread_count=1,strain_thread_count=1,package_thread_count=std::thread::hardware_concurrency();
		bool is_encompress=true;
		
		compresser &encompress();
		compresser &decompress();
	private:
		void encompress(package_t &in);
		void decompress(package_t &in);
		
	public:
		compresser(bool will_encompress=true,op_t form=length,op_t to=length/2){}
	};
	data_t &random_data(bool use_time_seed=false);
}

#endif
