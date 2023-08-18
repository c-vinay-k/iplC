#ifndef TYPE_HH
#define TYPE_HH

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Type {
    public:
        string name;
        vector<int> fields;
        int size;
        int npointers;
        int derefpointers;
        Type();
        Type(string name, vector<int> fields, int size, int npointers);
        Type(string name);
        Type(string name, int size);
        Type(Type* type);
        Type(string name, vector<int> fields, int size, int npointers, int derefpointers);

        void addField(int field);
        void addFields(vector<int> fields);

        void addPointer();
        void addDerefPointer();

        void print();
        void printName();
};

Type* insertField(Type* type, int field);
Type* insertPointer(Type* type);
Type* insertDerefPointer(Type* type);
// Type* removePointer(Type* type);
Type* convertToDeref(Type* type);

Type* createtype(string type);

class typeSpecifierClass{
    public:
        string name;
        int size;
        typeSpecifierClass();
        typeSpecifierClass(string name, int size);
        typeSpecifierClass(string name);
};

class declaratorArrayClass{
    public:
        string name;
        Type* type;

        declaratorArrayClass();
        declaratorArrayClass(string name, Type* type);

};

class declaratorClass{
    public:
        declaratorArrayClass* declaratorArray;

        declaratorClass();
        declaratorClass(declaratorArrayClass* declaratorArray);
};

class declaratorListClass{
    public:
        vector<declaratorClass*> declaratorList;

        declaratorListClass();
        declaratorListClass(vector<declaratorClass*> declaratorList);

        void addDeclarator(declaratorClass* declarator);
};

class parameterDeclarationClass{
    public:
        typeSpecifierClass* typeSpecifier;
        declaratorClass* declarator;

        parameterDeclarationClass();
        parameterDeclarationClass(typeSpecifierClass* typeSpecifier, declaratorClass* declarator);
};

class parameterListClass{
    public:
        vector<parameterDeclarationClass*> parameterDeclarationList;

        parameterListClass();
        parameterListClass(vector<parameterDeclarationClass*> parameterDeclarationList);

        void addParameter(parameterDeclarationClass* parameterDeclaration);
};

class funDeclaratorClass{
    public:
        string name;
        parameterListClass* parameterList;

        funDeclaratorClass();
        funDeclaratorClass(string name, parameterListClass* parameterList);
        funDeclaratorClass(string name);
};

class declarationClass{
    public:
        typeSpecifierClass* typeSpecifier;
        declaratorListClass* declaratorList;

        declarationClass();
        declarationClass(typeSpecifierClass* typeSpecifier, declaratorListClass* declaratorList);
};

class declarationListClass{
    public:
        vector<declarationClass*> declarationList;

        declarationListClass();
        declarationListClass(vector<declarationClass*> declarationList);

        void addDeclaration(declarationClass* declaration);
};


//////// Type Checking ////////

// class typeNode{
//     public:
//         string name;
//         int size;
//         typeNode* child;
//         virtual void print() = 0;
// };

// class baseTypeNode: public typeNode{
//     public:
//         string name;
//         int size;
//         typeNode* child;
//         baseTypeNode(){
//             child = NULL;
//         }
//         baseTypeNode(string name){
//             this->name = name;
//             if (name == "int"){
//                 size = 4;
//             }
//             else if (name == "float"){
//                 size = 4;
//             }
//             else if (name == "void"){
//                 size = 4;
//             }
//             this->child = NULL;
//         }
//         void print(){
//             cout << "baseType -> " << name << endl;
//         }
// };

// class pointerNode: public typeNode{
//     public:
//         typeNode* child;
//         pointerNode(){}
//         pointerNode(typeNode* child){
//             this->child = child;
//         }
//         void print(){
//             cout << "pointer " << endl;
//             child->print();
//         }
// };

// class arrayNode: public typeNode{
//     public:
//         typeNode* child;
//         int value;
//         arrayNode(){}
//         arrayNode(int size){
//             value = size;
//         }
//         arrayNode(typeNode* child, int size){
//             this->child = child;
//             value = size;
//         }
//         void print(){
//             cout << "array " << endl;
//             cout << "value -> " << value << endl;
//             child->print();
//         }
// };

// class typeNode{
//     public:
//         string name;
//         int size;
//         vector<int> type;
//         typeNode(){}
//         typeNode(Type* type);
// };

vector<int> convertType(Type* type);
void printType(Type* type);

bool canSubscript(Type* type);
bool checkStruct(Type* type);
bool checkPointerToStruct(Type* type);

bool checkPointer(Type* type);
bool checklvalue(Type* type); // this is not exactly lvalue checking, instead int or float or pointer checking

bool checklvalueForNOT(Type* type);

bool canDeref(Type* type);

bool checkPointerToAdd(Type* type);

bool compatible(Type* type1, Type* type2);
int compatibleForParameterPassing(Type* type1, Type* type2);
bool compForParamPassing(int num);
bool compatibleForParameters(Type* type1, Type* type2);
bool compatibleForAssignment(Type* type1, Type* type2);

//Type* evaluateTypeOfSubscriptable(Type* type);
// Type* evaluateTypeOfAdd(Type* type);

bool checkVoid(Type* type);

// bool checkNameForStruct(string name1, string name2);

#endif
