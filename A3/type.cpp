#include "type.hh"
#include <iostream>
using namespace std;

Type::Type() {
    name = "";
    fields = vector<int>();
    size = 1;
    npointers = 0;
    derefpointers = 0;
}

Type::Type(string name, vector<int> fields, int size, int npointers) {
    this->name = name;
    this->fields = fields;
    this->size = size;
    this->npointers = npointers;
    this->derefpointers = 0;
}

Type::Type(string name, vector<int> fields, int size, int npointers, int derefpointers) {
    this->name = name;
    this->fields = fields;
    this->size = size;
    this->npointers = npointers;
    this->derefpointers = derefpointers;
}


Type::Type(string name) {
    this->name = name;
    fields = vector<int>();
    if(name == "int"){
        size = 4;
        npointers = 0;
        derefpointers = 0;
    }else if(name == "float"){
        size = 4;
        npointers = 0;
        derefpointers = 0;
    }else if(name == "void"){
        size = 4;
        npointers = 0;
        derefpointers = 0;
    }
}

Type::Type(string name, int size) {
    this->name = name;
    fields = vector<int>();
    this->size = size;
    npointers = 0;
    derefpointers = 0;
}

Type::Type(Type* type) {
    name = type->name;
    fields = type->fields;
    size = type->size;
    npointers = type->npointers;
    derefpointers =type->derefpointers;
}

void Type::addField(int field) {
    fields.push_back(field);
    size *= field;
    name = name + "[" + to_string(field) + "]";
}

void Type::addFields(vector<int> fields) {
    for (int i = 0; i < fields.size(); i++) {
        this->fields.push_back(fields[i]);
        size *= fields[i];
        name = name + "[" + to_string(fields[i]) + "]";
    }
}

void Type::addPointer() {
    npointers++;
    size = 4;
    if(fields.size()>0){
        for(int i=0;i<fields.size();i++){
            size *= fields[i];
        }
    }else{
        size = 4;
    }
    name = name + "*";
}

void Type::addDerefPointer(){
    derefpointers++;
    if(fields.size()>0 && npointers>0){
        int i = name.size()-1;
        while(name[i]!='*'){
            i--;
        }
        name = name.substr(0,i+1) + "(*)" + name.substr(i+1);
    }else if(fields.size()==0 && npointers>0){
        name = name + "(*)";
    }else if(fields.size()>0 && npointers==0){
        int i = 0;
        for(i=0;i<name.size();i++){
            if(name[i]=='['){
                break;
            }
        }
        name = name.substr(0,i) + "(*)" + name.substr(i);
    }else if(fields.size()==0 && npointers==0){
        name = name + "(*)";
    }
}

void Type::print() {
    cout << "Type: " << name << endl;
    cout << "Fields: ";
    for (int i = 0; i < fields.size(); i++) {
        cout << fields[i] << " ";
    }
    cout << endl;
    cout << "Size: " << size << endl;
    cout << "NPointers: " << npointers << endl;
    cout<< "DerefPointers: "<< derefpointers << endl;
}

void Type::printName() {
    cout << name << endl;
}

Type* insertField(Type* type, int field) {
    Type* newtype = new Type(type->name, type->fields, type->size, type->npointers, type->derefpointers);
    // newtype->print();
    newtype->addField(field);
    // newtype->print();
    return newtype;
}

Type* insertPointer(Type* type) {
    Type* newtype = new Type(type->name, type->fields, type->size, type->npointers, type->derefpointers);
    newtype->addPointer();
    return newtype;
}

Type* insertDerefPointer(Type* type) {
    Type* newtype = new Type(type->name, type->fields, type->size, type->npointers, type->derefpointers);
    newtype->addDerefPointer();
    newtype->size = 4;
    return newtype;
}

