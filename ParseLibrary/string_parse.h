#include <initializer_list>
#include <functional>

#ifndef _LY_PARSER_LIBRARY_
#define _LY_PARSER_LIBRARY_

namespace ly{
	using namespace std;
	
	bool check_char(const char &n){
		if( 'a' <= n && 'z' >= n || 'A' <= n && 'Z' >= n || '0' <= n && '9' >= n || '_' == n) return true;
		return false;
	}
	
	string round(const string &str, size_t begin, size_t end){
		return str.substr(begin,end-begin);
	}
	
	size_t through_special_find(
			const string &str,
			char finding,
			initializer_list<char> specials,
			size_t begin_pos = 0,
			initializer_list<char> escapers = {}){
		auto finding_pos = str.find(finding,begin_pos);
		decltype(specials.begin()) special_pos;
		
		while([&](){
			for(special_pos=specials.begin();special_pos != specials.end(); ++special_pos)
				if(str.find(*special_pos, begin_pos) < finding_pos) return true;
			return false;
		}){
			auto begin = str.find(*special_pos, begin_pos) + 1;
			auto end = str.find(*special_pos,begin);
			while([&](){
				for(auto &i : escapers) if(str[end-1] == i) return true;
				return false;
			}){
				begin = end + 1;
				end = str.find(*special_pos, begin);
			}
			begin_pos = end + 1;
			finding_pos = str.find(finding, begin_pos);
		}
		return finding_pos;
	}
	
	using namespace std::placeholders;
	const auto search_operation = bind(&through_special_find, _1, _2, {'\"', '\'', '/'}, _3, {'\\'});
}

#endif
