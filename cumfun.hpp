struct cumfunOb{
	std::string type="cumfun";
	unsigned refcnt=0;
	byco cod;
	std::string arg;
	cumfunOb(const byco&c,const std::string&a):cod(c),arg(a){}
};
Ob cumfun(const byco&c,const std::string&s){
	return new cumfunOb(c,s);
}