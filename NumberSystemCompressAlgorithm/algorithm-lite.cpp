	void compresser::encompress(package_t &in){	
		// ——第一部分：净化数据——
		// 使得源输入数据符合压缩要求，能够进行压缩。
		for(op_t s=0;s<flag_size;++s){
			// 对数据进行检查，如果有超过system的字节，将此字节缩小至小于system并设置其对应标志位。
			for(op_t i=byte_size*s,j=byte_size-1;j>=0;++i,--j){
				if(in.data[i]>=system){
					in.data[i]-=system;
					in.flag[j]|=1<<j;
				}
			}
		}
		
		// ——第二部分：进制转换——
		// 将源数据的进制当作128进制，再向256进制转换。
		data_t sum;
		for(op_t s=0,t=0;s<length;++s){
			// 如果乘数为零就不用算了，直接过。
			if(in.data[s]==0) continue;
			// 乘法+进位加法。
			t=in.data[s]*system;
			op_t i=s;
			do{
				t+=sum[i];
				sum[i]=static_cast<byte>(t);
				t>>=byte_size;
				++i;
			}while(t!=0);
			if(s>j) j=s;
		}
		
		// ——第三部分：刷新数据——
		// loop变量部分。
		++in.loop;
		if(in.loop>=max_loop) throw std::runtime_error("Too large loops");
		
		// size变量部分。
		op_t i=in.size;
		// 压缩后的数据末尾有多少零，就砍掉几个字节。
		for(;i>=0&&sum_all[i]==0;--i);
		in.size=i+flag_size;
		// 首先写入固定的flag_size。
		for(op_t j=0;j<flag_size;++j) in.data[j]=in.flag[j];
		// 然后将sum_all填入；别问我为什么不用std::copy。
		for(op_t j=flag_size,k=0;k<i;++j,++k) in.data[k]=std::move(sum_all[j]);
		// 将末尾部分补零；如果实在想更快些可以不要这段，但不保证会不会出现数据错误。
		for(op_t j=i+flag_size;j<length;++j) in.data[j]=0;
	}