// Type* removePointer(Type* type){
//     if(type->derefpointers>0){
//         string name;
//         // loop through type->name and remove the first (*)
//         int i = 0;
//         while(i<type->name.size()){
//             if(type->name[i]=='('){
//                 name = type->name.substr(0,i) + type->name.substr(i+3);
//                 break;
//             }
//             i++;
//         }
//         Type* newtype = new Type(name, type->fields, type->size, type->npointers, type->derefpointers-1);
//         return newtype;
//     }else{
//         Type* newtype = new Type(type->name.substr(0,type->name.size()-1), type->fields, type->size, type->npointers-1, type->derefpointers);
//         return newtype;
//     }
//     return type;
// }

Type* convertToDeref(Type* type){
    // convert if array to deref
    if(type->derefpointers>0){
        return type;
    }else{
        if(type->fields.size()==0){
            return type;
        }else{
            // loop through to find the first [] and add (*) in its place
            string name;
            int i = 0;
            while(i<type->name.size()){
                if(type->name[i]=='['){
                    // find till ']'
                    int j = i+1;
                    while(type->name[j]!=']'){
                        j++;
                    }
                    name = type->name.substr(0,i) + "(*)" + type->name.substr(j+1);
                    break;
                }
                i++;
            }
            // remove first element in fields
            vector<int> fields;
            for(int i=1;i<type->fields.size();i++){
                fields.push_back(type->fields[i]);
            }
            Type* newtype = new Type(name, fields, type->size, type->npointers, type->derefpointers+1);
            return newtype;
        }
    }
}

// string constructName(Type* type){
//     if(type->name[0]=='i')
// }

Type* createtype(string type) {
    Type* t = new Type(type);
    return t;
}


//////////////////////////////////////// TypeSpecifierClass ////////////////////////////////////////

typeSpecifierClass::typeSpecifierClass() {
    name = "";
    size = 1;
}

typeSpecifierClass::typeSpecifierClass(string name, int size) {
    this->name = name;
    this->size = size;
}

typeSpecifierClass::typeSpecifierClass(string name) {
    this->name = name;
    if(name == "int"){
        size = 4;
    }else if(name == "float"){
        size = 4;
    }else if(name == "void"){
        size = 4;
    }
}

//////////////////////////////////////// DeclaratorArrayClass ////////////////////////////////////////

declaratorArrayClass::declaratorArrayClass() {
    name = "";
    type = new Type();
}

declaratorArrayClass::declaratorArrayClass(string name, Type* type) {
    this->name = name;
    this->type = type;
}

//////////////////////////////////////// DeclaratorClass ////////////////////////////////////////

declaratorClass::declaratorClass() {
    declaratorArray = new declaratorArrayClass();
}

declaratorClass::declaratorClass(declaratorArrayClass* declaratorArray) {
    this->declaratorArray = declaratorArray;
}

//////////////////////////////////////// DeclaratorListClass ////////////////////////////////////////

declaratorListClass::declaratorListClass() {
    declaratorList = vector<declaratorClass*>();
}

declaratorListClass::declaratorListClass(vector<declaratorClass*> declarators) {
    this->declaratorList = declarators;
}

void declaratorListClass::addDeclarator(declaratorClass* declarator) {
    declaratorList.push_back(declarator);
}

//////////////////////////////////////// ParameterDeclarationClass ////////////////////////////////////////

parameterDeclarationClass::parameterDeclarationClass() {
    typeSpecifier = new typeSpecifierClass();
    declarator = new declaratorClass();
}

parameterDeclarationClass::parameterDeclarationClass(typeSpecifierClass* typeSpecifier, declaratorClass* declarator) {
    this->typeSpecifier = typeSpecifier;
    this->declarator = declarator;
}

//////////////////////////////////////// ParameterListClass ////////////////////////////////////////

parameterListClass::parameterListClass() {
    parameterDeclarationList = vector<parameterDeclarationClass*>();
}

parameterListClass::parameterListClass(vector<parameterDeclarationClass*> parameters) {
    this->parameterDeclarationList = parameters;
}

void parameterListClass::addParameter(parameterDeclarationClass* parameter) {
    parameterDeclarationList.push_back(parameter);
}

