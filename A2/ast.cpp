#include "ast.hh"
#include <iostream>
#include <cstdarg>
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



