// 演示如何跳出深层循环(这种方法很玄)

// 警告：此程序只用于演示，不能正确执行！
int main(){
	
	// 两层循环体的跳转
	// 进入循环
	[](){
		while(true){
			// 第一层循环体
			while(true){
				// 第二层循环体
				return; //跳出整个循环体
			}
			return; //跳出整个循环体，这里也可用break
		}
	}();
	
	// 多层循环体的向外跳转
	// 进入循环
	[](){
		while(true){
			// 第一层循环体
			while(true){
				// 第二层循环体
					while(true){
						// 第三层循环体
						return; //跳出整个循环体
					}
				}
				return; //跳出整个循环体
			}
			return; //跳出整个循环体，这里也可用break
		}
	}();
	
	return 0;
}
