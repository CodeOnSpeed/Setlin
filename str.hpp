struct strOb{
	std::string type="str";
	unsigned refcnt=0;
	std::string cont;
	strOb(const std::string&t,const std::string&c):type(t),cont(c){}
};
Ob str(const std::string&s){
	return new strOb("str",s);
}
strOb varstr("var","str");
CARE(varstr)
strOb strempset("str","{}");
CARE(strempset)