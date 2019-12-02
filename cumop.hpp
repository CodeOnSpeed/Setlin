struct cumopOb{
	std::string type="cumop";
	unsigned refcnt=0;
	byco cod;
	std::string a;
	std::string b;
	std::string op;
	cumopOb(const byco&c,const std::string&A,const std::string&B,const std::string&o):cod(c),a(A),b(B),op(o){}
};
Ob cumop(const byco&c,const std::string&a,const std::string&b,const std::string&o="__op__"){
	return new cumopOb(c,a,b,o);
}