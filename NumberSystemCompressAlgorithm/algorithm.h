#include <array>
#include <functional>
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
	// 一个字节。
	typedef unsigned long op_t;
	// 在算法中参与运算的整数类型；如果要移植到低配置、低要求的机器上，可以将此类型改改。
	
	constexpr auto length=256;
	// 单批处理数据的长度；暂时不能更改。
	constexpr auto system=128;
	// 欲使用的进制；暂时不能更改。
	constexpr auto byte_size=sizeof(byte)*8;
	// 一个字节有多少位，主要用于进行移位运算；不能更改。
	constexpr auto flag_size=length/byte_size;
	// 标志位长度，用于记录欲压缩的字节是否超出system；暂时不能更改。
	constexpr auto max_byte=256;
	// 一个字节能够表示多少种数字；不能更改。
	constexpr auto max_loop=max_byte-2;
	// 一个字节最多能够容纳多少次循环；不能更改。
	
	typedef std::array<byte,length> data_t;
	// 一块欲压缩数据/已压缩数据。
	typedef std::array<byte,flag_size> flag_t;
	// 一块欲压缩数据/已压缩数据的标志位块。
	struct package_t{
		// 一个数据包，包含一块数据块以及配套的标志位块，以及实际有效数据块大小和已压缩次数记录。
		data_t data;
		flag_t flag;
		op_t size;
		byte loop=0;
	};
	typedef std::vector<std::thread> threads_t;
	// 一堆线程的载体；注意，如果要添加线程，一定要使用emplace_back，因为线程不可复制构造！
	typedef std::function<void()> void_func;
	// 一个没有任何参数也没有任何返回值的function，用于存储给线程使用的lambda表达式。
	typedef std::vector<package_t> packages_t;
	// 一堆数据包。
	typedef std::lock_guard<std::mutex> locker;
	// 互斥体的RAII表达形式，具体的请查cppreference，你只需要知道这个是用来防止多线程下数据冲突的就够了。
	struct compresser{
		friend std::istream &operator>>(std::istream &is,compresser &n);
		// 输入流重载，可以与标准库的所有标准输入流对象配套使用。
		friend std::ostream &operator<<(std::istream &os,compresser &n)noexcept;
		// 输出流重载，可以与标准库的所有标准输出流对象配套使用。
		
		packages_t p;
		// 一堆数据包。
		op_t input_size,output_size;
		// input_size为原本每个数据块的有效大小，output_size为欲压缩/解压到的数据大小，这个是可变的。
		op_t core_thread_count=1,strain_thread_count=1,package_thread_count=std::thread::hardware_concurrency();
		// 每部分需要分配线程的地方的使用线程数量；如果你想让机器卡到爆炸，请尽管调大。
		bool is_encompress;
		// 这个类是否用于压缩，仅影响到输入输出流所读取/写出的数据，不影响核心代码的运作。
		
		compresser &encompress();
		// 总压缩算法。
		compresser &decompress();
		// 总解压算法。
	private:
		void encompress(package_t &in);
		// 核心压缩算法。
		void decompress(package_t &in);
		// 核心解压算法。
		
	public:
		compresser(bool will_encompress=true,op_t form=length,op_t to=length/2):is_encompress(will_encompress),input_size(form),output_size(to){}
		// 构造函数；默认为压缩状态。
	};
	data_t random_data();
	// 测试用代码，暂时废弃。
}
#include "algorithm.cpp"
#endif