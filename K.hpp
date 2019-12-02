#include <string>
#include <fstream>
#include <streambuf>
typedef std::basic_string<unsigned char> byco;
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <execinfo.h>
#include <unistd.h>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define SWWCTA err_("Something went wrong. Contact the author. " AT);
#define SIGSEGV int i=*(int*)69;
#define CARE(o) const Ob care##o=&o;

void bt(){
	void*arr[10];
	size_t s=backtrace(arr,10);
	backtrace_symbols_fd(arr,s,STDERR_FILENO);
}

[[noreturn]]void err_(const std::string&x){
	std::cerr<<x<<'\n';
	_exit(-1);
}

[[noreturn]]void err(const std::string&x){
	throw std::runtime_error(x);
}

#define NYI err_("Not yet implemented. " AT);

#define LOAD (unsigned char)0
#define OP (unsigned char)1
#define SET (unsigned char)2

struct ob{
	std::string type;
	unsigned refcnt=0;
	ob(const std::string&t):type(t){}
};

struct Ob{
	ob*o;
	Ob(){
		o=nullptr;
	}
	Ob(const Ob&O){
		o=O.o;
		o->refcnt++;
	}
	Ob(Ob&&O){
		o=O.o;
		O.o=nullptr;
	}
	template<typename T>Ob(T*O){
		o=(ob*)O;
		O->refcnt++;
	}
	~Ob();
	Ob&operator=(const Ob&O){
		this->~Ob();
		o=O.o;
		o->refcnt++;
		return *this;
	}
	Ob&operator=(Ob&&O){
		this->~Ob();
		o=O.o;
		O.o=nullptr;
		return *this;
	}
	template<typename T> explicit operator T*()const{
		return (T*)o;
	}
	std::string type()const{
		return o->type;
	}
};
bool operator==(const Ob&a,const Ob&b){
	return a.o==b.o;
}
namespace std{
	template<> struct hash<Ob>{
		size_t operator()(const Ob&o)const{
			return std::hash<ob*>()(o.o);
		}
	};
}
#include "none.hpp"
struct biopOb{
	std::string type="biop";
	unsigned refcnt=0;
	Ob(*cont)(Ob,Ob);
	biopOb(Ob(*c)(Ob,Ob)){
		cont=c;
	}
};
struct setOb{
	std::string type;
	unsigned refcnt=0;
	std::unordered_set<Ob>cont;
	setOb(const std::string&t,const std::unordered_set<Ob>&c):type(t),cont(c){}
	setOb(const std::string&t,std::unordered_set<Ob>&&c):type(t),cont(c){}
};
setOb False("bool",{});
CARE(False)
setOb True("bool",{(Ob)&False});
CARE(True)
std::string indent(const std::string&s){
	std::string re;
	for(char i:s){
		if(i=='\n')
			re+="\n\t";
		else
			re+=i;
	}
	return re;
}
Ob execfile(const std::string&,const std::unordered_map<std::string,Ob>&);
Ob eval(const byco&,const std::unordered_map<std::string,Ob>&);
Ob bifun(Ob(*)(Ob));
Ob biop(Ob(*)(Ob,Ob));
Ob biopop(Ob(*)(Ob,Ob,std::string));
Ob comm(void(*)(const std::vector<Ob>&));
Ob getvar(const std::string&);
Ob equal(Ob,Ob);
Ob out(Ob);
Ob tilde(Ob,Ob);
Ob callbifun(Ob,Ob);
Ob newl(Ob,Ob);
Ob assignvar(Ob,Ob);
Ob comma(Ob,Ob);
biopOb Comma(comma);
Ob strequal(Ob,Ob);
biopOb Strequal(strequal);
Ob conv__str(Ob);
Ob Or(Ob,Ob);
Ob vartilde(Ob,Ob);
Ob varop(Ob,Ob,std::string);
Ob opvar(Ob,Ob,std::string);
Ob New(Ob);
Ob varassignvar(Ob,Ob);
Ob falsop(Ob,Ob);
biopOb Falsop(falsop);
Ob conv__set(Ob);
Ob codeexec(Ob,Ob);
Ob trop(Ob,Ob);
Ob commapp(Ob,Ob);
Ob commnewl(Ob,Ob);
Ob callcumfun(Ob,Ob);
Ob derefvar(Ob,Ob);
Ob Throw(Ob);
Ob codequal(Ob,Ob);
Ob equop(Ob,Ob);
Ob import(Ob);
Ob Export(Ob);
Ob greatoreq(Ob,Ob);
Ob force(Ob);
Ob lowr(Ob,Ob);
Ob varnewl(Ob,Ob);
Ob bigassvar(Ob,Ob);
Ob varbigassvar(Ob,Ob);
Ob from(Ob);
Ob backslash(Ob,Ob);
Ob Typeof(Ob);
Ob And(Ob,Ob);
Ob dbginfo(Ob);
Ob concat(Ob,Ob);
void def(const std::vector<Ob>&);
void While(const std::vector<Ob>&);
void If(const std::vector<Ob>&);
std::unordered_set<char> opc{' ','\n','>','<','|',',',';','.',':','-','#','+','*','~','?','\\','=','/','&','%','!'};
std::unordered_set<char> psopc{'(',')','[',']','{','}','"','\'','$'};
std::vector<Ob> consts;
std::vector<std::unordered_map<std::string,Ob>> vars{{{"",&None},{"~ var",biop(tilde)},{"out",bifun(out)},{"=",biop(equal)},{"bifun ",biop(callbifun)},{"\n",biop(newl)},{"-> var",biop(assignvar)},{"var ~ var",biop(vartilde)},{",",&Comma},{"str = str",&Strequal},{"~str",bifun(conv__str)},{"|",biop(Or)},{"var __",biopop(varop)},{"__ var",biopop(opvar)},{"new",bifun(New)},{"var -> var",biop(varassignvar)},{"var =",&Falsop},{"= var",&Falsop},{"var = var",&Strequal},{"~set",bifun(conv__set)},{"var ,",&Comma},{", var",&Comma},{"code ! none",biop(codeexec)},{"none = none",biop(trop)},{"comm ",biop(commapp)},{"comm \n",biop(commnewl)},{"def",comm(def)},{"cumfun ",biop(callcumfun)},{"throw",bifun(Throw)},{"code = code",biop(codequal)},{"==",biop(equop)},{"import",bifun(import)},{"export",bifun(Export)},{">=",biop(greatoreq)},{"while",comm(While)},{"if",comm(If)},{"True",&True},{"False",&False},{"force",bifun(force)},{"<",biop(lowr)},{"--> var",biop(bigassvar)},{"var --> var",biop(varbigassvar)},{"from",bifun(from)},{"\\",biop(backslash)},{"typeof",bifun(Typeof)},{"&",biop(And)},{"dbginfo",bifun(dbginfo)},{"str  str",biop(concat)}}};
std::vector<std::unordered_set<Ob>>*Stack;
Ob oper(Ob,Ob,std::string);
const std::unordered_set<std::string> setfree{"none","str","bifun","biop","biopop","var"};
std::unordered_set<std::string> forced,*Expolst,*Findanass;
bool*Export_all;
void insert(std::unordered_set<Ob>&s,Ob o){
	s.insert(o);
}
bool bOOl(Ob);
#include "str.hpp"
#include "set.hpp"
#include "var.hpp"
#include "code.hpp"
#include "cumfun.hpp"
#include "cumop.hpp"
#include "comm.hpp"
#include "bifun.hpp"
#include "biop.hpp"
#include "biopop.hpp"

