#ifndef SYMBTAB_HH
#define SYMBTAB_HH
#include "type.hh"
#include <map>
using namespace std;

class SymbolTable;

class SymbolTableEntry{
    public:
        string name;
        string varfuntype;
        string scope;
        int size;
        int offset;
        string returnType;
        Type* retType;
        SymbolTable* child;

        SymbolTableEntry();
        SymbolTableEntry(string name, string varfuntype, string scope, int size, int offset, string returnType, Type* retType, SymbolTable* child);
        SymbolTableEntry(string name, string varfuntype, string scope, int size, int offset, string returnType, Type* retType);

};

class SymbolTable{
    public:
        map<string, SymbolTableEntry*> entries;

        SymbolTable();
        SymbolTable(map<string, SymbolTableEntry*> entries);
        void addEntry(SymbolTableEntry* entry);
        SymbolTableEntry* getEntry(string name);

        void printgst();
        void print();
};

vector<Type*> sortedParam(SymbolTable* st);
bool isDefined(SymbolTable st, string name);
bool isDefinedPointer(SymbolTable* st, string name);

#endif
