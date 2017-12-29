#include <array>
#include <utility>
#include <random>
#include <ctime>
#include <iostream>
#include <stdexcept>

#ifndef __LangCompressAlgorithm__lite__h__
#define __LangCompressAlgorithm__lite__h__

typedef unsigned char byte;
// 一个字节。
typedef long op_t;
// 在算法中参与运算的整数类型；如果要移植到低配置、低要求的机器上，可以将此类型改改。
	
const op_t length=256;
// 单批处理数据的长度；暂时不能更改。
const op_t system=128;
// 欲使用的进制；暂时不能更改。
const op_t byte_size=8;
// 一个字节有多少位，主要用于进行移位运算；不能更改。
const op_t flag_size=length/byte_size;
// 标志位长度，用于记录欲压缩的字节是否超出system；暂时不能更改。
const op_t max_byte=256;
// 一个字节能够表示多少种数字；不能更改。
const op_t max_loop=max_byte-2;
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
void encompress(package_t &in);
// 核心压缩算法。
void decompress(package_t &in);
// 核心解压算法。
data_t &random_data(bool use_time_seed=false);
// 测试用代码，用于生成一串随机数据以测试压缩；调试状态下，请保持参数为false；如果想测试大批量测试压缩性能，请开true以生成不同的数据。
// 我承认这随机数产生器是很蠢，但这是临时用来测试的，能用就行a.a。
#endif