//////////////////////////////////////// FunDeclaratorClass ////////////////////////////////////////

funDeclaratorClass::funDeclaratorClass() {
    name = "";
    parameterList = new parameterListClass();
}

funDeclaratorClass::funDeclaratorClass(string name, parameterListClass* parameterList) {
    this->name = name;
    this->parameterList = parameterList;
}

funDeclaratorClass::funDeclaratorClass(string name) {
    this->name = name;
    parameterList = new parameterListClass();
}

//////////////////////////////////////// DeclarationClass ////////////////////////////////////////

declarationClass::declarationClass() {
    typeSpecifier = new typeSpecifierClass();
    declaratorList = new declaratorListClass();
}

declarationClass::declarationClass(typeSpecifierClass* typeSpecifier, declaratorListClass* declaratorList) {
    this->typeSpecifier = typeSpecifier;
    this->declaratorList = declaratorList;
}

//////////////////////////////////////// DeclarationListClass ////////////////////////////////////////

declarationListClass::declarationListClass() {
    declarationList = vector<declarationClass*>();
}

declarationListClass::declarationListClass(vector<declarationClass*> declarations) {
    this->declarationList = declarations;
}

void declarationListClass::addDeclaration(declarationClass* declaration) {
    declarationList.push_back(declaration);
}

//////////////////////////////////////// HelperFunctions ////////////////////////////////////////

// typeNode* convertType(Type* type) {
//     typeNode* curr = new baseTypeNode();
//     if(type->name[0]=='i'){
//         curr = new baseTypeNode("int");
//         for(int i=0;i<type->npointers;i++){
//             pointerNode* p = new pointerNode();
//             p->child = curr;
//             curr = p;
//         }
//         for(int i=type->fields.size()-1;i>=0;i--){
//             arrayNode* a = new arrayNode(curr, type->fields[i]);
//             curr = a;
//         }
//     }else if(type->name[0]=='f'){
//         curr = new baseTypeNode("float");
//         for(int i=0;i<type->npointers;i++){
//             pointerNode* p = new pointerNode();
//             p->child = curr;
//             curr = p;
//         }
//         for(int i=type->fields.size()-1;i>=0;i--){
//             arrayNode* a = new arrayNode(curr, type->fields[i]);
//             curr = a;
//         }
//     }else if(type->name[0]=='v'){
//         curr = new baseTypeNode("void");
//         for(int i=0;i<type->npointers;i++){
//             pointerNode* p = new pointerNode();
//             p->child = curr;
//             curr = p;
//         }
//         for(int i=type->fields.size()-1;i>=0;i--){
//             arrayNode* a = new arrayNode(curr, type->fields[i]);
//             curr = a;
//         }
//     }
//     return curr;
// }

// void printNode(typeNode* node) {
//     typeNode* curr = node;
//     curr->print();
// }

// bool compatible(Type* t1, Type* t2) {
//     typeNode* n1 = convertType(t1);
//     typeNode* n2 = convertType(t2);
    
//     return true;
// }

// bool canSubscript(Type* type) {
//     typeNode* n = convertType(type);
//     if(n->child==NULL){
//         return false;
//     }else{
//         while(n->child!=NULL){
//             n = n->child;
//         }
//         if(n->name == "void"){
//             return false;
//         }else if(n->name == "int"){
//             return true;
//         }else if(n->name == "float"){
//             return true;
//         }else{
//             return false;
//         }
//     }
//     return false;
// }

vector<int> convertType(Type* type){
    vector<int> v;
    for(int i=0;i<type->derefpointers;i++){
        v.push_back(-1);
    }
    for(int i=0;i<type->fields.size();i++){
        v.push_back(type->fields[i]);
    }
    for(int i=0;i<type->npointers;i++){
        v.push_back(-1);
    }
    if(type->name[0]=='i'){
        v.push_back(-2);
    }else if(type->name[0]=='f'){
        v.push_back(-3);
    }else if(type->name[0]=='v'){
        v.push_back(-4);
    }else if(type->name[0]=='s'){
        v.push_back(-5);
    }
    return v;
}

