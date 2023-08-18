#include "symbtab.hh"
#include <iostream>
using namespace std;

SymbolTableEntry::SymbolTableEntry() {
    name = "";
    varfuntype = "";
    scope = "";
    size = 0;
    offset = 0;
    returnType = "";
    child = NULL;
}

SymbolTableEntry::SymbolTableEntry(string name, string varfuntype, string scope, int size, int offset, string returnType, Type* retType, SymbolTable* child) {
    this->name = name;
    this->varfuntype = varfuntype;
    this->scope = scope;
    this->size = size;
    this->offset = offset;
    this->returnType = returnType;
    this->retType = retType;
    this->child = child;
}

SymbolTableEntry::SymbolTableEntry(string name, string varfuntype, string scope, int size, int offset, string returnType, Type* retType) {
    this->name = name;
    this->varfuntype = varfuntype;
    this->scope = scope;
    this->size = size;
    this->offset = offset;
    this->returnType = returnType;
    this->retType = retType;
    this->child = NULL;
}

SymbolTable::SymbolTable() {
    entries = map<string, SymbolTableEntry*>();
}

SymbolTable::SymbolTable(map<string, SymbolTableEntry*> entries) {
    this->entries = entries;
}

void SymbolTable::addEntry(SymbolTableEntry* entry) {
    entries[entry->name] = entry;
}

SymbolTableEntry* SymbolTable::getEntry(string name) {
    return entries[name];
}

void SymbolTable::printgst() {
    cout << "[";
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        cout << "[";
        cout << "\t\"" << it->second->name << "\",";
        cout << "\t\"" << it->second->varfuntype << "\",";
        cout << "\t\"" << it->second->scope << "\",\t";
        cout << it->second->size << ",\t";
        if(it->second->varfuntype == "struct")
            cout << "\"-\"" << ",";
        else
            cout << it->second->offset << ",";
        cout << "\"" << it->second->returnType << "\"";
        cout << "\n]";
        if (next(it,1) != entries.end()) 
        cout << "," << endl;
        else
        cout << endl;
    }
    cout << "]" << endl;
}

void SymbolTable::print(){
    cout << "[\n";
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        cout << "[";
        cout << "\"" << it->second->name << "\",";
        cout << "\t\"" << it->second->varfuntype << "\",";
        cout << "\t\"" << it->second->scope << "\",\t";
        cout << it->second->size << ",\t";
        cout << it->second->offset << ",";
        cout << "\"" << it->second->returnType << "\"";
        // cout << "\""; it->second->retType->printName(); cout<< "\"";
        cout << "\n]";
        if (next(it,1) != entries.end()) 
        cout << "\n," << endl;
        else
        cout << endl;
    }
    cout << "]" << endl;
}

vector<Type*> sortedParam(SymbolTable* st){
    vector<Type*> v;
    map<int, SymbolTableEntry*> m;
    for (auto it = st->entries.begin(); it != st->entries.end(); ++it)
    {
        if(it->second->scope == "param")
            m[it->second->offset] = it->second;
    }
    for (auto it = m.rbegin(); it != m.rend(); ++it)
    {
        v.push_back(it->second->retType);
    }
    return v;
}

bool isDefined(SymbolTable st, string name){
    if(st.entries.find(name) != st.entries.end())
        return true;
    return false;
}

bool isDefinedPointer(SymbolTable* st, string name){
    if(st->entries.find(name) != st->entries.end())
        return true;
    return false;
}

int getOffset(SymbolTable* st, string name){
    return st->entries[name]->offset;
}

Type* getReturnType(SymbolTable* st, string name){
    return st->entries[name]->retType;
}

Type* evaluateTypeOfSubscriptable(Type* type, SymbolTable gst){
    Type* ty = new Type();
    string name;
    int npointers;
    int derefpointers;
    vector<int> fields;
    int size;
    if(type->derefpointers>0){
        derefpointers = type->derefpointers-1;
        int i = 0;
        while(i<type->name.size()){
            if(type->name[i]=='('){
                name = type->name.substr(0,i) + type->name.substr(i+3);
                break;
            }
            i++;
        }
        npointers = type->npointers;
        fields = type->fields;
        // size = type->size;
        size = 1;
        for(int i=0;i<fields.size();i++){
            size*=fields[i];
        }
        if(npointers>0){
            size*=4;
        }else if(type->name.substr(0,6)!="struct"){
            size*=4;
        }else if(type->name.substr(0,6)=="struct"){
            for(auto hj: gst.entries){
                if(hj.second->name == name.substr(0, hj.second->name.size())){
                    size*=hj.second->size;
                    break;
                }
            }
        }
    }else if(type->fields.size()>0){
        derefpointers = type->derefpointers;
        int temp = type->fields[0];
        for(int i=0;i<type->name.size();i++){
            if(type->name[i]=='['){
                int j = i+1;
                while(type->name[j]!=']'){
                    j++;
                }
                name = type->name.substr(0,i) + type->name.substr(j+1);
                break;
            }
        }
        npointers = type->npointers;
        fields = type->fields;
        // size = type->size;
        size = type->size/temp;
        fields.erase(fields.begin());
    }else if(type->npointers>0){
        derefpointers = type->derefpointers;
        for(int i=0;i<type->name.size();i++){
            if(type->name[i]=='*'){
                name = type->name.substr(0,i) + type->name.substr(i+1);
                break;
            }
        }
        npointers = type->npointers-1;
        fields = type->fields;
        if(npointers>0) size = type->size;
        else{
            size = 1;
            for(int i=0;i<fields.size();i++){
                size*=fields[i];
            }
            if(type->name.substr(0,6)!="struct"){
                size*=4;
            }else if(type->name.substr(0,6)=="struct"){
                for(auto hj: gst.entries){
                    if(hj.second->name == name.substr(0, hj.second->name.size())){
                        size*=hj.second->size;
                        break;
                    }
                }
            }
        }
    }
    ty->name = name;
    ty->npointers = npointers;
    ty->derefpointers = derefpointers;
    ty->fields = fields;
    ty->size = size;
    // cout<<"Subscriptable: "<<ty->name<<endl;
    return ty;
}

