struct biopopOb{
	std::string type="biopop";
	unsigned refcnt=0;
	Ob(*cont)(Ob,Ob,std::string);
	biopopOb(Ob(*c)(Ob,Ob,std::string)){
		cont=c;
	}
};
Ob biopop(Ob(*f)(Ob,Ob,std::string)){
	return new biopopOb(f);
}
Ob varop(Ob a,Ob b,std::string op){
	return oper(getvar(((strOb*)a)->cont),b,op);
}
Ob opvar(Ob a,Ob b,std::string op){
	return oper(a,getvar(((strOb*)b)->cont),op);
}