void printType(Type* type){
    vector<int> v = convertType(type);
    for(int i=0;i<v.size();i++){
        cout<<v[i]<<endl;
    }
}

bool canSubscript(Type* type){
    vector<int> v = convertType(type);
    int n = v.size();
    if(n==0 || n==1) return false;
    return v[n-1]!=-4;
}

bool checkStruct(Type* type) {
    if(type->name.substr(0,7) == "struct " && type->npointers == 0 && type->derefpointers == 0 && type->fields.size() == 0){
        return true;
    }
    return false;
}

bool checkPointerToStruct(Type* type) {
    if(type->name.substr(0,7) == "struct " && type->npointers+type->derefpointers+type->fields.size() == 1){
        return true;
    }
    return false;
}

bool checkPointer(Type* type) {
    if(type->derefpointers>0 || type->fields.size()==0 && type->npointers>0){
        return true;
    }
    return false;
}

bool checklvalue(Type* type) {
    if(type->name == "int" || type->name == "float" || checkPointer(type)){
        return true;
    }
    return false;
}

bool checklvalueForNOT(Type* type){
    if(type->name == "int" || type->name == "float" || (type->derefpointers>0 || type->fields.size()>0 || type->npointers>0)){
        return true;
    }
    return false;
}

bool canDeref(Type* type) {
    if(type->derefpointers+type->fields.size()+type->npointers>0 && type->name[0]!='v'){
        return true;
    }
    return false;
}

bool checkPointerToAdd(Type* type) {
    if(type->derefpointers>0) return true;
    if(type->fields.size()>0) return true;
    if(type->npointers>0) return true;
    return false;
}

bool compatible(Type* type1, Type* type2){
    vector<int> v1 = convertType(type1);
    vector<int> v2 = convertType(type2);
    if(v1==v2)  return true;
    if(v1.size()!=v2.size()){
        return false;
    }
    else if(v1[0]>0 && v2[0]<0){
        vector<int> v;
        if(v1[0]>0){
            v.push_back(-1);
            for(int i=1;i<v1.size();i++){
                v.push_back(v1[i]);
            }
        }
        if(v==v2) return true;
    }else if(v1[0]<0 && v2[0]>0){
        vector<int> v;
        if(v2[0]>0){
            v.push_back(-1);
            for(int i=1;i<v2.size();i++){
                v.push_back(v2[i]);
            }
        }
        if(v==v1) return true;
    }
    return false;
}

bool compatibleForParameters(Type* type1, Type* type2){ // int[20] can be passed if expected argument is int[10]
    vector<int> v1 = convertType(type1);
    vector<int> v2 = convertType(type2);
    if(v1.size()!=v2.size()){
        return false;
    }
    if(v1[0]>0) v1[0]=-1;
    if(v2[0]>0) v2[0]=-1;
    if(v1==v2)  return true;
    return false;
}

int compatibleForParameterPassing(Type* type1, Type* type2){
    if(type1->name == "int" && type2->name == "int"){
        return 1;
    }else if(type1->name == "float" && type2->name == "float"){
        return 2;
    }else if(type1->name == "int" && type2->name == "float"){
        return 3;
    }else if(type1->name == "float" && type2->name == "int"){
        return 4;
    }else if(compatibleForParameters(type1, type2)){
        return 5;
    }else if(type1->name == type2->name){
        return 6;
    }else if(type1->name == "void*" && type2->derefpointers+type2->fields.size()+type2->npointers>0){
        return 7;
    }
    return 0;
}

bool compForParamPassing(int num){
    if(num==1 || num==2 || num==5 || num==6 || num==7) return true;
    return false;
}

