#include "ast.hh"
#include <cstdarg>
#include <bits/stdc++.h>
using namespace std;

// implementation of all the classes in ast.hh

void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
{   
	typedef vector<abstract_astnode *>* pv;
	va_list args;
	va_start(args, fmt);
	if ((astname != NULL) && (astname[0] != '\0'))
	{
		cout << "{ ";
		cout << "\"" << astname << "\"" << ": ";
	}
	cout << "{" << endl;
	while (*fmt != '\0')
	{
		if (*fmt == 'a')
		{
			char * field = va_arg(args, char *);
			abstract_astnode *a = va_arg(args, abstract_astnode *);
			cout << "\"" << field << "\": " << endl;
			
			a->print(0);
		}
		else if (*fmt == 's')
		{
			char * field = va_arg(args, char *);
			char *str = va_arg(args, char *);
			cout << "\"" << field << "\": ";

			cout << str << endl;
		}
		else if (*fmt == 'i')
		{
			char * field = va_arg(args, char *);
			int i = va_arg(args, int);
			cout << "\"" << field << "\": ";

			cout << i;
		}
		else if (*fmt == 'f')
		{
			char * field = va_arg(args, char *);
			double f = va_arg(args, double);
			cout << "\"" << field << "\": ";
			cout << f;
		}
		else if (*fmt == 'l')
		{
			char * field = va_arg(args, char *);
			pv f =  va_arg(args, pv);
			cout << "\"" << field << "\": ";
			cout << "[" << endl;
			for (int i = 0; i < (int)f->size(); ++i)
			{
				(*f)[i]->print(0);
				if (i < (int)f->size() - 1)
					cout << "," << endl;
				else
					cout << endl;
			}
			cout << endl;
			cout << "]" << endl;
		}
		++fmt;
		if (*fmt != '\0')
			cout << "," << endl;
	}
	cout << "}" << endl;
	if ((astname != NULL) && (astname[0] != '\0'))
		cout << "}" << endl;
	va_end(args);
}


exp_astnode::exp_astnode(Type* type) {
    this->type = type;
}

exp_astnode::exp_astnode() {
    this->type = new Type();
}

empty_astnode::empty_astnode() {
    this->astnode_type = typeExp::Empty_astnode;
}

seq_astnode::seq_astnode() {
    this->astnode_type = typeExp::Seq_astnode;
    this->statements = vector<statement_astnode*>();
}

seq_astnode::seq_astnode(vector<statement_astnode*> statements) {
    this->astnode_type = typeExp::Seq_astnode;
    this->statements = statements;
}

void seq_astnode::addStatement(statement_astnode* statement) {
    this->statements.push_back(statement);
}

assignS_astnode::assignS_astnode() {
    this->astnode_type = typeExp::AssignS_astnode;
}

assignS_astnode::assignS_astnode(exp_astnode* lhs, exp_astnode* rhs) {
    this->astnode_type = typeExp::AssignS_astnode;
    this->lhs = lhs;
    this->rhs = rhs;
}

return_astnode::return_astnode() {
    this->astnode_type = typeExp::Return_astnode;
}

return_astnode::return_astnode(exp_astnode* exp) {
    this->astnode_type = typeExp::Return_astnode;
    this->exp = exp;
}

proccall_astnode::proccall_astnode() {
    this->astnode_type = typeExp::Proccall_astnode;
    // this->fname = new identifier_astnode();
    this->args = vector<exp_astnode*>();
}

proccall_astnode::proccall_astnode(identifier_astnode* fname, vector<exp_astnode*> args) {
    this->astnode_type = typeExp::Proccall_astnode;
    this->args = args;
    this->fname = fname;
}

proccall_astnode::proccall_astnode(identifier_astnode* fname) {
    this->astnode_type = typeExp::Proccall_astnode;
    this->fname = fname;
    this->args = vector<exp_astnode*>();
}

void proccall_astnode::addArg(exp_astnode* arg) {
    this->args.push_back(arg);
}

if_astnode::if_astnode() {
    this->astnode_type = typeExp::If_astnode;
}

if_astnode::if_astnode(exp_astnode* cond, statement_astnode* then_, statement_astnode* else_) {
    this->astnode_type = typeExp::If_astnode;
    this->cond = cond;
    this->then_ = then_;
    this->else_ = else_;
}

while_astnode::while_astnode() {
    this->astnode_type = typeExp::While_astnode;
}

while_astnode::while_astnode(exp_astnode* cond, statement_astnode* body) {
    this->astnode_type = typeExp::While_astnode;
    this->cond = cond;
    this->body = body;
}

for_astnode::for_astnode() {
    this->astnode_type = typeExp::For_astnode;
}

for_astnode::for_astnode(exp_astnode* init, exp_astnode* cond, exp_astnode* incr, statement_astnode* body){
    this->astnode_type = typeExp::For_astnode;
    this->init = init;
    this->cond = cond;
    this->incr = incr;
    this->body = body;
}

op_binary_astnode::op_binary_astnode() {
    this->astnode_type = typeExp::Op_binary_astnode;
}

