#include "K.hpp"

int main(int argc,char**argv){
	std::string arg;
	for(int i=1;i!=argc;i++){
		if(*(argv[i])=='-'){
			if(arg!="")
				err_("You cannot specify more than 1 option");
			arg=std::string(argv[i]+1);
		}else{
			try{
				execfile(argv[i],{{"__arg__",str(arg)}});
			}catch(const std::runtime_error&e){
				std::cerr<<e.what()<<'\n';
				_exit(-1);
			}
			arg="";
		}
	}
	_exit(0);
}