Ob getvar(const std::string&S){
	std::string s=S;
	for(std::vector<std::unordered_map<std::string,Ob>>::reverse_iterator i=vars.rbegin();i!=vars.rend();i++){
		try{
			return i->at(s);
		}catch(const std::out_of_range&){}
	}
	std::string traba;
	for(;s!="";s.pop_back()){
		for(std::vector<std::unordered_map<std::string,Ob>>::reverse_iterator i=vars.rbegin();i!=vars.rend();i++){
			try{
				Ob f=i->at(s+"...");
				if(f.type()=="bifun")
					return ((bifunOb*)f)->cont(str(s));
				else if(f.type()=="cumfun")
					return eval(((cumfunOb*)f)->cod,{{((cumfunOb*)f)->arg,str(s)}});
				else
					err("Can't call "+f.type()+" object");
			}catch(const std::out_of_range&){}
			catch(const std::runtime_error&e){
				traba+="\n'"+s+"...' failed: "+e.what();
			}
		}
	}
	err("Unknown variable "+S+'['+indent(traba)+']');
}

Ob::~Ob(){
	if(o){
		if(!--o->refcnt){
			//std::cerr<<"Destructing "<<type()<<" object ";
			if(type()=="none"||type()=="bifun"||type()=="biop"||type()=="biopop")
				err_("Replacing of built-in objects is not permitted. Try Pro!");
			else if(type()=="str"||type()=="var")
				delete (strOb*)o;
			else if(type()=="code")
				delete (codeOb*)o;
			else if(type()=="comm")
				delete (commOb*)o;
			else if(type()=="cumfun")
				delete (cumfunOb*)o;
			else if(type()=="cumop")
				delete (cumopOb*)o;
			else
				delete (setOb*)o;
			//std::cerr<<"successfully\n";
		}
	}
}
std::string print(std::string s){
	std::cerr<<s<<'\n';
	return s;
}
Ob oper(Ob a,Ob b,std::string op){
	Ob F,re;
	std::string traba;
	std::string tsa[]={a.type()+' '+op+' '+b.type(),a.type()+" __ "+b.type(),a.type()+' '+op,op+' '+b.type(),a.type()+" __","__ "+b.type(),op,"__"};
	for(std::string*i=tsa;i!=std::end(tsa);i++){
		try{
			F=getvar(*i);
			if(F.type()=="biop")
				re=((biopOb*)F)->cont(a,b);
			else if(F.type()=="biopop")
				re=((biopopOb*)F)->cont(a,b,op);
			else if(F.type()=="cumop"){
				if(a.type()=="var")
					a=getvar(((strOb*)a)->cont);
				if(b.type()=="var")
					b=getvar(((strOb*)b)->cont);
				re=eval(((cumopOb*)F)->cod,{{((cumopOb*)F)->a,a},{((cumopOb*)F)->b,b},{((cumopOb*)F)->op,str(op)}});
			}else{
				traba+="\n'"+*i+"' failed: Can't call "+F.type()+" object";
				continue;
			}
			return re;
		}catch(const std::runtime_error&e){
			if(e.what()!="Unknown variable "+*i+"[]")
				traba+="\n'"+*i+"' failed: "+e.what();
			if(forced.count(*i))
				err_(op+" failed for "+a.type()+" and "+b.type()+'['+indent(traba)+" (forced)]");
		}
	}
	err(op+" failed for "+a.type()+" and "+b.type()+'['+indent(traba)+']');
}