op_binary_astnode::op_binary_astnode(string op, exp_astnode* lhs, exp_astnode* rhs) {
    this->astnode_type = typeExp::Op_binary_astnode;
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

op_unary_astnode::op_unary_astnode() {
    this->astnode_type = typeExp::Op_unary_astnode;
}

op_unary_astnode::op_unary_astnode(string op, exp_astnode* exp) {
    this->astnode_type = typeExp::Op_unary_astnode;
    this->exp = exp;
    this->op = op;
}

assignE_astnode::assignE_astnode() {
    this->astnode_type = typeExp::AssignE_astnode;
}

assignE_astnode::assignE_astnode(exp_astnode* lhs, exp_astnode* rhs) {
    this->astnode_type = typeExp::AssignE_astnode;
    this->lhs = lhs;
    this->rhs = rhs;
}

funcall_astnode::funcall_astnode() {
    this->astnode_type = typeExp::Funcall_astnode;
    // this->fname = "";
    this->args = vector<exp_astnode*>();
}

funcall_astnode::funcall_astnode(identifier_astnode* fname, vector<exp_astnode*> args) {
    this->astnode_type = typeExp::Funcall_astnode;
    this->args = args;
    this->fname = fname;
}

funcall_astnode::funcall_astnode(identifier_astnode* fname) {
    this->astnode_type = typeExp::Funcall_astnode;
    this->fname = fname;
    this->args = vector<exp_astnode*>();
}

void funcall_astnode::addArg(exp_astnode* arg) {
    this->args.push_back(arg);
}

intconst_astnode::intconst_astnode() {
    this->astnode_type = typeExp::Intconst_astnode;
}

intconst_astnode::intconst_astnode(int value) {
    this->astnode_type = typeExp::Intconst_astnode;
    this->value = value;
}

floatconst_astnode::floatconst_astnode() {
    this->astnode_type = typeExp::Floatconst_astnode;
}

floatconst_astnode::floatconst_astnode(float value) {
    this->astnode_type = typeExp::Floatconst_astnode;
    this->value = value;
}

stringconst_astnode::stringconst_astnode() {
    this->astnode_type = typeExp::Stringconst_astnode;
}

stringconst_astnode::stringconst_astnode(string value) {
    this->astnode_type = typeExp::Stringconst_astnode;
    this->value = value;
}

identifier_astnode::identifier_astnode() {
    this->astnode_type = typeExp::IDENTIFIER_astnode;
}

identifier_astnode::identifier_astnode(string name) {
    this->astnode_type = typeExp::IDENTIFIER_astnode;
    this->name = name;
}

arrayref_astnode::arrayref_astnode() {
    this->astnode_type = typeExp::Arrayref_astnode;
}

arrayref_astnode::arrayref_astnode(exp_astnode* array, exp_astnode* index) {
    this->astnode_type = typeExp::Arrayref_astnode;
    this->array = array;
    this->index = index;
}

member_astnode::member_astnode() {
    this->astnode_type = typeExp::Member_astnode;
}

member_astnode::member_astnode(exp_astnode* exp, identifier_astnode* mem) {
    this->astnode_type = typeExp::Member_astnode;
    this->exp = exp;
    this->mem = mem;
}

arrow_astnode::arrow_astnode() {
   this->astnode_type = typeExp::Arrow_astnode;
}

arrow_astnode::arrow_astnode(exp_astnode* exp, identifier_astnode* mem) {
    this->astnode_type = typeExp::Arrow_astnode;
    this->exp = exp;
    this->mem = mem;
}

void empty_astnode::print(int blanks){
    cout<<string(blanks, ' ')<<"\"empty\""<<endl;
}

void seq_astnode::print(int blanks){
    printAst("","l","seq",statements);
}

void assignS_astnode::print(int blanks){
    printAst("assignS","aa","left",lhs,"right",rhs);
}

void return_astnode::print(int blanks){
    printAst("", "a", "return", exp);
}

void proccall_astnode::print(int blanks){
    // cout<<"HI proccall"<<endl;
    printAst("proccall", "al", "fname", fname, "params", args);
}

void if_astnode::print(int blanks){
    printAst("if", "aaa", "cond", cond, "then", then_, "else", else_);
}

void while_astnode::print(int blanks){
    printAst("while", "aa", "cond", cond, "stmt", body);
}

void for_astnode::print(int blanks){
    printAst("for", "aaaa", "init", init, "guard", cond, "step", incr, "body", body);
}

void op_binary_astnode::print(int blanks){
    string opstr = "\""+op+"\"";
    char* opstrc = const_cast<char*>(opstr.c_str());
    printAst("op_binary", "saa", "op", opstrc, "left", lhs, "right", rhs);
}   

void op_unary_astnode::print(int blanks){
    string opstr = "\""+op+"\"";
    char* opstrc = const_cast<char*>(opstr.c_str());
    printAst("op_unary", "sa", "op", opstrc, "child", exp);
}

void assignE_astnode::print(int blanks){
    printAst("assignE", "aa", "left", lhs, "right", rhs);
}

void funcall_astnode::print(int blanks){
    printAst("funcall", "al", "fname", fname, "params", args);
}

void intconst_astnode::print(int blanks){
    printAst("", "i", "intconst", value);
}

void floatconst_astnode::print(int blanks){
    printAst("", "f", "floatconst", value);
}

void stringconst_astnode::print(int blanks){
    printAst("", "s", "stringconst", const_cast<char*>(value.c_str()));
}

void identifier_astnode::print(int blanks){
    // cout<<"HI identifier"<<endl;
    string str = "\""+name+"\"";
    printAst("", "s", "identifier", const_cast<char*>(str.c_str()));   
}

void arrayref_astnode::print(int blanks){
    printAst("arrayref", "aa", "array", array, "index", index);
}

void member_astnode::print(int blanks){
    printAst("member", "aa", "struct", exp, "field", mem);
}

void arrow_astnode::print(int blanks){
    printAst("arrow", "aa", "pointer", exp, "field", mem);
}


bool isPrintf = false;
int printfCount = -1;
bool toReg = true;
stack<int> used;
int paramLength = 0;
int offsetForPP = 0;
string identifierForStruct ="";
int offsetForStruct = 0;
Type* typeForStruct;
int offsetForFunction = 0;
string identifierForStructPtr = "";
int offsetForStructPtr = 0;
Type* typeForStructPtr;
int offsetForArray = 0;
string identifierForArray = "";
Type* typeForArray;
bool structRetType = false;
bool local_ = false;
bool structArray = false;
int sizeOfCorrespondingParam = -1;

void empty_astnode::gencode(string scope){
}

void seq_astnode::gencode(string scope){
    for (auto it = statements.begin(); it != statements.end(); it++){
        (*it)->gencode(scope);
    }
}

void assignS_astnode::gencode(string scope){
    rhs->gencode("exp");
    // cout<<"lhs"<<endl;
    lhs->gencode("lhs");
}

void return_astnode::gencode(string scope){
    // cout<<"return"<<endl;
    // cout<<func<<" "<<used.size()<<endl;
    Type* type___ = new Type();
    int sizeOfParams = 0;
    exp->gencode("exp");
    // cout<<func<<" "<<used.size()<<endl;
    // cout<<sizeOfParams<<endl;
    for(auto ig: localst->entries){
        // get the offset of param with highest value
        if(ig.second->offset >= sizeOfParams){
            sizeOfParams = ig.second->offset;
            type___ = ig.second->retType;
        }
    }
    // cout<<"sizeOfParams: "<<sizeOfParams<<endl;
    for(auto igh: gststruct.entries){
        // cout<<igh.second->retType->name<<endl;
        // cout<<type___->name<<endl;
        if(igh.second->retType->name == type___->name){
            sizeOfParams += igh.second->retType->size;
            break;
        }
    }
    if(type___->name == "int" || checkPointer(type___)){
        sizeOfParams += 4;
    }
    // cout<<"sizeOfParams: "<<sizeOfParams<<endl;
    // cout<<"exp done"<<endl;
    // cout<<exp->type->name<<endl;
    if(func != "main"){
        if(exp->type->name.substr(0,6)!="struct" || checkPointer(exp->type)){
            instructions[label].push_back("movl "+registers[used.top()]+", "+to_string(sizeOfParams+24)+"(%ebp)");
            rstack.push(used.top());
            used.pop();
        }else if(exp->type->name.substr(0,6)=="struct" && !checkPointer(exp->type)){
            // cout<<"substr"<<endl;
            int used_size = used.size();
            for(int jk=0;jk<used_size;jk++){
                rstack.push(used.top());
                used.pop();
            }
            // instructions[label].push_back("popl %eax");
            // instructions[label].push_back("movl %eax, "+to_string(sizeOfParams)+"(%ebp)");
            for(auto i: gststruct.entries){
                if(i.first == exp->type->name){
                    // pop in the reverse order of offsets
                    vector<pair<int, string>> temp;
                    for(auto j: i.second->child->entries){
                        temp.push_back(make_pair(j.second->offset, j.first));
                    }
                    //sort in reverse order
                    sort(temp.begin(), temp.end());
                    for(auto j: temp){
                        instructions[label].push_back("popl "+registers[rstack.top()]);
                        instructions[label].push_back("movl  "+registers[rstack.top()]+", "+to_string(sizeOfParams+24+j.first)+"(\%ebp)");
                    }
                }
            }
        }
    }else{
        if(func == "main") instructions[label].push_back("movl "+registers[used.top()]+", %eax");
        int used_size = used.size();
        for(int jk=0;jk<used_size;jk++){
            rstack.push(used.top());
            used.pop();
        }
    }
    // cout<<func<<" "<<used.size()<<endl;
    int temp_count = used.size();
    for(int hj=0; hj<temp_count; hj++){
        rstack.push(used.top());
        used.pop();
    }
    // cout<<func<<" "<<used.size()<<endl;
    instructions[label].push_back("leave");
    instructions[label].push_back("ret");
}

void proccall_astnode::gencode(string scope){
    if(this->fname->name == "printf"){
        isPrintf = true;
        printfCount++;
        printCountMap[label]++;
        for (auto it = args.rbegin(); it != args.rend(); it++){
            (*it)->gencode("param");
        }
        // for(int i=1; i<args.size(); i++){
        //     instructions[label].push_back("pushl "+registers[used.top()-(args.size()-i-1)]);
        // }
        instructions[label].push_back("pushl $.LC"+to_string(printfCount));
        instructions[label].push_back("call printf");
        instructions[label].push_back("addl $"+to_string(4*(args.size()))+", %esp");
        // for(int i=1; i<args.size(); i++){
        //     rstack.push(used.top());
        //     used.pop();
        // }
        // pop all registers in used stack
        int used_size_ = used.size();
        for(int i=0;i<used_size_;i++){
            rstack.push(used.top());
            used.pop();
        }
        isPrintf = false;
        // cout<<"printf end"<<endl;
    }else{
        int off_ = 0;
    for(auto ijk: gstfun.entries){
        if(ijk.first == fname->name){
            if(ijk.second->retType->name == "int"){
                instructions[label].push_back("subl $4, \%esp");
                off_ = 4;
                structRetType = false;
            }else if(ijk.second->retType->name.substr(0,7)=="struct " && !checkPointer(ijk.second->retType)){
                for(auto jkl: gststruct.entries){
                    if(jkl.first == ijk.second->retType->name){
                        instructions[label].push_back("subl $"+to_string(jkl.second->size)+", \%esp");
                        off_ = jkl.second->size;
                        structRetType = true;
                    }
                }
            }else if(checkPointer(ijk.second->retType)){
                instructions[label].push_back("subl $4, \%esp");
                off_ = 4;
                structRetType = false;
            }
            // else if(!checkPointer(ijk.second->retType)){
            //     instructions[label].push_back("subl $4, \%esp");
            //     off_ = 4;
            //     structRetType = false; // check this
            // }
        }
    }
    offsetForFunction = off_;
    // push all the registers
    for(int i=0; i<6; i++){
        instructions[label].push_back("pushl "+registers[i]);
        // used.push(i);
    }
    vector<pair<int,int>> paramSizes;
    for(auto abc: gstfun.entries){
        if(abc.first == fname->name){
            for(auto xyz: abc.second->child->entries){
                paramSizes.push_back(make_pair(xyz.second->offset, xyz.second->size));
            }
        }
    }
    // sort the vector in the decreasing order of first value
    sort(paramSizes.begin(), paramSizes.end(), greater<pair<int,int>>());
    // for (auto it = args.begin(); it != args.end(); it++){
    //     (*it)->gencode("param");
    // }
    for(int ijk = 0; ijk<args.size();ijk++){
        sizeOfCorrespondingParam = paramSizes[ijk].second;
        args[ijk]->gencode("param");
    }
    int temp_count = used.size();
    // cout<<temp_count<<endl;
    for(int pl=0;pl<temp_count;pl++){
        rstack.push(used.top());
        used.pop();
    }
    instructions[label].push_back("subl $4, \%esp");
    instructions[label].push_back("call "+this->fname->name);
    int c__ = 0;
    for(int i=0; i<args.size(); i++){
        if(args[i]->type->name == "int")
            c__ += args[i]->type->size;
        else if(args[i]->type->name.substr(0,7) == "struct" && !checkPointer(args[i]->type)){
            c__ += args[i]->type->size;
        }else if(checkPointer(args[i]->type)){
            c__ += 4;
        }else if(!checkPointer(args[i]->type)){
            c__ += paramSizes[i].second;
        }
    }
    // cout<<func<<" "<<c__<<endl;
    // cout<<args.size()<<endl;
    // cout<<args[0]->type->size<<endl;
    // cout<<args[0]->type->name<<endl;
    instructions[label].push_back("addl $"+to_string(c__+4)+" , \%esp");
    for(int i=0; i<6; i++){
        instructions[label].push_back("popl "+registers[5-i]);
        // used.push(5-i);
    }
    for(int pl=0;pl<temp_count;pl++){
        used.push(rstack.top());
        rstack.pop();
    }
    if(offsetForFunction == 4 && !structRetType){
        instructions[label].push_back("popl "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }
    if(scope == "param" && offsetForFunction == 4){
        instructions[label].push_back("pushl "+registers[used.top()]);
        rstack.push(used.top());
        used.pop();
    }
    // else if(scope =="param" && offsetForFunction != 4){
    //     instructions[label].push_back("pushl "+registers[used.top()]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    }
}

void if_astnode::gencode(string scope){
    cond->gencode("exp");
    instructions[label].push_back("cmpl $0, "+registers[used.top()]);
    instructions[label].push_back("jne .L"+to_string(labelCount));
    int temp = labelCount;
    labelCount+=1;
    rstack.push(used.top());
    used.pop();
    else_->gencode("statement");
    instructions[label].push_back("jmp .L"+to_string(labelCount));
    instructions[label].push_back(".L"+to_string(temp)+":");
    temp = labelCount;
    labelCount+=1;
    then_->gencode("statement");
    instructions[label].push_back(".L"+to_string(temp)+":");
}

void while_astnode::gencode(string scope){
    instructions[label].push_back("jmp .L"+to_string(labelCount));
    int temp = labelCount;
    labelCount+=1;
    instructions[label].push_back(".L"+to_string(labelCount)+":");
    labelCount+=1;
    body->gencode("statement");
    instructions[label].push_back(".L"+to_string(temp)+":");
    cond->gencode("exp");
    instructions[label].push_back("cmpl $0, "+registers[used.top()]);
    instructions[label].push_back("jne .L"+to_string(temp+1));
    rstack.push(used.top());
    used.pop();
}

void for_astnode::gencode(string scope){
    init->gencode("exp");
    instructions[label].push_back("jmp .L"+to_string(labelCount));
    int temp = labelCount;
    labelCount+=1;
    instructions[label].push_back(".L"+to_string(labelCount)+":");
    labelCount+=1;
    body->gencode("statement");
    incr->gencode("exp");
    instructions[label].push_back(".L"+to_string(temp)+":");
    cond->gencode("exp");
    instructions[label].push_back("cmpl $0, "+registers[used.top()]);
    instructions[label].push_back("jne .L"+to_string(temp+1));
    rstack.push(used.top());
    used.pop();
}

void op_binary_astnode::gencode(string scope){
    // lhs->gencode();
    // rhs->gencode();
    // if(op=="PLUS_INT"){
    //     instructions[label].push_back("addl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="MINUS_INT"){
    //     instructions[label].push_back("subl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="MULT_INT"){
    //     instructions[label].push_back("imull "+registers[used.top()]+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="DIV_INT"){
    //     instructions[label].push_back("idivl "+registers[used.top()]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="PLUS_FLOAT"){
    //     instructions[func].push_back("addl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="MINUS_FLOAT"){
    //     instructions[func].push_back("subl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="MULT_FLOAT"){
    //     instructions[func].push_back("imull "+registers[used.top()]+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="DIV_FLOAT"){
    //     instructions[func].push_back("idivl "+registers[used.top()]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="EQ_OP_INT"){
    //     instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     instructions[label].push_back("sete \%"+registers[used.top()-1].substr(2,1)+"l");
    //     instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op=="NE_OP_INT"){
    //     instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     instructions[label].push_back("setne \%"+registers[used.top()-1].substr(2,1)+"l");
    //     instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op == "LT_OP_INT"){
    //     instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     instructions[label].push_back("setl \%"+registers[used.top()-1].substr(2,1)+"l");
    //     instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op == "LE_OP_INT"){
    //     instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     instructions[label].push_back("setle \%"+registers[used.top()-1].substr(2,1)+"l");
    //     instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op == "GT_OP_INT"){
    //     instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     instructions[label].push_back("setg \%"+registers[used.top()-1].substr(2,1)+"l");
    //     instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op == "GE_OP_INT"){
    //     instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
    //     instructions[label].push_back("setge \%"+registers[used.top()-1].substr(2,1)+"l");
    //     instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op == "OR_OP"){
    //     instructions[label].push_back("cmpl $0, "+registers[used.top()-1]);
    //     instructions[label].push_back("jne .L"+to_string(labelCount));
    //     instructions[label].push_back("cmpl $0, "+registers[used.top()]);
    //     instructions[label].push_back("je .L"+to_string(labelCount+1));
    //     instructions[label].push_back(".L"+to_string(labelCount)+":");
    //     instructions[label].push_back("movl $1, "+registers[used.top()-1]);
    //     instructions[label].push_back("jmp .L"+to_string(labelCount+2));
    //     instructions[label].push_back(".L"+to_string(labelCount+1)+":");
    //     instructions[label].push_back("movl $0, "+registers[used.top()-1]);
    //     instructions[label].push_back(".L"+to_string(labelCount+2)+":");
    //     labelCount+=3;
    //     rstack.push(used.top());
    //     used.pop();
    // }
    // else if(op == "AND_OP"){
    //     instructions[label].push_back("cmpl $0, "+registers[used.top()-1]);
    //     instructions[label].push_back("je .L"+to_string(labelCount));
    //     instructions[label].push_back("cmpl $0, "+registers[used.top()]);
    //     instructions[label].push_back("je .L"+to_string(labelCount));
    //     instructions[label].push_back("movl $1, "+registers[used.top()-1]);
    //     instructions[label].push_back("jmp .L"+to_string(labelCount+1));
    //     instructions[label].push_back(".L"+to_string(labelCount)+":");
    //     instructions[label].push_back("movl $0, "+registers[used.top()-1]);
    //     instructions[label].push_back(".L"+to_string(labelCount+1)+":");
    //     labelCount+=2;
    //     rstack.push(used.top());
    //     used.pop();
    // }

    // cout<<scope<<" "<<op<<endl;
    if(scope == "exp" || scope == "param"){
        if(op == "OR_OP"){
            lhs->gencode("exp");
            instructions[label].push_back("cmpl $0, "+registers[used.top()]);
            instructions[label].push_back("jne .L"+to_string(labelCount));
            int temp = labelCount;
            labelCount++;
            rstack.push(used.top());
            used.pop();
            rhs->gencode("exp");
            instructions[label].push_back("cmpl $0, "+registers[used.top()]);
            instructions[label].push_back("je .L"+to_string(labelCount));
            labelCount++;
            instructions[label].push_back(".L"+to_string(temp)+":");
            instructions[label].push_back("movl $1, "+registers[used.top()]);
            instructions[label].push_back("jmp .L"+to_string(labelCount));
            instructions[label].push_back(".L"+to_string(labelCount-1)+":");
            instructions[label].push_back("movl $0, "+registers[used.top()]);
            instructions[label].push_back(".L"+to_string(labelCount)+":");
            labelCount++;
            // rstack.push(used.top());
            // used.pop();
        }else if(op == "AND_OP"){
            lhs->gencode("exp");
            instructions[label].push_back("cmpl $0, "+registers[used.top()]);
            instructions[label].push_back("je .L"+to_string(labelCount));
            int temp = labelCount;
            labelCount++;
            rstack.push(used.top());
            used.pop();
            rhs->gencode("exp");
            instructions[label].push_back("cmpl $0, "+registers[used.top()]);
            instructions[label].push_back("jne .L"+to_string(labelCount));
            labelCount++;
            instructions[label].push_back(".L"+to_string(temp)+":");
            instructions[label].push_back("movl $0, "+registers[used.top()]);
            instructions[label].push_back("jmp .L"+to_string(labelCount));
            instructions[label].push_back(".L"+to_string(labelCount-1)+":");
            instructions[label].push_back("movl $1, "+registers[used.top()]);
            instructions[label].push_back(".L"+to_string(labelCount)+":");
            labelCount++;
            // rstack.push(used.top());
            // used.pop();
        }else{
            rhs->gencode("exp");
            instructions[label].push_back("pushl "+registers[used.top()]);
            rstack.push(used.top());
            used.pop();
            lhs->gencode("exp");
            instructions[label].push_back("popl "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
            if(op == "PLUS_INT"){
                if(rhs->type->name == "int" && lhs->type->name == "int"){
                    instructions[label].push_back("addl "+registers[used.top()]+", "+registers[used.top()-1]);
                    rstack.push(used.top());
                    used.pop();
                }else if(rhs->type->name == "int" && lhs->type->name != "int"){
                    int size___  = sizeOfAdd(lhs->type, gst);
                    instructions[label].push_back("imull $"+to_string(size___)+", "+registers[used.top()]);
                    instructions[label].push_back("addl "+registers[used.top()]+", "+registers[used.top()-1]);
                    rstack.push(used.top());
                    used.pop();
                }else if(rhs->type->name != "int" && lhs->type->name == "int"){
                    int size___  = sizeOfAdd(rhs->type, gst);
                    instructions[label].push_back("imull $"+to_string(size___)+", "+registers[used.top()-1]);
                    instructions[label].push_back("addl "+registers[used.top()]+", "+registers[used.top()-1]);
                    rstack.push(used.top());
                    used.pop();
                }
            }else if(op == "MINUS_INT"){
                if(rhs->type->name == "int" && lhs->type->name == "int"){
                    instructions[label].push_back("subl "+registers[used.top()]+", "+registers[used.top()-1]);
                    rstack.push(used.top());
                    used.pop();
                }else if(rhs->type->name == "int" && lhs->type->name != "int"){
                    int size___  = sizeOfAdd(lhs->type, gst);
                    instructions[label].push_back("imull $"+to_string(size___)+", "+registers[used.top()]);
                    instructions[label].push_back("subl "+registers[used.top()]+", "+registers[used.top()-1]);
                    rstack.push(used.top());
                    used.pop();
                }else if(lhs->type->name != "int" && rhs->type->name != "int"){
                    int size___  = lhs->type->size;
                    instructions[label].push_back("subl "+registers[used.top()]+", "+registers[used.top()-1]);
                }
            }else if(op == "MULT_INT"){
                instructions[label].push_back("imull "+registers[used.top()]+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }else if(op == "DIV_INT"){
                int gh = used.size();
                if(gh==2){
                    instructions[label].push_back("movl $0, \%edx");
                    instructions[label].push_back("cltd");
                    instructions[label].push_back("idivl "+registers[used.top()]);
                }else{
                    for(int jkl=0;jkl<gh-2;jkl++){
                        instructions[label].push_back("pushl "+registers[used.top()-jkl-2]);
                    }
                    instructions[label].push_back("movl "+registers[used.top()-1]+", \%eax");
                    instructions[label].push_back("movl $0, \%edx");
                    instructions[label].push_back("cltd");
                    instructions[label].push_back("idivl "+registers[used.top()]);
                    instructions[label].push_back("movl \%eax, "+registers[used.top()-1]);
                    for(int jkl=gh-3;jkl>=0;jkl--){
                        instructions[label].push_back("popl "+registers[used.top()-jkl-2]);
                    }
                }
                rstack.push(used.top());
                used.pop();
            }else if(op == "EQ_OP_INT"){
                instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
                instructions[label].push_back("sete \%"+registers[used.top()-1].substr(2,1)+"l");
                instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }else if(op == "NE_OP_INT"){
                instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
                instructions[label].push_back("setne \%"+registers[used.top()-1].substr(2,1)+"l");
                instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }else if(op == "LT_OP_INT"){
                instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
                instructions[label].push_back("setl \%"+registers[used.top()-1].substr(2,1)+"l");
                instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }else if(op == "GT_OP_INT"){
                instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
                instructions[label].push_back("setg \%"+registers[used.top()-1].substr(2,1)+"l");
                instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }else if(op == "LE_OP_INT"){
                instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
                instructions[label].push_back("setle \%"+registers[used.top()-1].substr(2,1)+"l");
                instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }else if(op == "GE_OP_INT"){
                instructions[label].push_back("cmpl "+registers[used.top()]+", "+registers[used.top()-1]);
                instructions[label].push_back("setge \%"+registers[used.top()-1].substr(2,1)+"l");
                instructions[label].push_back("movzbl \%"+registers[used.top()-1].substr(2,1)+"l"+", "+registers[used.top()-1]);
                rstack.push(used.top());
                used.pop();
            }
        }
    }
    if(scope == "param"){
        // sizeOfParams+=4;
        instructions[label].push_back("pushl "+registers[used.top()]);
        rstack.push(used.top());
        used.pop();
    }
    
}

void op_unary_astnode::gencode(string scope){
    // cout<<"op_unary_astnode"<<endl;
    // cout<<scope<<endl;
    if(scope == "exp" || scope == "param"){
        if(op == "UMINUS"){
            exp->gencode("exp");
            instructions[label].push_back("negl "+registers[used.top()]);
        }else if(op == "NOT"){
            exp->gencode("exp");
            instructions[label].push_back("cmpl $0, "+registers[used.top()]);
            instructions[label].push_back("sete \%"+registers[used.top()].substr(2,1)+"l");
            instructions[label].push_back("movzbl \%"+registers[used.top()].substr(2,1)+"l"+", "+registers[used.top()]);
        }else if(op == "PP"){
            // cout<<"PP"<<endl;
            exp->gencode("exp");
            if(scope == "param"){
                instructions[label].push_back("pushl "+registers[used.top()]);
            }
            /*instructions[label].push_back("movl "+registers[used.top()]+", "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
            instructions[label].push_back("addl $1, "+registers[used.top()]);
            instructions[label].push_back("movl "+registers[used.top()]+", "+to_string(offsetForPP)+"(%ebp)");
            rstack.push(used.top());
            used.pop();*/
            // cout<<"movl "<<registers[used.top()]<<", "<<registers[rstack.top()]<<endl;
            instructions[label].push_back("movl "+registers[used.top()]+", "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
            // cout<<"addl $1, "<<registers[used.top()]<<endl;
            instructions[label].push_back("add $1, "+registers[used.top()]);
            exp->gencode("lhs");
        }else if(op == "ADDRESS"){
            exp->gencode("addr");
        }else if(op == "DEREF"){
            exp->gencode("exp");
            // if(checkPointer(exp->type)){
            //     instructions[label].push_back("movl ("+registers[used.top()]+"), "+registers[used.top()]);
            if(this->type->name == "int"){
                instructions[label].push_back("movl ("+registers[used.top()]+"), "+registers[used.top()]);
            }
        }
    }
    if(scope == "param" && op != "PP"){
        instructions[label].push_back("pushl "+registers[used.top()]);
        rstack.push(used.top());
        used.pop();
    }
    if(scope == "lhs" && op == "DEREF"){
        exp->gencode("exp");
        instructions[label].push_back("movl "+registers[used.top()-1]+", ("+registers[used.top()]+")");
        rstack.push(used.top());
        used.pop();
    }
}

void assignE_astnode::gencode(string scope){
    rhs->gencode("exp");
    lhs->gencode("lhs");
}

void funcall_astnode::gencode(string scope){
    int off_ = 0;
    for(auto ijk: gstfun.entries){
        if(ijk.first == fname->name){
            if(ijk.second->retType->name == "int"){
                instructions[label].push_back("subl $4, \%esp");
                off_ = 4;
                structRetType = false;
            }else if(ijk.second->retType->name.substr(0,7)=="struct " && !checkPointer(ijk.second->retType)){
                for(auto jkl: gststruct.entries){
                    if(jkl.first == ijk.second->retType->name){
                        instructions[label].push_back("subl $"+to_string(jkl.second->size)+", \%esp");
                        off_ = jkl.second->size;
                        structRetType = true;
                    }
                }
            }else if(checkPointer(ijk.second->retType)){
                instructions[label].push_back("subl $4, \%esp");
                off_ = 4;
                structRetType = false;
            }
            // else if(!checkPointer(ijk.second->retType)){
            //     instructions[label].push_back("subl $4, \%esp");
            //     off_ = 4;
            //     structRetType = false; // check this
            // }
        }
    }
    offsetForFunction = off_;
    // push all the registers
    for(int i=0; i<6; i++){
        instructions[label].push_back("pushl "+registers[i]);
        // used.push(i);
    }
    vector<pair<int,int>> paramSizes;
    for(auto abc: gstfun.entries){
        if(abc.first == fname->name){
            for(auto xyz: abc.second->child->entries){
                paramSizes.push_back(make_pair(xyz.second->offset, xyz.second->size));
            }
        }
    }
    // sort the vector in the decreasing order of first value
    sort(paramSizes.begin(), paramSizes.end(), greater<pair<int,int>>());
    // for (auto it = args.begin(); it != args.end(); it++){
    //     (*it)->gencode("param");
    // }
    for(int ijk = 0; ijk<args.size();ijk++){
        sizeOfCorrespondingParam = paramSizes[ijk].second;
        args[ijk]->gencode("param");
    }
    int temp_count = used.size();
    // cout<<temp_count<<endl;
    for(int pl=0;pl<temp_count;pl++){
        rstack.push(used.top());
        used.pop();
    }
    instructions[label].push_back("subl $4, \%esp");
    instructions[label].push_back("call "+this->fname->name);
    int c__ = 0;
    for(int i=0; i<args.size(); i++){
        if(args[i]->type->name == "int")
            c__ += args[i]->type->size;
        else if(args[i]->type->name.substr(0,7) == "struct" && !checkPointer(args[i]->type)){
            c__ += args[i]->type->size;
        }else if(checkPointer(args[i]->type)){
            c__ += 4;
        }else if(!checkPointer(args[i]->type)){
            c__ += paramSizes[i].second;
        }
    }
    // cout<<func<<" "<<c__<<endl;
    // cout<<args.size()<<endl;
    // cout<<args[0]->type->size<<endl;
    // cout<<args[0]->type->name<<endl;
    instructions[label].push_back("addl $"+to_string(c__+4)+" , \%esp");
    for(int i=0; i<6; i++){
        instructions[label].push_back("popl "+registers[5-i]);
        // used.push(5-i);
    }
    for(int pl=0;pl<temp_count;pl++){
        used.push(rstack.top());
        rstack.pop();
    }
    if(offsetForFunction == 4 && !structRetType){
        instructions[label].push_back("popl "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }
    if(scope == "param" && offsetForFunction == 4){
        instructions[label].push_back("pushl "+registers[used.top()]);
        rstack.push(used.top());
        used.pop();
    }
    // else if(scope =="param" && offsetForFunction != 4){
    //     instructions[label].push_back("pushl "+registers[used.top()]);
    //     rstack.push(used.top());
    //     used.pop();
    // }
    
}

void intconst_astnode::gencode(string scope){
    if(scope == "param"){
        instructions[label].push_back("pushl  $"+to_string(this->value));
    }else{
        instructions[label].push_back("movl  $"+to_string(this->value)+", "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }
}

void floatconst_astnode::gencode(string scope){
    if(scope == "param"){
        instructions[label].push_back("pushl  $"+to_string(this->value));
    }else{
        instructions[label].push_back("movl  $"+to_string(this->value)+", "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }
}

void stringconst_astnode::gencode(string scope){
    if(isPrintf){
        instructions[".LC"+to_string(printfCount)].push_back(".string " + this->value);
        instructions[".LC"+to_string(printfCount)].push_back(".text");
        instructions[".LC"+to_string(printfCount)].push_back(".globl "+func);
        instructions[".LC"+to_string(printfCount)].push_back(".type "+func+", @function");
    }
}

void identifier_astnode::gencode(string scope){
    int offset_ = getOffset(localst, this->name);
    identifierForStruct = this->name;
    offsetForStruct = 0;
    offsetForStructPtr = 0;
    offsetForArray = 0;
    identifierForArray = this->name;
    if(scope == "param" && this->type->name == "int"){
        instructions[label].push_back("pushl "+to_string(offset_)+"(\%ebp)");
    }else if(scope == "exp" && this->type->name == "int"){
        offsetForPP = offset_;
        instructions[label].push_back("movl  "+to_string(offset_)+"(\%ebp), "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }else if(scope == "lhs" && this->type->name == "int"){
        instructions[label].push_back("movl  "+registers[used.top()]+", "+to_string(offset_)+"(\%ebp)");
        rstack.push(used.top());
        used.pop();
    }else if(scope == "struct"){
        for(auto i: localst->entries){
            if(i.first == identifierForStruct){
                offsetForStruct += i.second->offset;
                typeForStruct = i.second->retType;
            }
        }
    }else if(scope == "param" && this->type->name.substr(0,7) == "struct " && !checkPointer(this->type) && this->type->fields.size() == 0){
        for(auto i: gststruct.entries){
            if(i.first == this->type->name){
                vector<pair<int, string>> temp;
                for(auto j: i.second->child->entries){
                    temp.push_back(make_pair(j.second->offset, j.first));
                }
                sort(temp.begin(), temp.end(),greater<pair<int, string>>());
                for(auto j: temp){
                    instructions[label].push_back("pushl "+to_string(offset_+j.first)+"(\%ebp)");
                }
            }
        }
    }else if(scope == "exp" && this->type->name.substr(0,7) == "struct " && !checkPointer(this->type) && this->type->fields.size() == 0){
        offsetForPP = offset_;
        for(auto i: gststruct.entries){
            if(i.first == this->type->name){
                vector<pair<int, string>> temp;
                for(auto j: i.second->child->entries){
                    temp.push_back(make_pair(j.second->offset, j.first));
                }
                sort(temp.begin(), temp.end(), greater<pair<int, string>>());
                for(auto j: temp){
                    instructions[label].push_back("pushl "+to_string(offset_+j.first)+"(\%ebp)");
                }
            }
        }
    }else if(scope == "lhs" && this->type->name.substr(0,7) == "struct " && !checkPointer(this->type) && this->type->fields.size() == 0){
        for(auto i: gststruct.entries){
            if(i.first == this->type->name){
                vector<pair<int, string>> temp;
                for(auto j: i.second->child->entries){
                    temp.push_back(make_pair(j.second->offset, j.first));
                }
                sort(temp.begin(), temp.end());
                for(auto j: temp){
                    instructions[label].push_back("popl "+registers[rstack.top()]);
                    instructions[label].push_back("movl  "+registers[rstack.top()]+", "+to_string(offset_+j.first)+"(\%ebp)");
                }
            }
        }
    }else if(scope == "addr"){
        instructions[label].push_back("leal  "+to_string(offset_)+"(\%ebp), "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }else if(scope == "structPtr"){
        for(auto i: localst->entries){
            if(i.first == identifierForStruct){
                offsetForStructPtr += i.second->offset;
                typeForStructPtr = i.second->retType;
            }
        }
        instructions[label].push_back("movl  "+to_string(offsetForStructPtr)+"(\%ebp), "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }else if(scope == "lhs" && this->type->name.substr(0,7) == "struct " && (checkPointer(this->type))){
        instructions[label].push_back("movl "+registers[used.top()]+", "+to_string(offset_)+"(\%ebp)");
        rstack.push(used.top());
        used.pop();
    }else if(scope == "lhs" && this->type->name.substr(0,7) == "struct " && (!checkPointer(this->type) && this->type->fields.size() > 0)){
        instructions[label].push_back("movl "+registers[used.top()]+", "+to_string(offset_)+"(\%ebp)");
        rstack.push(used.top());
        used.pop();
    }else if(scope == "param" && this->type->name.substr(0,7) == "struct " && (checkPointer(this->type))){
        instructions[label].push_back("pushl "+to_string(offset_)+"(\%ebp)");
    }else if(scope == "param" && this->type->name.substr(0,7) == "struct " && (!checkPointer(this->type) && this->type->fields.size() > 0)){
        instructions[label].push_back("leal "+to_string(offset_)+"(\%ebp), "+registers[rstack.top()]);
        instructions[label].push_back("pushl "+registers[rstack.top()]);
    }
    else if(scope == "exp" && this->type->name.substr(0,7) == "struct " && (checkPointer(this->type))){
        instructions[label].push_back("movl  "+to_string(offset_)+"(\%ebp), "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }else if(scope == "exp" && this->type->name.substr(0,7) == "struct " && (!checkPointer(this->type) && this->type->fields.size() > 0)){
        instructions[label].push_back("leal  "+to_string(offset_)+"(\%ebp), "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }
    else if(scope == "array"){
        for(auto i: localst->entries){
            if(i.first == identifierForArray){
                local_ = i.second->scope == "local";
                offsetForArray += i.second->offset;
                typeForArray = i.second->retType;
                offsetForStruct += i.second->offset;
                typeForStruct = i.second->retType;
            }
        }
        if(local_ && !checkPointer(typeForArray)){
            instructions[label].push_back("leal  "+to_string(offsetForArray)+"(\%ebp), "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
        }else{
            instructions[label].push_back("movl "+to_string(offsetForArray)+"(\%ebp), "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
        }
    }else if(scope == "param" && !checkPointer(this->type)){
        instructions[label].push_back("subl $"+to_string(sizeOfCorrespondingParam-4)+", \%esp");
        instructions[label].push_back("leal "+to_string(offset_)+"(\%ebp) , "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
        instructions[label].push_back("pushl "+registers[used.top()]);
        rstack.push(used.top());
        used.pop();
    }else if(scope == "param" && checkPointer(this->type)){
        instructions[label].push_back("pushl "+to_string(offset_)+"(\%ebp)");
    }else if(scope == "exp" && !checkPointer(this->type)){
        instructions[label].push_back("leal "+to_string(offset_)+"(\%ebp) , "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }else if(scope == "exp" && checkPointer(this->type)){
        instructions[label].push_back("movl "+to_string(offset_)+"(\%ebp), "+registers[rstack.top()]);
        used.push(rstack.top());
        rstack.pop();
    }else if(scope == "lhs" && !checkPointer(this->type)){
        instructions[label].push_back("movl "+registers[used.top()]+", "+to_string(offset_)+"(\%ebp)");
        rstack.push(used.top());
        used.pop();
    }else if(scope == "lhs" && checkPointer(this->type)){
        instructions[label].push_back("movl "+registers[used.top()]+", "+to_string(offset_)+"(\%ebp)");
        rstack.push(used.top());
        used.pop();
    }   
}

void arrayref_astnode::gencode(string scope){
    array->gencode("array");
    index->gencode("exp");
    instructions[label].push_back("imull $"+to_string(this->type->size)+", "+registers[used.top()]);
    instructions[label].push_back("addl "+registers[used.top()]+", "+registers[used.top()-1]);
    rstack.push(used.top());
    used.pop();
    if(scope == "exp"){
        if(this->type->name == "int"){
            instructions[label].push_back("movl  ("+registers[used.top()]+"), "+registers[used.top()]);
        }
    }else if(scope == "lhs"){
        structArray = false;
        if(this->type->name == "int"){
            instructions[label].push_back("movl "+registers[used.top()-1]+", ("+registers[used.top()]+")");
            rstack.push(used.top());
            used.pop();
            rstack.push(used.top());
            used.pop();
        }else{
            instructions[label].push_back("movl "+registers[used.top()-1]+", "+registers[used.top()]);
            rstack.push(used.top());
            used.pop();
            rstack.push(used.top());
            used.pop();
        }
    }else if(scope == "param"){
        structArray = false;
        if(this->type->name == "int"){
            instructions[label].push_back("pushl ("+registers[used.top()]+")");
            rstack.push(used.top());
            used.pop();
        }else{
            instructions[label].push_back("subl $"+to_string(this->type->size-4)+", \%esp");
            instructions[label].push_back("pushl "+registers[used.top()]);
            rstack.push(used.top());
            used.pop();
        }
    }else if(scope == "addr"){
        structArray = false;
        // instructions[label].push_back("leal  ("+registers[used.top()]+"), "+registers[used.top()]);
    }else if(scope == "struct"){
        structArray = true;
        typeForStruct = this->type;
        // instructions[label].push_back("leal ("+registers[used.top()]+"), "+registers[used.top()]);
    }

}

void member_astnode::gencode(string scope){
    exp->gencode("struct");
    int temp_count = 0;
    for(auto i: gststruct.entries){
        if(i.first == typeForStruct->name){
            for(auto j: i.second->child->entries){
                if(j.first == this->mem->name){
                    offsetForStruct += j.second->offset;
                    temp_count = j.second->offset;
                    typeForStruct = j.second->retType;
                }
            }
        }
    }
    if(scope == "exp"){
        if(!structArray){
            offsetForPP = offsetForStruct;
            instructions[label].push_back("movl  "+to_string(offsetForStruct)+"(\%ebp), "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
        }else{
            instructions[label].push_back("addl $"+to_string(temp_count)+", "+registers[used.top()]);
        }
    }else if(scope == "lhs"){
        if(!structArray){
            instructions[label].push_back("movl  "+registers[used.top()]+", "+to_string(offsetForStruct)+"(\%ebp)");
            rstack.push(used.top());
            used.pop();
        }else{
            instructions[label].push_back("addl $"+to_string(temp_count)+", "+registers[used.top()]);
            instructions[label].push_back("movl  "+registers[used.top()-1]+", "+"("+registers[used.top()]+")");
            rstack.push(used.top());
            used.pop();
            rstack.push(used.top());
            used.pop();
        }
    }else if(scope == "param"){
        if(!structArray){
            instructions[label].push_back("pushl "+to_string(offsetForStruct)+"(\%ebp)");
        }else{
            if(this->type->name == "int"){
                instructions[label].push_back("addl $"+to_string(temp_count)+", "+registers[used.top()]);
                instructions[label].push_back("pushl ("+registers[used.top()]+")");
                rstack.push(used.top());
                used.pop();
            }else{
                instructions[label].push_back("addl $"+to_string(temp_count)+", "+registers[used.top()]);
                instructions[label].push_back("pushl "+registers[used.top()]);
                rstack.push(used.top());
                used.pop();
            }
        }
    }else if(scope == "addr"){
        if(!structArray){
            instructions[label].push_back("leal  "+to_string(offsetForStruct)+"(\%ebp), "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
        }
    }else if(scope == "array"){
        if(!structArray){
            instructions[label].push_back("leal  "+to_string(offsetForStruct)+"(\%ebp), "+registers[rstack.top()]);
            used.push(rstack.top());
            rstack.pop();
        }else{
            instructions[label].push_back("addl $"+to_string(temp_count)+", "+registers[used.top()]);
        }
        // instructions[label].push_back("leal "+to_string(offsetForStruct)+"(\%ebp), "+registers[rstack.top()]);
        // used.push(rstack.top());
        // rstack.pop();
    }
}


void arrow_astnode::gencode(string scope){
    exp->gencode("structPtr");
    for(auto i: gststruct.entries){
        if(i.first + "*" == typeForStructPtr->name  || i.first + "(*)" == typeForStructPtr->name || i.first == typeForStructPtr->name.substr(0, i.first.length())){
            for(auto j: i.second->child->entries){
                if(j.first == this->mem->name){
                    offsetForStructPtr = j.second->offset;
                    typeForStructPtr = j.second->retType;
                }
            }
        }
    }
    if(scope == "exp"){
        instructions[label].push_back("movl  "+to_string(offsetForStructPtr)+"("+registers[used.top()]+"), "+registers[used.top()]);
    }else if(scope == "lhs"){
        instructions[label].push_back("movl  "+registers[used.top()-1]+", "+to_string(offsetForStructPtr)+"("+registers[used.top()]+")");
        rstack.push(used.top());
        used.pop();
        rstack.push(used.top());
        used.pop();
    }else if(scope == "param"){
        instructions[label].push_back("pushl "+to_string(offsetForStructPtr)+"("+registers[used.top()]+")");
        rstack.push(used.top());
        used.pop();
    }else if(scope == "addr"){
        instructions[label].push_back("leal  "+to_string(offsetForStructPtr)+"("+registers[used.top()]+"), "+registers[used.top()]);
    }else if(scope == "array"){
        instructions[label].push_back("leal  "+to_string(offsetForStructPtr)+"("+registers[used.top()]+"), "+registers[used.top()]);
    }
}

