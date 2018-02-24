#ifndef LY_JSON_CPP
#define LY_JSON_CPP

namespace{
	template <typename T> node &node::operator=(T data){
		*this=node(data);
		return *this;
	} 
		
	node &node::operator[](std::string name){
		if(type!=type_t::object) throw std::logic_error(error_info::only_object);
		return std::any_cast<object_t>(data).at(name);
	}
	
	node &node::operator[](std::size_t id){
		if(type!=type_t::array) throw std::logic_error(error_info::only_array);
		return std::any_cast<array_t>(data).at(id);
	} 

	node::operator std::string(){
		if(type!=type_t::string) throw std::logic_error(error_info::not_string); 
		return std::any_cast<std::string>(data);
	} 

	node::operator long long(){
		if(type!=type_t::integer) throw std::logic_error(error_info::not_integer); 
		return static_cast<long long>(data);
	} 

	node::operator long double(){
		if(type!=type_t::decimal) throw std::logic_error(error_info::not_decimal); 
		return static_cast<long double>(data);
	} 

	node::operator bool(){
		if(type!=type_t::decimal) throw std::logic_error(error_info::not_boolean); 
		return static_cast<bool>(data);
	} 

	template <typename T> bool &node::test(std::string key){
		if(type!=type_t::object) return false;
		if(data->find(key)==data.end()) return false;
		return true;
	} 

	template <typename T> node &node::insert(std::string key,T value){ 
		switch(type){ 
			case type_t::null:
				data=object_t();
				type=type_t::object;
			case type_t::object:
				static_cast<object_t>(data).insert(std::make_pair(key,node(value)));
				break;
			default:
				throw std::logic_error(error_code::only_allow_object); 
		} 
		return *this; 
	}
		
	template <typename T> node &node::insert(T value){ 
		switch(type){ 
			case type_t::null:
				data=array_t();
				type=type_t::array;
			case type_t::array:
				static_cast<array_t>(data).push_back(node(value));
				break;
			default:
				throw std::logic_error(error_code::only_allow_array); 
		}
		return *this; 
	}
		
	template<typename T> node &node::insert(const std::pair<std::string,T>  &n){
		return this->insert(n.first,n.second);
	}
		
	node &node::erase(const std::string &name){
		switch(type){
			case type_t::object:
				static_cast<object_t>(data).erase(name);
				break;
			default:
				throw std::logic_error(error_code::only_allow_object);
		}
		return *this;
	}
		
	node &node::erase(long id){
		switch(type){
			case type_t::array:
				static_cast<array_t>(data).erase(name);
				break;
			default:
				throw std::logic_error(error_code::only_allow_array);
		}
		return *this;
	}
	
	long filter_string_find(const std::string &finded,const char &finding,long pos=0){
		for(;pos<finded.length();++pos){
			if(finded.at(pos)=="\""){
				for(;finded.at(pos)!="\"";++pos){
					if(finded.at(pos)=="\\") ++pos;
				}
			}
			if(finded.at(pos)==finding) return pos;
		}
		return -1;
	}
	
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
		long colon_pos=filter_string_find(text,":");
		return std::make_pair(text.substr(has_quotes?1:0,has_quotes?colon_pos-1:colon_pos),parse_value(text.substr(colon_pos+1,text.length())));
	}
	
	node parse_block(const std::string &text){
		if(text.at(0)=="{"&&text.at(text.length()-1)=="}"){
			node ret;
			for(long comma_pos=filter_string_find(text,","),last_comma_pos=1;comma_pos==-1;last_comma_pos=comma_pos+1,comma_pos=filter_string_find(text,",",last_comma_pos)){
				ret.insert(parse_couple(text.substr(last_comma_pos,comma_pos)));
			}
			return ret;
		}else if(text.at(0)=="["&&text.at(text.length()-1)=="]"){
			node ret;
			for(long comma_pos=filter_string_find(text,","),last_comma_pos=1;comma_pos==-1;last_comma_pos=comma_pos+1,comma_pos=filter_string_find(text,",",last_comma_pos)){
				ret.insert(text.substr(last_comma_pos,comma_pos));
			}
			return ret;
		}else throw std::logic_error(error_code::no_match);
	}
} 

#endif
