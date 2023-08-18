#include "scanner.hh"
// #include "parser.tab.hh"
#include <fstream>
#include <stack>
using namespace std;

SymbolTable gst, gstfun, gststruct; 
string filename;
extern std::map<string,abstract_astnode*> ast;
std::map<std::string, Type*> predefined {
            {"printf", createtype("void")},
            {"scanf", createtype("void")},
            {"mod", createtype("int")}
        };

string registers[6] = {"\%eax","\%ebx","\%ecx","\%edx","\%esi","\%edi"};
stack<int> rstack;
string func = "";
SymbolTable* localst;
map<string, vector<string>> instructions;
int printCount = 0;
map<string, int> printCountMap;
map<string, int> functionSize;
int labelCount = 1;
string label = "";

int main(int argc, char **argv)
{
	using namespace std;
	fstream in_file, out_file;
	filename = argv[1];
	in_file.open(argv[1], ios::in);

	IPL::Scanner scanner(in_file);

	IPL::Parser parser(scanner);

#ifdef YYDEBUG
	parser.set_debug_level(1);
#endif
parser.parse();
// }
// create gstfun with function entries only

for (const auto &entry : gst.entries)
{
	if (entry.second->varfuntype == "fun")
	gstfun.entries.insert({entry.first, entry.second});
}
// create gststruct with struct entries only

for (const auto &entry : gst.entries)
{
	if (entry.second->varfuntype == "struct")
	gststruct.entries.insert({entry.first, entry.second});
}
// start the JSON printing

cout<<"\t.file\t\""<<filename<<"\""<<endl;
cout<<"\t.text"<<endl;
// cout<<"\t.section\t.rodata"<<endl;


for(int i=5;i>=0;i--)
{
	rstack.push(i);
}

// cout << "{\"globalST\": " << endl;
// gst.printgst();
// cout << "," << endl;

// cout << "  \"structs\": [" << endl;
// for (auto it = gststruct.entries.begin(); it != gststruct.entries.end(); ++it)

// {   cout << "{" << endl;
// 	cout << "\"name\": " << "\"" << it->first << "\"," << endl;
// 	cout << "\"localST\": " << endl;
// 	it->second->child->print();
// 	cout << "}" << endl;
// 	if (next(it,1) != gststruct.entries.end()) 
// 	cout << "," << endl;
// }
// cout << "]," << endl;
// cout << "  \"functions\": [" << endl;

// for (auto ita = gstfun.entries.begin(); ita != gstfun.entries.end(); ++ita)
// {
// 	func = ita->first;
// 	if(func!="main") continue;
// 	localst = (ita->second->child);
// 	labelCount++;
// 	label = func;
// 	// cout<<func<<":"<<endl;
// 	instructions[func].push_back("pushl \%ebp");
// 	instructions[func].push_back("movl \%esp, \%ebp");
// 	int totalSize = 0;
// 	for (auto ab = localst->entries.begin(); ab != localst->entries.end(); ++ab)
// 	{
// 		if(ab->second->scope=="local")
// 			totalSize += ab->second->size;
// 	}
// 	if(totalSize%4!=0){
// 		totalSize = totalSize + (4-totalSize%4);
// 	}
// 	functionSize[func] = totalSize;
// 	// cout<<totalSize<<endl;
// 	if(totalSize!=0)
// 	instructions[func].push_back("subl $"+to_string(totalSize)+", \%esp");

// 	// cout<<"start of function"<<endl;
// 	// ast[ita->first]->print(0);
// 	ast[ita->first]->gencode("");
// 	// cout<<"end of function"<<endl;
// }

for (auto ita = gstfun.entries.begin(); ita != gstfun.entries.end(); ++ita)

{
	// cout << "{" << endl;
	// cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	// cout << "\"localST\": " << endl;
	// it->second->child->print();
	// cout << "," << endl;
	// cout << "\"ast\": " << endl;
	// ast[it->first]->print(0);
	// cout << "}" << endl;
	// if (next(it,1) != gstfun.entries.end()) cout << "," << endl;
	func = ita->first;
	localst = (ita->second->child);
	labelCount++;
	label = func;
	// cout<<func<<":"<<endl;
	instructions[func].push_back("pushl \%ebp");
	instructions[func].push_back("movl \%esp, \%ebp");
	int totalSize = 0;
	for (auto ab = localst->entries.begin(); ab != localst->entries.end(); ++ab)
	{
		if(ab->second->scope=="local")
			totalSize += ab->second->size;
	}
	if(totalSize%4!=0){
		totalSize = totalSize + (4-totalSize%4);
	}
	functionSize[func] = totalSize;
	// cout<<totalSize<<endl;
	if(totalSize!=0)
	instructions[func].push_back("subl $"+to_string(totalSize)+", \%esp");

	// cout<<"start of function"<<endl;
	// ast[ita->first]->print(0);
	// if(func!="main")
	ast[ita->first]->gencode("");
	// cout<<"end of function"<<endl;

	// for (auto ab = instructions.begin(); ab != instructions.end(); ++ab)
	// {
	// 	if(ab->first.substr(0,3)==".LC"){
	// 		printCount++;
	// 	}
	// }

	// cout<<printCountMap[func]<<endl;

	if(printCountMap[func]>0){
		cout<<"\t.section\t.rodata"<<endl;
	}else{
		cout<<"\t.globl\t"+func<<endl;
		cout<<"\t.type\t"+func+", @function"<<endl;
	}

	for (auto it = instructions.begin(); it != instructions.end(); ++it)
	{
		// cout<<func<<" "<<printCountMap[func]<<" "<<printCount<<endl;
		if(it->first.substr(0,3)==".LC" && stoi(it->first.substr(3,it->first.size()-3))<printCount+printCountMap[func]-1 && stoi(it->first.substr(3,it->first.size()-3))>=printCount){
			cout<<it->first<<":"<<endl;
			cout<<"\t"<<it->second[0]<<endl;
		}else if(it->first.substr(0,3)==".LC" && stoi(it->first.substr(3,it->first.size()-3))==printCount+printCountMap[func]-1 && stoi(it->first.substr(3,it->first.size()-3))>=printCount){
			cout<<it->first<<":"<<endl;
			for(int i=0;i<it->second.size();i++)
			{
				cout<<"\t"<<it->second[i]<<endl;
			}
		}else if(it->first[0]!='.' && it->first==func){
			cout<<it->first<<":"<<endl;
			for(int i=0;i<it->second.size();i++)
			{
				if(it->second[i][0]=='.')
					cout
					<<it->second[i]<<endl;
				else
					cout<<"\t"<<it->second[i]<<endl;
			}
			// check if ret is there
			if(it->second[it->second.size()-1].substr(0,3)!="ret"){
				cout<<"\tleave"<<endl;
				cout<<"\tret"<<endl;
			}
		}
	}
	for(auto ijk = instructions.begin(); ijk != instructions.end(); ++ijk)
	{
		if(ijk->first[0]=='.' && ijk->first.substr(0,3)!=".LC"){
			cout<<ijk->first<<":"<<endl;
			for(int i=0;i<ijk->second.size();i++)
			{
				cout<<"\t"<<ijk->second[i]<<endl;
			}
		}
	}
	printCount+=printCountMap[func];
	
}
	// cout << "]" << endl;
	// cout << "}" << endl;

	// print the instructions
	//count the prints
	// for (auto ab = instructions.begin(); ab != instructions.end(); ++ab)
	// {
	// 	if(ab->first.substr(0,3)==".LC"){
	// 		printCount++;
	// 	}
	// }

	// // cout<<printCount<<endl;

	// for (auto it = instructions.begin(); it != instructions.end(); ++it)
	// {
	// 	if(it->first.substr(0,3)==".LC" && stoi(it->first.substr(3,it->first.size()-3))!=printCount-1){
	// 		cout<<it->first<<":"<<endl;
	// 		cout<<"\t"<<it->second[0]<<endl;
	// 		// cout<<it->first.substr(3,it->first.size()-3)<<":"<<endl;
	// 		// for(int i=0;i<it->second.size();i++)
	// 		// {
	// 		// 	cout<<"\t"<<it->second[i]<<endl;
	// 		// }
	// 	}else if(it->first.substr(0,3)==".LC" && stoi(it->first.substr(3,it->first.size()-3))==printCount-1){
	// 		cout<<it->first<<":"<<endl;
	// 		// cout<<it->first.substr(3,it->first.size()-3)<<":"<<endl;
	// 		for(int i=0;i<it->second.size();i++)
	// 		{
	// 			cout<<"\t"<<it->second[i]<<endl;
	// 		}
	// 	}else if(it->first[0]!='.'){
	// 		cout<<it->first<<":"<<endl;
	// 		for(int i=0;i<it->second.size();i++)
	// 		{
	// 			if(it->second[i][0]=='.')
	// 				cout
	// 				<<it->second[i]<<endl;
	// 			else
	// 				cout<<"\t"<<it->second[i]<<endl;
	// 		}
	// 	}
	// }
	// for(auto ijk = instructions.begin(); ijk != instructions.end(); ++ijk)
	// {
	// 	if(ijk->first[0]=='.' && ijk->first.substr(0,3)!=".LC"){
	// 		cout<<ijk->first<<":"<<endl;
	// 		for(int i=0;i<ijk->second.size();i++)
	// 		{
	// 			cout<<"\t"<<ijk->second[i]<<endl;
	// 		}
	// 	}
	// }

	fclose(stdout);
}
// void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
// {   
// 	typedef vector<abstract_astnode *>* pv;
// 	va_list args;
// 	va_start(args, fmt);
// 	if ((astname != NULL) && (astname[0] != '\0'))
// 	{
// 		cout << "{ ";
// 		cout << "\"" << astname << "\"" << ": ";
// 	}
// 	cout << "{" << endl;
// 	while (*fmt != '\0')
// 	{
// 		if (*fmt == 'a')
// 		{
// 			char * field = va_arg(args, char *);
// 			abstract_astnode *a = va_arg(args, abstract_astnode *);
// 			cout << "\"" << field << "\": " << endl;
			
