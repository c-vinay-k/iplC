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
