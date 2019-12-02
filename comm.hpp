struct commOb{
	std::string type="comm";
	unsigned refcnt=0;
	std::vector<Ob> args;
	void(*fun)(const std::vector<Ob>&);
	commOb(std::vector<Ob>&&a,void(*f)(const std::vector<Ob>&)):args(a){
		fun=f;
	}
};
Ob comm(void(*f)(const std::vector<Ob>&)){
	return new commOb({},f);
}
void def(const std::vector<Ob>&v){
	switch(v.size()){
		case 3:
		if(v.front().type()!="var"||v[1].type()!="var")
			err("Variable exspected in def-statement");
		assignvar(cumfun(((codeOb*)v.back())->cont,((strOb*)v[1])->cont),v.front());
		return;
		case 4:
		if(v.front().type()!="var"||v[1].type()!="var"||v[2].type()!="var")
			err("Variable exspected in def-statement");
		assignvar(cumop(((codeOb*)v.back())->cont,((strOb*)v[1])->cont,((strOb*)v[2])->cont),v.front());
		return;
		case 5:
		if(v.front().type()!="var"||v[1].type()!="var"||v[2].type()!="var"||v[3].type()!="var")
			err("Variable exspected in def-statement");
		assignvar(cumop(((codeOb*)v.back())->cont,((strOb*)v[1])->cont,((strOb*)v[2])->cont,((strOb*)v[3])->cont),v.front());
		return;
		default:
		err("Wrong use of def");
	}
}
void While(const std::vector<Ob>&v){
	if(v.size()!=2)
		err("Wrong use of while");
	if(v.front().type()!="code")
		err("Syntax error: Did you forget []?");
	while(bOOl(eval(((codeOb*)v.front())->cont,{})))
		eval(((codeOb*)v.back())->cont,{});
}
void If(const std::vector<Ob>&v){
	if(v.size()>3)
		err("Wrong use of if");
	if(v[1].type()!="code")
		err("Syntax error: Did you forget []?");
	if(bOOl(v.front())){
		eval(((codeOb*)v[1])->cont,{});
	}else{
		if(v.size()==3)
			eval(((codeOb*)v.back())->cont,{});
	}
}