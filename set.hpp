Ob set(const std::unordered_set<Ob>&v){
	return new setOb("set",v);
}
Ob set(std::unordered_set<Ob>&&v){
	return new setOb("set",v);
}
Ob bOOl(bool b){
	if(b)
		return &True;
	return &False;
}
setOb empset("set",{});
CARE(empset)