struct bifunOb{
	std::string type="bifun";
	unsigned refcnt=0;
	Ob(*cont)(Ob);
	bifunOb(Ob(*c)(Ob)){
		cont=c;
	}
};
Ob bifun(Ob(*f)(Ob)){
	return new bifunOb(f);
}

Ob out(Ob o){
	o=oper(o,&varstr,"~");
	std::cout<<((strOb*)o)->cont<<'\n';
	return o;
}
Ob conv__str(Ob o){
	if(setfree.count(o.type()))
		err("Can't convert "+o.type()+" to string");
	if(((setOb*)o)->cont.empty())
		return &strempset;
	std::unordered_set<Ob>::iterator i=((setOb*)o)->cont.begin();
	std::string re="{"+((strOb*)oper(*i,&varstr,"~"))->cont;
	for(i++;i!=((setOb*)o)->cont.end();i++)
		re+=','+((strOb*)oper(*i,&varstr,"~"))->cont;
	return str(re+'}');
}
Ob New(Ob o){
	if(o.type()!="var")
		err("Expected type name after new");
	if(setfree.count(((strOb*)o)->cont))
		err("Don't use new for built-in types");
	return new setOb(((strOb*)o)->cont,{});
}
Ob conv__set(Ob o){
	if(setfree.count(o.type()))
		err("Can't convert "+o.type()+" to set");
	return set(((setOb*)o)->cont);
}
Ob Throw(Ob o){
	o=oper(o,&varstr,"~");
	err(((strOb*)o)->cont);
}
Ob import(Ob o){
	std::string n;
	if(o.type()=="var")
		n=((strOb*)o)->cont+".stl";
	else if(o.type()=="str")
		n=((strOb*)o)->cont;
	else
		err("Expected module or file name after import");
	return execfile(n,{});
}
Ob Export(Ob o){
	if(o.type()=="none"){
		*Export_all=true;
		return o;
	}
	if(o.type()!="var")
		err("Expected variable after export");
	try{
		vars.back().at(((strOb*)o)->cont);
	}catch(std::out_of_range&){
		err("No local variable '"+((strOb*)o)->cont+"' found (export)");
	}
	Expolst->insert(((strOb*)o)->cont);
	return o;
}
Ob force(Ob o){
	if(o.type()=="none"){
		for(std::pair<std::string,Ob>p:vars.back())
			forced.insert(p.first);
		return o;
	}
	if(o.type()!="var")
		err("Expected variable after forced");
	forced.insert(((strOb*)o)->cont);
	return o;
}
Ob from(Ob o){
	if(o.type()=="var")
		o=getvar(((strOb*)o)->cont);
	if(o.type()=="str")
		return str(std::string()+((strOb*)o)->cont.back());
	if(setfree.count(o.type()))
		err("Expected string or setlike object after from");
	if(((setOb*)o)->cont.empty())
		err("Empty set (from)");
	return *((setOb*)o)->cont.begin();
}
Ob Typeof(Ob o){
	if(o.type()=="var")
		o=getvar(((strOb*)o)->cont);
	return str(o.type());
}
Ob dbginfo(Ob o){
	if(o.type()=="var"){
		std::cerr<<"var "<<((strOb*)o)->cont<<": ";
		o=getvar(((strOb*)o)->cont);
	}
	std::cerr<<o.type();
	if(!setfree.count(o.type()))
		std::cerr<<": size "<<((setOb*)o)->cont.size();
	std::cerr<<'\n';
	return o;
}