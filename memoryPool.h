#include <algorithm>
#include <vector>
#include <tuple>
#include <utility>
#include <memory>
#include <fstream>

#ifndef {langLibraries/memoryPool.h}
#define {langLibraries/memoryPool.h}

// Version 0.7
// Wrote by langyo.

namespace{
	// 默认的单元大小。最好取该机器最大能够表示的单次处理大小。
	using unit=std::size_t;
	// 默认分配unit的个数。
	constexpr auto defaultSize=1024;
	// 默认分配字节数。
	constexpr auto defaultBits=defaultSize*sizeof(unit);
	
	// 此为内存池中一个元素应该包含的各个结构。
	struct Node{
		// 引用计数指针。
		std::size_t *constPtr;
		// 数据块大小指针。
		std::size_t *sizePtr;
		// 数据块指针。
		std::size_t *ptr;
	};
	
	// 此为内存池中的一个区块，一个内存池由一个或多个区块组成。
	struct Block{
		// 默认分配器，替代new和delete。
		static std::allocator<unit> alloc;
		// [elements,cap)之间的数据为这个block所掌握的可用内存地址范围。
		// count用于对分配进的元素数量进行统计。
		// insideFreeLargestPtr指向这个block已被标记为回收的空闲空间中最大的一块，insideFreeLargestSize为其大小。
		// finallyFreePtr指向这个block未被分配的空间的开头，finallyFreeSize为其大小。
		std::size_t *elements,*cap,count,insideFreeLargestSize,finallySize,*insideFreeLargestPtr,*finallyFreePtr;
		// 注意，insideFreeLargestSize和finallyFreeSize的单位为unit个数。
		
		block(std::size_t size=defaultSize):elements(alloc.allocate(size)),cap(elements+size*sizeof(unit)),count(0),insideFreeLargestSize(0),finallyFreeSize(cap-elements),insideFreePtr(nullptr),finallyFreePtr(elements){ }
		~block(){ alloc.deallocate(elements,(cap-elements)/sizeof(unit)); }
		
		// 刷新该block中用于统计的变量，包括insideFreeLargestXXX与finallyFreeXXX。
		// 该成员函数以count为基准进行统计。
		void refresh(){
			// 合并相邻的已被回收的空闲空间的大小计数，同时寻找最大的已被回收的空闲空间。
			std::size_t *biggestFreePtr=nullptr;
		}
		
		std::size_t *allocate(std::size_t n){
			// 确定此block是否有空间可以分配，如果没有直接返回nullptr。
			if(insideFreeLargestSize<n && finallyFreeSize<n) return nullptr;
			// 先试图分配已被回收的空闲空间，再试图分配末尾剩余空间。
			if(insideFreeLargest>=n){
				//
				
			}
		}
	};
	
	static std::vector<Block> blocks;
}

namespace langyo{
	template <typename T> struct PoolAllocator{
		using value_type=T;
		PoolAllocator()=default;
		template <typename U> constexpr PoolAllocator(const PoolAllocator<U>&) noexcept {}
		T *allocate(std::size_t n){
			
		}
	};
}

#endif