// 			a->print(0);
// 		}
// 		else if (*fmt == 's')
// 		{
// 			char * field = va_arg(args, char *);
// 			char *str = va_arg(args, char *);
// 			cout << "\"" << field << "\": ";

// 			cout << str << endl;
// 		}
// 		else if (*fmt == 'i')
// 		{
// 			char * field = va_arg(args, char *);
// 			int i = va_arg(args, int);
// 			cout << "\"" << field << "\": ";

// 			cout << i;
// 		}
// 		else if (*fmt == 'f')
// 		{
// 			char * field = va_arg(args, char *);
// 			double f = va_arg(args, double);
// 			cout << "\"" << field << "\": ";
// 			cout << f;
// 		}
// 		else if (*fmt == 'l')
// 		{
// 			char * field = va_arg(args, char *);
// 			pv f =  va_arg(args, pv);
// 			cout << "\"" << field << "\": ";
// 			cout << "[" << endl;
// 			for (int i = 0; i < (int)f->size(); ++i)
// 			{
// 				(*f)[i]->print(0);
// 				if (i < (int)f->size() - 1)
// 					cout << "," << endl;
// 				else
// 					cout << endl;
// 			}
// 			cout << endl;
// 			cout << "]" << endl;
// 		}
// 		++fmt;
// 		if (*fmt != '\0')
// 			cout << "," << endl;
// 	}
// 	cout << "}" << endl;
// 	if ((astname != NULL) && (astname[0] != '\0'))
// 		cout << "}" << endl;
// 	va_end(args);
// }