bool compatibleForAssignment(Type* type1, Type* type2){
    vector<int> v1 = convertType(type1);
    vector<int> v2 = convertType(type2);
    // printType(type1);
    // printType(type2);
    if(v1.size()!=v2.size()){
        return false;
    }
    if(v1==v2 && v1[0]<0)  return true;
    else if(v1[0]>0 && v2[0]<0){
        // vector<int> v;
        // if(v1[0]>0){
        //     v.push_back(-1);
        //     for(int i=1;i<v1.size();i++){
        //         v.push_back(v1[i]);
        //     }
        // }
        // if(v==v2) return true;
        return false;
    }else if(v1[0]<0 && v2[0]>0){
        vector<int> v;
        if(v2[0]>0){
            v.push_back(-1);
            for(int i=1;i<v2.size();i++){
                v.push_back(v2[i]);
            }
        }
        if(v==v1) return true;
    }
    return false;
}

// Type* evaluateTypeOfSubscriptable(Type* type){
//     Type* ty = new Type();
//     string name;
//     int npointers;
//     int derefpointers;
//     vector<int> fields;
//     int size;
//     if(type->derefpointers>0){
//         derefpointers = type->derefpointers-1;
//         int i = 0;
//         while(i<type->name.size()){
//             if(type->name[i]=='('){
//                 name = type->name.substr(0,i) + type->name.substr(i+3);
//                 break;
//             }
//             i++;
//         }
//         npointers = type->npointers;
//         fields = type->fields;
//         size = type->size;   
//     }else if(type->fields.size()>0){
//         derefpointers = type->derefpointers;
//         for(int i=0;i<type->name.size();i++){
//             if(type->name[i]=='['){
//                 int j = i+1;
//                 while(type->name[j]!=']'){
//                     j++;
//                 }
//                 name = type->name.substr(0,i) + type->name.substr(j+1);
//                 break;
//             }
//         }
//         npointers = type->npointers;
//         fields = type->fields;
//         size = type->size;
//         fields.erase(fields.begin());
//     }else if(type->npointers>0){
//         derefpointers = type->derefpointers;
//         for(int i=0;i<type->name.size();i++){
//             if(type->name[i]=='*'){
//                 name = type->name.substr(0,i) + type->name.substr(i+1);
//                 break;
//             }
//         }
//         npointers = type->npointers-1;
//         fields = type->fields;
//         size = type->size;
//     }
//     ty->name = name;
//     ty->npointers = npointers;
//     ty->derefpointers = derefpointers;
//     ty->fields = fields;
//     ty->size = size;
//     // cout<<"Subscriptable: "<<ty->name<<endl;
//     return ty;
// }

// Type* evaluateTypeOfAdd(Type* type){
//     Type* ty = new Type();
//     string name;
//     int npointers;
//     int derefpointers;
//     vector<int> fields;
//     int size;
//     if(type->derefpointers==0 && type->fields.size()>0){
//         derefpointers = type->derefpointers+1;
//         for(int i=0;i<type->name.size();i++){
//             if(type->name[i]=='['){
//                 int j = i+1;
//                 while(type->name[j]!=']'){
//                     j++;
//                 }
//                 name = type->name.substr(0,i) + "(*)"+type->name.substr(j+1);
//                 break;
//             }
//         }
//         npointers = type->npointers;
//         for(int i=1;i<type->fields.size();i++){
//             fields.push_back(type->fields[i]);
//         }
//         size = type->size;
//     }else{
//         derefpointers = type->derefpointers;
//         name = type->name;
//         npointers = type->npointers;
//         fields = type->fields;
//         size = type->size;
//     }
//     ty->name = name;
//     ty->npointers = npointers;
//     ty->derefpointers = derefpointers;
//     ty->fields = fields;
//     ty->size = size;
//     return ty;
// }

bool checkVoid(Type* type){
    if(type->name == "void" && type->npointers==0){
        return true;
    }
    return false;
}

// bool checkNameForStruct(string name1, string name2){
//     // loop through name2 and check if name1 is present till we find '[
//     int i = 0;
//     while(i<name2.size()){
//         if(name2[i]=='['){
//             break;
//         }
//         i++;
//     }
//     if(name1.substr(0,i) == name2.substr(0,i)){
//         return true;
//     }
//     return false;
// }