bool bOOl(Ob o){
	bool re;
	if(o.type()=="var")
		o=getvar(((strOb*)o)->cont);
	if(o.type()=="none")
		re=false;
	else if(o.type()=="str")
		re=!((strOb*)o)->cont.empty();
	else if(o.type()=="bifun"||o.type()=="biop"||o.type()=="biopop"||o.type()=="comm"||o.type()=="cumfun"||o.type()=="cumop")
		err("Invalid use of function");
	else if(o.type()=="code")
		err("Can't interpret code object as bool");
	else{
		re=!((setOb*)o)->cont.empty();
	}
	return re;
}

byco load(Ob o){
	std::vector<Ob>::iterator i;
	for(i=consts.begin();i!=consts.end();i++){
		if(bOOl(oper(o,*i,"=")))
			break;
	}
	byco re{LOAD,(unsigned char)(i-consts.begin())};
	if(i==consts.end())
		consts.push_back(o);
	return re;
}

byco compile(std::string::const_iterator&,char);

byco compile1(std::string::const_iterator&i,char term){
	std::string s;
	switch(*i){
		case ')':case ']':case '}':
		if(*i!=term){
			err(std::string("Unexpected ")+*i);
		}
		return load(&None);
		case '(':
		return compile(++i,')');
		case '[':
		return load(code(compile(++i,']')));
		case '{':
		if(*++i=='}'){
			i++;
			return load(&empset);
		}
		return compile(i,'}')+SET;
		case '"':
		while(*++i!='"'){
			if(*i=='\\')
				i++;
			s+=*i;
		}
		i++;
		return load(str(s));
		case '\'':
		while(*++i!='\''){
			if(*i=='\\')
				i++;
			s+=*i;
		}
		i++;
		return load(var(s));
		default:
		while(!(opc.count(*i)||psopc.count(*i)))
			s+=*(i++);
		return load(var(s));
	}
}

