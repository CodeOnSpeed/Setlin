struct codeOb{
	std::string type="code";
	unsigned refcnt=0;
	byco cont;
	codeOb(const byco&c):cont(c){}
};
Ob code(const byco&c){
	return new codeOb(c);
}