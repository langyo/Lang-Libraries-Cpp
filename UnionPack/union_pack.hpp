/*	联合容器类
	union container type 

	未使用任何协议。 
	Unlicense.
*/

#include <functional>
#include <stdexcept>

#ifndef _LY_UNION_PACK_
#define _LY_UNION_PACK_
 
namespace ly{
 	namespace union_pack{
 		using std::size_t;
 		using std::function;
 		using std::logic_error;
 		
 		template <size_t id,typename T,typename... Args>
 		struct union_element{
 			union{
 				T myself;
 				union_element<id+1,Args...> others;
			};
			
			size_t &disc;
			const function<void()> &destroy;
			
			void do_destroy(){
			    if(id==disc) myself;
			    else others.do_distroy();
			}
			
			template <T> bool is(){
			    if(id==disc) return true;
			    return false;
			}
			template <typename N> bool is(){    return others.is<N>();    }
			
			explicit operator T&(){ return myself;    }
			operator decltype(others) &(){    return others;    }
			
			union_element(size_t &d,const function<void()> &f):disc(d),destroy(f),others(d,f){}
			union_element(size_t &d,const function<void()> &f,const T &n):disc(d),destroy(f),myself(n),others(d,f){}
			template <typename N> union_element(size_t &d,const function<void()> &f,const N &n):disc(d),destroy(f),others(d,f,n){}
			union_element(const union_element &) = delete;
			
			T &operator=(const T &n){
				if(id!=disc) destroy();
				myself = n;
				disc = id;
				return myself;
			}
			template <typename N> N &operator=(const N &n){
			    return others = n;
			}
			union_element &operator=(const union_element &) = delete;
			
			~union_element(){   destroy();  }
		};
		
		template <size_t id,typename T>
 		struct union_element<id,T>{
 			T myself;
			
			size_t &disc;
			const function<void()> &destroy;
			
			void do_destroy(){
			    if(id==disc) myself;
			    else throw logic_error("Can't destroy this union pack");
			}
			
			template <T> bool is(){
			    if(id==disc) return true;
			    return false;
			}
			
			explicit operator T&(){ return myself;    }
			
			union_element(size_t &d,const function<void()> &f):disc(d),destroy(f){}
			union_element(size_t &d,const function<void()> &f,const T &n):disc(d),destroy(f),myself(n){}
			union_element(const union_element &) = delete;
			
			T &operator=(const T &n){
				if(id!=disc) destroy();
				myself = n;
				disc = id;
				return myself;
			}
			union_element &operator=(const union_element &) = delete;
			
			~union_element(){   destroy();  }
		};
 		
 		template <typename T,typename... Args>
 		class union_pack{
 			size_t disc;
 			union_element<0,T,Args...> elements;
 			function<void()> destroy = [&](){	elements.do_destroy();	};
		public:
			template <typename N> bool is(){	return elements.is<N>();	}
			template <typename N> bool is(const N &){	return this->is<N>();	}
			
			decltype(elements) &get(){return elements;	}
			template <typename N> N &get(){	return static_cast<N>(elements);	}
			
			template <typename N> decltype(elements) &set(const N &n){
				elements = n;
				return elements;
			}
			
			union_pack():elements(disc,destroy){ };
			template <typename N> union_pack(const N &n):elements(disc,destroy,n){	}
			union_pack(const union_pack &n):disc(n.disc),elements(disc,destroy,n){	}
			
			template <typename N> decltype(elements) &operator=(const N &n){	return this->set(n);	} 
			union_pack &operator=(const union_pack &)=default;
			
			~union_pack(){	destroy();	}
		};
	}
}
 
#endif 