byco compile(std::string::const_iterator&i,char term){
	byco re=compile1(i,term);
	while(*i!=term){
		byco op;
		if(*i=='$'){
			while(*++i!='$')
				op+=*i;
			i++;
		}else{
			for(;opc.count(*i);i++){
				if(*i!=' ')
					op+=*i;
			}
		}
		re+=compile1(i,term);
		re.push_back(OP);
		re+=op;
		re.push_back(0);
	}
	i++;
	return re;
}

char hx(unsigned i){
	if(i<10)
		return '0'+i;
	return 'a'-10+i;
}
void pb(unsigned char i){
	std::cerr<<hx(i/16)<<hx(i%16)<<i;
}
Ob printtype(Ob o){
	std::cerr<<o.type()<<'\n';
	return o;
}
Ob eval(const byco&c,const std::unordered_map<std::string,Ob>&args){
	vars.push_back(args);
	std::vector<std::unordered_set<Ob>>*oStack=Stack;
	std::vector<std::unordered_set<Ob>> stack;
	Stack=&stack;
	std::unordered_set<std::string> expolst,findanass,*oExpolst=Expolst,*oFindanass=Findanass;
	bool export_all,*oExport_all=Export_all;
	Expolst=&expolst;
	Findanass=&findanass;
	Export_all=&export_all;
	try{
		for(byco::const_iterator i=c.begin();i!=c.end();i++){
			std::string op;
			std::unordered_set<Ob> av,bv;
			switch(*i){
				case LOAD:
				stack.push_back({consts[*++i]});
				break;
				case OP:
				while(*++i!=0)
					op+=*i;
				bv=stack.back();
				stack.pop_back();
				av=stack.back();
				stack.pop_back();
				stack.push_back({});
				for(Ob b:bv){
					for(Ob a:av)
						stack.back().insert(oper(a,b,op));
				}
				break;
				case SET:
				av=stack.back();
				stack.pop_back();
				for(std::unordered_set<Ob>::const_iterator j=av.cbegin();j!=av.cend();j++){
					if(j->type()=="var")
						bv.insert(getvar(((strOb*)*j)->cont));
					else
						bv.insert(*j);
				}
				stack.push_back({set(std::move(bv))});
			}
		}
	}catch(const std::runtime_error&){
		vars.pop_back();
		Stack=oStack;
		Expolst=oExpolst;
		Findanass=oFindanass;
		Export_all=oExport_all;
		throw;
	}
	if(stack.size()!=1)
		SWWCTA
	Stack=oStack;
	if(stack.front().size()!=1){
		vars.pop_back();
		Expolst=oExpolst;
		Findanass=oFindanass;
		Export_all=oExport_all;
		err("Trouble with comma");
	}
	Ob re;
	if(stack.front().begin()->type()=="var"){
		try{
			re=getvar(((strOb*)*stack.front().begin())->cont);
		}catch(const std::runtime_error&){
			vars.pop_back();
			throw;
		}
	}else
		re=*stack.front().begin();
	for(const std::string&s:findanass){
		Ob o=vars.back().at(s);
		for(std::vector<std::unordered_map<std::string,Ob>>::reverse_iterator i=vars.rbegin()+1;i!=vars.rend();i++){
			try{
				i->at(s)=o;
				break;
			}catch(const std::out_of_range&){}
		}
	}
	if(export_all){
		for(const std::pair<std::string,Ob>&p:vars.back())
			(*(vars.end()-2))[p.first]=p.second;
	}else{
		for(const std::string&s:expolst)
			(*(vars.end()-2))[s]=vars.back().at(s);
	}
	Expolst=oExpolst;
	Findanass=oFindanass;
	Export_all=oExport_all;
	vars.pop_back();
	return re;
}
Ob execfile(const std::string&n,const std::unordered_map<std::string,Ob>&a){
	std::ifstream is(n);
	if(is.fail())
		err("Couldn't open "+n);
	std::string s((std::istreambuf_iterator<char>(is)),std::istreambuf_iterator<char>());
	s+=')';
	std::string::const_iterator j=s.begin();
	try{
		return eval(compile(j,')'),a);
	}catch(const std::runtime_error&e){
		err("File "+n+": "+e.what());
	}
}