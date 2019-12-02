Ob biop(Ob(*f)(Ob,Ob)){
	return new biopOb(f);
}
Ob tilde(Ob a,Ob b){
	if(a.type()==((strOb*)b)->cont)
		return a;
	std::string traba;
	Ob re;
	try{
		Ob f=getvar(a.type()+" ~"+((strOb*)b)->cont);
		if(f.type()=="bifun")
			re=callbifun(f,a);
		else if(f.type()=="cumfun")
			re=callcumfun(f,a);
		else
			err("Can't call "+f.type()+" objects");
		if(re.type()!=((strOb*)b)->cont)
			err("Function didn't return "+((strOb*)b)->cont+", but "+re.type());
		return re;
	}catch(const std::runtime_error&e){
		if(e.what()!="Unknown variable "+a.type()+" ~"+((strOb*)b)->cont+"[]")
			traba="\n'"+a.type()+" ~"+((strOb*)b)->cont+"' failed: "+e.what();
		if(forced.count(a.type()+" ~"+((strOb*)b)->cont))
			err("Can't convert "+a.type()+" to "+((strOb*)b)->cont+'['+indent(traba)+" (forced)]");
	}
	try{
		Ob f=getvar('~'+((strOb*)b)->cont);
		if(f.type()=="bifun")
			re=callbifun(f,a);
		else if(f.type()=="cumfun")
			re=callcumfun(f,a);
		else
			err("Can't call "+f.type()+" objects");
		if(re.type()!=((strOb*)b)->cont)
			err("Function didn't return "+((strOb*)b)->cont);
		return re;
	}catch(const std::runtime_error&e){
		if(e.what()!="Unknown variable ~"+((strOb*)b)->cont+"[]")
			traba="\n'~"+((strOb*)b)->cont+"' failed: "+e.what();
	}
	err("Can't convert "+a.type()+" to "+((strOb*)b)->cont+'['+indent(traba)+']');
}
Ob equal(Ob a,Ob b){
	if(a.type()!=b.type()){
		return &False;
	}
	if(setfree.count(a.type()))
		err("No = defined for "+a.type()+" and "+b.type());
	if(((setOb*)a)->cont.size()!=((setOb*)b)->cont.size())
		return &False;
	return oper(oper(a,b,">="),oper(b,a,">="),"&");
}
Ob callbifun(Ob a,Ob b){
	return ((bifunOb*)a)->cont(b);
}
Ob newl(Ob a,Ob b){
	if(a.type()=="comm")
		err("Error above");
	return b;
}
Ob assignvar(Ob a,Ob b){
	vars.back()[((strOb*)b)->cont]=a;
	return b;
}
Ob comma(Ob a,Ob b){
	insert(Stack->back(),a);
	return b;
}
Ob strequal(Ob a,Ob b){
	Ob re=bOOl(((strOb*)a)->cont==((strOb*)b)->cont);
	return re;
}
Ob Or(Ob a,Ob b){
	if(setfree.count(a.type())||setfree.count(b.type()))
		err("No | defined for "+a.type()+" and "+b.type());
	std::unordered_set<Ob> re=((setOb*)a)->cont;
	for(Ob o:((setOb*)b)->cont)
		re.insert(o);
	return new setOb(a.type(),std::move(re));
}
Ob vartilde(Ob a,Ob b){
	return oper(getvar(((strOb*)a)->cont),b,"~");
}
Ob varassignvar(Ob a,Ob b){
	return assignvar(getvar(((strOb*)a)->cont),b);
}
Ob falsop(Ob a,Ob b){
	return &False;
}
Ob codeexec(Ob a,Ob b){
	return eval(((codeOb*)a)->cont,{});
}
Ob trop(Ob a,Ob b){
	return &True;
}
Ob commapp(Ob a,Ob b){
	std::vector<Ob> v=((commOb*)a)->args;
	v.push_back(b);
	return new commOb(std::move(v),((commOb*)a)->fun);
}
Ob commnewl(Ob a,Ob b){
	if(((commOb*)a)->args.empty())
		err("Syntax error: No arguments specified");
	if(((commOb*)a)->args.back().type()!="code")
		err("Syntax error: Did you forget []?");
	((commOb*)a)->fun(((commOb*)a)->args);
	return b;
}
Ob callcumfun(Ob a,Ob b){
	if(b.type()=="var")
		b=getvar(((strOb*)b)->cont);
	return eval(((cumfunOb*)a)->cod,{{((cumfunOb*)a)->arg,b}});
}
Ob codequal(Ob a,Ob b){
	return bOOl(((codeOb*)a)->cont==((codeOb*)b)->cont);
}
Ob equop(Ob a,Ob b){
	return oper(a,b,"=");
}
Ob greatoreq(Ob a,Ob b){
	if(setfree.count(a.type())||setfree.count(b.type()))
		err("No >= defined for "+a.type()+" and "+b.type());
	for(Ob o:((setOb*)b)->cont){
		if(!((setOb*)a)->cont.count(o))
			return &False;
	}
	return &True;
}
Ob lowr(Ob a,Ob b){
	if(bOOl(oper(a,b,"=")))
		return &False;
	return oper(b,a,">=");
}
Ob varnewl(Ob a,Ob b){
	//vars.back().try_emplace(((strOb*)a)->cont,&None);
	return b;
}
Ob bigassvar(Ob a,Ob b){
	assignvar(a,b);
	for(std::vector<std::unordered_map<std::string,Ob>>::reverse_iterator i=vars.rbegin()+1;i!=vars.rend();i++){
		try{
			i->at(((strOb*)b)->cont);
			Findanass->insert(((strOb*)b)->cont);
			return b;
		}catch(const std::out_of_range&){}
	}
	err("Couldn't find variable "+((strOb*)b)->cont);
}
Ob varbigassvar(Ob a,Ob b){
	return bigassvar(getvar(((strOb*)a)->cont),b);
}
Ob backslash(Ob a,Ob b){
	if(setfree.count(a.type()))
		err("No \\ defined for "+a.type()+" and "+b.type());
	std::unordered_set<Ob> s=((setOb*)a)->cont;
	s.erase(b);
	return new setOb(a.type(),std::move(s));
}
Ob And(Ob a,Ob b){
	if(setfree.count(a.type())||setfree.count(b.type()))
		err("No & defined for "+a.type()+" and "+b.type());
	std::unordered_set<Ob> re;
	for(Ob o:((setOb*)a)->cont){
		if(((setOb*)b)->cont.count(o))
			re.insert(o);
	}
	return new setOb(a.type(),std::move(re));
}
Ob concat(Ob a,Ob b){
	return str(((strOb*)a)->cont+((strOb*)b)->cont);
}