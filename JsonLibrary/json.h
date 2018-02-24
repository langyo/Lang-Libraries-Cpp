#ifndef LY_JSON_H
#define LY_JSON_H

namespace{
	namespace error_info{
		constexpr auto only_allow_object="无法对对象进行不属于对象的操作!"; 
		constexpr auto only_allow_array="无法对列表进行不属于列表的操作!"; 
		constexpr auto not_string="此结点的值不是字符串!";
		constexpr auto not_integer="此结点的值不是整数!";
		constexpr auto not_decimal="此结点的值不是小数!";
		constexpr auto not_boolean="此结点的值不是布尔值!";
		constexpr auto no_match="No match";
	};
	
	typedef std::map<std::string,node> object_t;
	typedef std::deque<node> array_t;
	
	enum type_t{
			object,array,string,integer,decimal,boolean,null
	};
	
	struct node{
		std::any data;
		type_t type;
		
		node():type(type_t::null){	} 
		node(const node &)=default;
		node(const std::string &n):type(type_t::string),data(n){	} 
		node(std::string &&n):type(type_t::string),data(std::move(n)){	}
		node(long long n):type(type_t::integer),data(n){	}
		node(long double n):type(type_t::decimal),data(n){	}
		node(bool n):type(type_t::boolean),data(n){	} 
		
		template <typename T> node &operator=(T);
		
		/* Operations */
		node &operator[](std::string);
		node &operator[](std::size_t);
		operator std::string();
		operator long long();
		operator long double();
		explicit operator bool();
		
		/* Functions for object and array. */
		template <typename T> bool &test(std::string);
		
		template <typename T> node &insert(std::string,T);
		template<typename T> node &insert(const std::pair<std::string,T> &);
		
		template <typename T> node &insert(T);
		
		node &erase(const std::string &);
		node &erase(long);
	};
	
	long filter_string_find(const std::string &,const char &,long);
	type_t judge_type(const std::string &);
	node parse_value(const std::string &);
	std::pair<std::string,node> parse_couple(const std::string &);
	node parse_block(const std::string &);
}

namespace ly{
	class json{
		
	}
}

#endif
