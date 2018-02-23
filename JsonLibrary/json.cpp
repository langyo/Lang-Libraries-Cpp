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
	}
	
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

		template <typename T> node &operator=(T data){
			*this=node(data);
			return *this;
		} 
		
		/* Operations */
		
		node &operator[](std::string name){
			if(type!=type_t::object) throw std::logic_error(error_info::only_object);
			return (*data)[name];
		}
		node &operator[](std::size_t id){
			if(type!=type_t::array) throw std::logic_error(error_info::only_array);
			return data->at(id);
		} 

		operator std::string(){
			if(type!=type_t::string) throw std::logic_error(error_info::not_string); 
			return data;
		} 

		operator long long(){
			if(type!=type_t::integer) throw std::logic_error(error_info::not_integer); 
			return data;
		} 

		operator long double(){
			if(type!=type_t::decimal) throw std::logic_error(error_info::not_decimal); 
			return data;
		} 

		explicit operator bool(){
			if(type!=type_t::decimal) throw std::logic_error(error_info::not_boolean); 
			return data;
		} 

		/* Functions for object and array. */
		
		template <typename T> bool &test(std::string key){
			if(type!=type_t::object) return false;
			if(data->find(key)==data.end()) return false;
			return true;
		} 

		template <typename T> node &insert(std::string key,T value){ 
			switch(type){ 
				case type_t::null:
					data=object_t();
				case type_t::object:
					static_cast<object_t>(data).insert(std::make_pair(key,node(value)));
					break;
				default:
					throw std::logic_error(error_code::only_allow_object); 
			} 
			return *this; 
		}
		
		template <typename T> node &insert(T value){ 
			switch(type){ 
				case type_t::null:
					data=array_t();
				case type_t::array:
					static_cast<array_t>(data).push_back(node(value));
					break;
				default:
					throw std::logic_error(error_code::only_allow_array); 
			}
			return *this; 
		}
		
		node &erase(const std::string &name){
			switch(type){
				case type_t::object:
					static_cast<object_t>(data).erase(name);
					break;
				default:
					throw std::logic_error(error_code::only_allow_object);
			}
			return *this;
		}
		
		node &erase(long id){
			switch(type){
				case type_t::array:
					static_cast<array_t>(data).erase(name);
					break;
				default:
					throw std::logic_error(error_code::only_allow_array);
			}
			return *this;
		}
	};
	
	type_t judge_type(const std::string &text){
		// Test if it's an object.
		if(text.at(0)=="{"&&text.at(text.length())=="}") return type_t::object;
		// Test if it's an array.
		if(text.at(0)=="["&&text.at(text.length())=="]") return type_t::array;
		// Test if it's a string.
		if(text.at(0)=="\""&&text.at(text.length())=="\"") return type_t::string;
		// Test if it's an integer.
		bool is_integer=true;
		for(auto &i:text){
			if(!(i>="0" && i<="9" || i=="-")){
				is_integer=false;
				break;
			}
		}
		if(is_integer) return type_t::integer;
		// Test if it's a decimal.
		bool is_decimal=true;
		for(auto &i:text){
			if(!(i>="0" && i<="9" || i=="." || i=="-")){
				is_decimal=false;
				break;
			}
		}
		if(is_decimal) return type_t::decimal;
		// Test if it's a boolean.
		if(text=="true" || text=="false") return type_t::boolean;
		// Test if it's a null value.
		if(text=="null") return type_t::null;
		
		throw std::logic_error(error_code::no_match);
	}
	
	node parse_value(const std::string &text){
		struct(judge_type(text)){
			case type_t::object:
			case type_t::array:
				return std::move(parse_block(text));
			case type_t::string:
				return node(text.substr(1,text.length-1));
			case type_t::integer:
				return node(text.stoll());
			case type_t::decimal:
				return node(text.stold());
			case type_t::boolean:
				return node(text=="true");
			case type_t::null:
				return node();
			default:
				throw std::logic_error(error_info::no_match);
		}
	}
	
	std::pair<std::string,node> parse_couple(const std::string &text){
		bool has_quotes=text.at(0)=="\""&&text.at(text.find(":",text.find("\"",1))-1)=="\"");
		long colon_pos;
		
		/* Get the colon's pos. */
	 	if(has_quotes){
			for(colon_pos=text.find("\"",colon_pos);text.at(colon_pos-1)=="\"");colon_pos=text.find("\"",colon_pos));
		}else{
			colon_pos=text.find(":");
		}
		
		/* Make the node. */
		std::pair<std::string,node> ret;
		
		ret.first=text.substr(has_quotes?1:0,has_quotes?colon_pos-1:colon_pos);
		
		auto value=text.substr(colon_pos,text.length());
		if(value.at(0)=="\""&&value.at(value.length())=="\"") ret.second=value.substr(1,value.length()-1);
		else if([&]->bool{for(auto i=0;i<text.length();++i) if(text.at(i)}
		
		
	}
	
	node &parse_block(const std::string &text){
		
	}
} 

#endif