Type* evaluateTypeOfAdd(Type* type, SymbolTable gst){
    Type* ty = new Type();
    string name;
    int npointers;
    int derefpointers;
    vector<int> fields;
    int size;
    if(type->derefpointers==0 && type->fields.size()>0){
        derefpointers = type->derefpointers+1;
        for(int i=0;i<type->name.size();i++){
            if(type->name[i]=='['){
                int j = i+1;
                while(type->name[j]!=']'){
                    j++;
                }
                name = type->name.substr(0,i) + "(*)"+type->name.substr(j+1);
                break;
            }
        }
        npointers = type->npointers;
        // size = type->size/fields[0];
        size = 4;
        for(int i=1;i<type->fields.size();i++){
            fields.push_back(type->fields[i]);
        }
    }else{
        derefpointers = type->derefpointers;
        name = type->name;
        npointers = type->npointers;
        fields = type->fields;
        size = type->size;
    }
    ty->name = name;
    ty->npointers = npointers;
    ty->derefpointers = derefpointers;
    ty->fields = fields;
    ty->size = size;
    return ty;
}

int sizeOfAdd(Type* type, SymbolTable gst){
    if(type->derefpointers>0){
        int size = 1;
        for(int i=0;i<type->fields.size();i++){
            size*=type->fields[i];
        }
        if(type->npointers>0){
            size*=4;
        }else if(type->name.substr(0,6)!="struct"){
            size*=4;
        }else if(type->name.substr(0,6)=="struct"){
            for(auto hj: gst.entries){
                if(hj.second->name == type->name.substr(0, hj.second->name.size())){
                    size*=hj.second->size;
                    break;
                }
            }
        }
        return size;
    }else if(type->fields.size()>0){
        return type->size/type->fields[0];
    }else if(type->npointers>0){
        if(type->npointers>1) return type->size;
        else{
            int size = 1;
            for(int i=0;i<type->fields.size();i++){
                size*=type->fields[i];
            }
            if(type->name.substr(0,6)!="struct"){
                size*=4;
            }else if(type->name.substr(0,6)=="struct"){
                for(auto hj: gst.entries){
                    if(hj.second->name == type->name.substr(0, hj.second->name.size())){
                        size*=hj.second->size;
                        break;
                    }
                }
            }
            return size;
        }
    }
    return 1;
}

Type* removePointer(Type* type, SymbolTable gst){
    Type* ty = new Type();
    string name;
    int npointers;
    int derefpointers;
    vector<int> fields;
    int size;
    if(type->derefpointers>0){
        derefpointers = type->derefpointers-1;
        for(int i=0;i<type->name.size();i++){
            if(type->name[i]=='('){
                name = type->name.substr(0,i) + type->name.substr(i+3);
                break;
            }
        }
        npointers = type->npointers;
        fields = type->fields;
        size = 1;
        for(int i=0;i<fields.size();i++){
            size*=fields[i];
        }
        if(npointers>1) size*=4;
        else{
            if(type->name.substr(0,6)!="struct"){
                size*=4;
            }else if(type->name.substr(0,6)=="struct"){
                for(auto hj: gst.entries){
                    if(hj.second->name == name.substr(0, hj.second->name.size())){
                        size*=hj.second->size;
                        break;
                    }
                }
            }
        }
    }else if(type->fields.size()>0){
        // remove the first brackets [ ]
        for(int i=0;i<type->name.size();i++){
            if(type->name[i]=='['){
                int j = i+1;
                while(type->name[j]!=']'){
                    j++;
                }
                name = type->name.substr(0,i) + type->name.substr(j+1);
                break;
            }
        }
        npointers = type->npointers;
        derefpointers = type->derefpointers;
        size = type->size/type->fields[0];
        for(int i=1;i<type->fields.size();i++){
            fields.push_back(type->fields[i]);
        }
    }else{
        npointers = type->npointers-1;
        derefpointers = type->derefpointers;
        for(int i=0;i<type->name.size();i++){
            if(type->name[i]=='*'){
                name = type->name.substr(0,i) + type->name.substr(i+1);
                break;
            }
        }
        fields = type->fields;
        if(npointers>1) size = type->size;
        else{
            size = 1;
            for(int i=0;i<fields.size();i++){
                size*=fields[i];
            }
            if(type->name.substr(0,6)!="struct"){
                size*=4;
            }else if(type->name.substr(0,6)=="struct"){
                for(auto hj: gst.entries){
                    if(hj.second->name == name.substr(0, hj.second->name.size())){
                        size*=hj.second->size;
                        break;
                    }
                }
            }
        }
    }
    ty->name = name;
    ty->npointers = npointers;
    ty->derefpointers = derefpointers;
    ty->fields = fields;
    ty->size = size;
    return ty;
}
