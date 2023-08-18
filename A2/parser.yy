%skeleton "lalr1.cc"
%require  "3.0.1"

%defines 
%define api.namespace {IPL}
%define api.parser.class {Parser}

%define parse.trace

%code requires{
  #include "ast.hh"
  #include "symbtab.hh"
  #include "type.hh"
  #include "location.hh"
   namespace IPL {
      class Scanner;
   }

}
/* %printer {std::cerr<<$$;} INT
%printer {std::cerr<<$$;} FLOAT
%printer {std::cerr<<$$;} VOID
%printer {std::cerr<<$$;} STRUCT
%printer {std::cerr<<$$;} IF
%printer {std::cerr<<$$;} ELSE
%printer {std::cerr<<$$;} WHILE
%printer {std::cerr<<$$;} FOR
%printer {std::cerr<<$$;} RETURN
%printer {std::cerr<<$$;} OR_OP
%printer {std::cerr<<$$;} AND_OP
%printer {std::cerr<<$$;} EQ_OP
%printer {std::cerr<<$$;} NE_OP
%printer {std::cerr<<$$;} LE_OP
%printer {std::cerr<<$$;} GE_OP
%printer {std::cerr<<$$;} INC_OP */
%printer {std::cerr<<$$;} IDENTIFIER
%printer {std::cerr<<$$;} INT_CONST
%printer {std::cerr<<$$;} FLOAT_CONST
%printer {std::cerr<<$$;} STRING_LITERAL
/* %printer {std::cerr<<$$;} PTR_OP */

%parse-param {Scanner &scanner}
%locations
%code{
   #include<iostream>
   #include<cstdlib>
   #include<fstream>
   #include<string>


   #include"scanner.hh"
  //  int nodeCount = 0;

#undef yylex
#define yylex IPL::Parser::scanner.yylex

extern SymbolTable gst;
int structSize = 0;
bool isStruct = false;
string type_ ="";
int typeSize = 0;
int offset = 0;
string structName = "";
bool isCurrentStruct = false;

bool isFunction = false;
bool isFunReturn = false;
string functionName = "";
Type* functionReturnType;
SymbolTable* st;
int paramoffset = 12;
int localoffset = 0;

Type* type__;
bool parenthesis = false;

map<string, abstract_astnode*> ast;

int parameterLength = 0;
vector<Type*> parameters;

extern map<std::string, Type*> predefined;
bool foundinPredefined = false;

bool doneinLoop = false;
}

%define api.value.type variant
%define parse.assert

%token <string> IDENTIFIER STRING_LITERAL INT FLOAT VOID STRUCT IF ELSE WHILE FOR RETURN OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP INC_OP PTR_OP INT_CONST FLOAT_CONST OTHERS
%token '+' ',' '(' ')' ':' '{' '}' '[' ']' '-' '=' '!' '|' '&' ';' '*' '/' '>' '<'
/* %nterm  translation_unit struct_specifier function_definition type_specifier fun_declarator parameter_list parameter_declaration declarator_arr declarator compound_statement statement_list statement assignment_expression assignment_statement procedure_call expression logical_and_expression equality_expression relational_expression additive_expression unary_expression multiplicative_expression postfix_expression primary_expression expression_list unary_operator selection_statement iteration_statement declaration_list declarator_list
%type <int> translation_unit struct_specifier function_definition type_specifier fun_declarator parameter_list parameter_declaration declarator_arr declarator compound_statement statement_list statement assignment_expression assignment_statement procedure_call expression logical_and_expression equality_expression relational_expression additive_expression unary_expression multiplicative_expression postfix_expression primary_expression expression_list unary_operator selection_statement iteration_statement declaration_list declarator_list declaration */
%start translation_unit
/* add left associativity rules for - and + and also precedence rules */
%left '+' '-'
%left '*' '/'

%nterm <abstract_astnode*> translation_unit struct_specifier function_definition
%nterm <typeSpecifierClass*> type_specifier
%nterm <funDeclaratorClass*> fun_declarator
%nterm <parameterListClass*> parameter_list
%nterm <parameterDeclarationClass*> parameter_declaration
%nterm <declaratorArrayClass*> declarator_arr
%nterm <declaratorClass*> declarator

%nterm <seq_astnode*> compound_statement statement_list
%nterm <statement_astnode*> statement
%nterm <assignE_astnode*> assignment_expression
%nterm <assignS_astnode*> assignment_statement
%nterm <proccall_astnode*> procedure_call
%nterm <exp_astnode*> expression logical_and_expression equality_expression relational_expression additive_expression unary_expression multiplicative_expression postfix_expression primary_expression
%nterm <proccall_astnode*> expression_list
%nterm <string> unary_operator
%nterm <statement_astnode*> selection_statement
%nterm <statement_astnode*> iteration_statement
%nterm <declarationListClass*> declaration_list
%nterm <declaratorListClass*> declarator_list
%nterm <declarationClass*> declaration

%%
translation_unit:
                struct_specifier
                {
                }
                | function_definition
                {
                }      
                | translation_unit struct_specifier
                {
                }
                | translation_unit function_definition
                {
                }
                ;

struct_specifier:
                STRUCT IDENTIFIER
                {
                  isStruct = true;
                  st = new SymbolTable();
                  offset = 0;
                  structName = "struct "+$2;
                }
                '{' declaration_list '}' ';'
                {
                  gst.addEntry(new SymbolTableEntry("struct "+$2, "struct", "global", structSize, 0, "-", new Type("struct "+$2, structSize), st));
                  isStruct = false;
                  structSize = 0;
                  offset = 0;
                  structName = "";
                }
                ;

function_definition:
                   type_specifier
                   fun_declarator
                   {
                    functionReturnType = new Type($1->name, $1->size);
                   }
                   compound_statement
                   {
                      gst.addEntry(new SymbolTableEntry($2->name, "fun", "global", 0, 0, $1->name, new Type($1->name, $1->size), st));
                      isFunction = false;
                      localoffset = 0;
                      paramoffset = 0;
                      functionName = "";
                      ast[$2->name] = $4;
                   }
                   ;

type_specifier:
              VOID
              {
                type_ = "void";
                typeSize = 4;
                type__ = new Type("void");
                // if(isFunReturn)
                //   functionReturnType = type__;
                $$ = new typeSpecifierClass(type_);
              }
              | INT
              {
                type_ = "int";
                typeSize = 4;
                type__ = new Type("int");
                // if(isFunReturn)
                //   functionReturnType = type__;
                $$ = new typeSpecifierClass(type_);
              }
              | FLOAT
              {
                type_ = "float";
                typeSize = 4;
                type__ = new Type("float");
                // if(isFunReturn)
                //   functionReturnType = type__;
                $$ = new typeSpecifierClass(type_);
              }
              | STRUCT IDENTIFIER
              {
                type_ = "struct "+$2;
                // cout << "type_ = " << type_ << endl;
                // cout << "structName = " << structName << endl;
                if(type_ == structName){
                  typeSize = 4;
                  isCurrentStruct = true;
                }
                else{
                  typeSize = gst.getEntry(type_)->size;
                }
                // else
                //   typeSize = gst.getEntry(type_)->size;  // what happens if it is not present in gst?
                // typeSize = gst.getEntry(type_)->size;  // what happens if it is not present in gst?
                type__ = new Type(type_,typeSize);
                // if(isFunReturn)
                //   functionReturnType = type__;
                $$ = new typeSpecifierClass(type_, typeSize);
              }
              ;

fun_declarator:
              IDENTIFIER '(' 
              {
                for(auto i: gst.entries){
                  if(i.first == $1 && i.second->varfuntype == "fun"){
                    error(@1, "\""+$1+"\" has a previous declaration");
                  }
                }

                st = new SymbolTable();
                paramoffset = 12;
                localoffset = 0;
                functionName = $1;
                isFunction = true;}
              parameter_list ')'
              {
                $$ = new funDeclaratorClass($1, $4);
                if(isFunction)
                {
                  for(int i=$4->parameterDeclarationList.size()-1; i>=0; i--)
                  {
                    for(auto j: st->entries){
                      if(j.first == $4->parameterDeclarationList[i]->declarator->declaratorArray->name){
                        error(@1, "\""+$4->parameterDeclarationList[i]->declarator->declaratorArray->name+"\" has a previous declaration");
                      }
                    }
                    if($4->parameterDeclarationList[i]->declarator->declaratorArray->type->name == "void"){
                      error(@1, "Cannot declare the type of a parameter as void");
                    }
                    // st->addEntry(new SymbolTableEntry($4->parameterDeclarationList[i]->declarator->declaratorArray->name, "var", "param", $4->parameterDeclarationList[i]->typeSpecifier->size, paramoffset, $4->parameterDeclarationList[i]->typeSpecifier->name, $4->parameterDeclarationList[i]->declarator->declaratorArray->type));
                    st->addEntry(new SymbolTableEntry($4->parameterDeclarationList[i]->declarator->declaratorArray->name, "var", "param", $4->parameterDeclarationList[i]->declarator->declaratorArray->type->size, paramoffset, $4->parameterDeclarationList[i]->declarator->declaratorArray->type->name, $4->parameterDeclarationList[i]->declarator->declaratorArray->type));
                    paramoffset += $4->parameterDeclarationList[i]->declarator->declaratorArray->type->size;
                  }
                }
              }
              | IDENTIFIER 
              '(' ')'
              {
                for(auto i: gst.entries){
                  if(i.first == $1 && i.second->varfuntype == "fun"){
                    error(@1, "\""+$1+"\" has a previous declaration");
                  }
                }
                
                st = new SymbolTable();
                paramoffset = 12;
                localoffset = 0;
                functionName = $1;
                isFunction = true;
                $$ = new funDeclaratorClass($1);
              }
              ;

parameter_list: 
              parameter_declaration
              {
                $$ = new parameterListClass();
                $$->addParameter($1);
                // st->addEntry(new SymbolTableEntry($1->declarator->declaratorArray->name, "var", "param", $1->typeSpecifier->size, paramoffset, $1->typeSpecifier->name, $1->declarator->declaratorArray->type));
                // if(isFunction && $$->parameterDeclarationList.size()==1)
                // {
                //   for(int i=$$->parameterDeclarationList.size()-1; i>=0; i--)
                //   {
                //     for(auto j: st->entries){
                //       if(j.first == $$->parameterDeclarationList[i]->declarator->declaratorArray->name){
                //         error(@1, "\""+$$->parameterDeclarationList[i]->declarator->declaratorArray->name+"\" has a previous declaration");
                //       }
                //     }
                //     st->addEntry(new SymbolTableEntry($$->parameterDeclarationList[i]->declarator->declaratorArray->name, "var", "param", $$->parameterDeclarationList[i]->typeSpecifier->size, paramoffset, $$->parameterDeclarationList[i]->typeSpecifier->name, $$->parameterDeclarationList[i]->declarator->declaratorArray->type));
                //     paramoffset += $$->parameterDeclarationList[i]->typeSpecifier->size;
                //   }
                // }
              }
              | parameter_list ',' parameter_declaration
              {
                $$ = $1;
                $$->addParameter($3);
                // st = new SymbolTable();
                // if(isFunction)
                // {
                //   for(int i=$$->parameterDeclarationList.size()-1; i>=0; i--)
                //   {
                //     for(auto j: st->entries){
                //       if(j.first == $$->parameterDeclarationList[i]->declarator->declaratorArray->name){
                //         error(@1, "\""+$$->parameterDeclarationList[i]->declarator->declaratorArray->name+"\" has a previous declaration");
                //       }
                //     }
                //     st->addEntry(new SymbolTableEntry($$->parameterDeclarationList[i]->declarator->declaratorArray->name, "var", "param", $$->parameterDeclarationList[i]->typeSpecifier->size, paramoffset, $$->parameterDeclarationList[i]->typeSpecifier->name, $$->parameterDeclarationList[i]->declarator->declaratorArray->type));
                //     paramoffset += $$->parameterDeclarationList[i]->typeSpecifier->size;
                //   }
                // }
              }
              ;

parameter_declaration: 
                    type_specifier declarator
                    {
                      $$ = new parameterDeclarationClass($1, $2);
                    }
                    ;

declarator_arr: 
              IDENTIFIER
              {
                $$ = new declaratorArrayClass($1, new Type(type__));
              }
              | declarator_arr '[' INT_CONST ']'
              {
                $$ = new declaratorArrayClass($1->name, insertField($1->type, stoi($3)));
              }
              ;
declarator: 
          declarator_arr
          {
            $$ = new declaratorClass($1);
            parenthesis = false;
          }
          | '*'
          {
            type__ = insertPointer(type__);         
            // type_ = type_ + "*";
            if(isCurrentStruct){
              isCurrentStruct = false;
            }
          }
          declarator
          {
            $$ = new declaratorClass(new declaratorArrayClass($3->declaratorArray->name, ($3->declaratorArray->type)));
          }
          ;

compound_statement:
                  '{' '}'
                  {
                    $$ = new seq_astnode();
                  }
                  | '{' statement_list '}'
                  {
                    $$ = $2;
                  }
                  | '{' declaration_list '}'
                  {
                    $$ = new seq_astnode();
                  }
                  | '{' declaration_list statement_list '}'
                  {
                    $$ = $3;
                  }
                  ;

statement_list: 
              statement
              {
                $$ = new seq_astnode();
                $$->addStatement($1);
              }
              | statement_list statement
              {
                $$ = $1;
                $$->addStatement($2);
              }
              ;

statement: 
        ';'
        {
          $$ = new empty_astnode();
        }
        | '{' statement_list '}'
        {
          $$ = $2;
        }
        | selection_statement
        {
          $$ = $1;
        }
        | iteration_statement
        {
          $$ = $1;
        }
        | assignment_statement
        {
          $$ = $1;
        }
        | procedure_call
        {
          $$ = $1;
          // cout << "procedure call finished" << endl;
        }
        | RETURN expression ';'
        {
          if(functionReturnType->name == "int" && $2->type->name == "int"){
            $$ = new return_astnode($2);
          }else if(functionReturnType->name == "float" && $2->type->name == "float"){
            $$ = new return_astnode($2);
          }else if(functionReturnType->name == "int" && $2->type->name == "float"){
            $$ = new return_astnode(new op_unary_astnode("TO_INT",$2));
          }else if(functionReturnType->name == "float" && $2->type->name == "int"){
            $$ = new return_astnode(new op_unary_astnode("TO_FLOAT",$2));
          }else if(functionReturnType->name == $2->type->name){
            $$ = new return_astnode($2);
          }else{
            error(@1, "Incompatible type \""+$2->type->name+"\" returned, expected \""+functionReturnType->name+"\"");
          }
        }
        ;

assignment_expression: 
                    unary_expression '='
                    expression
                    {
                      // if(isDefined($1->name) == false){
                      //   error(@1, "\""+$1->name+"\" is not defined");
                      // }
                      // cout << $1->type->name << endl;
                      // cout << $3->type->name << endl;
                      // cout << $3->isZero << endl;
                      // cout << $1->type->derefpointers+$1->type->fields.size()+$1->type->npointers << endl;
                      // cout << ($1->type->derefpointers+$1->type->fields.size()+$1->type->npointers > 0 && $3->isZero == true) << endl;
                      if($1->lvalue == true){
                        // cout << "entered" << endl;
                        if($1->type->name == "int" && $3->type->name == "int"){
                          $$ = new assignE_astnode($1, $3);
                        }else if($1->type->name == "float" && $3->type->name == "float"){
                          $$ = new assignE_astnode($1, $3);
                        }else if($1->type->name == "int" && $3->type->name == "float"){
                          $$ = new assignE_astnode(new op_unary_astnode("TO_FLOAT",$1), $3);
                        }else if($1->type->name == "float" && $3->type->name == "int"){
                          $$ = new assignE_astnode($1, new op_unary_astnode("TO_FLOAT",$3));
                        }else if(compatibleForAssignment($1->type, $3->type)){
                          $$ = new assignE_astnode($1, $3); // should there be any casting here? like TO_INT or TO_FLOAT?
                        }else if($1->type->name == $3->type->name){
                          $$ = new assignE_astnode($1, $3);
                        }else if($1->type->name == "void*" && $3->type->derefpointers+$3->type->fields.size()+$3->type->npointers>0){
                          $$ = new assignE_astnode($1, $3);
                        }else if($3->type->name == "void*" && $1->type->derefpointers+$1->type->fields.size()+$1->type->npointers>0){
                          $$ = new assignE_astnode($1, $3);
                        }else if($1->type->derefpointers+$1->type->fields.size()+$1->type->npointers>0 && $3->isZero == true){
                          // cout << "here" << endl;
                          $$ = new assignE_astnode($1, $3);
                        }
                        else{
                          error(@1, "Incompatible assignment when assigning to type \""+$1->type->name+"\" from type \""+$3->type->name+"\"");
                        }
                      // cout << $1->type->name << endl;
                      // cout << $4->type->name << endl;
                      // if($1->lvalue == true){
                      //   if($1->type->name == "int" && $4->type->name == "int"){
                      //     $$ = new assignE_astnode($1, $4);
                      //   }else if($1->type->name == "float" && $4->type->name == "float"){
                      //     $$ = new assignE_astnode($1, $4);
                      //   }else if($1->type->name == "int" && $4->type->name == "float"){
                      //     $$ = new assignE_astnode(new op_unary_astnode("TO_FLOAT",$1), $4);
                      //   }else if($1->type->name == "float" && $4->type->name == "int"){
                      //     $$ = new assignE_astnode($1, new op_unary_astnode("TO_FLOAT",$4));
                      //   }else if(compatibleForAssignment($1->type, $4->type)){
                      //     $$ = new assignE_astnode($1, $4); // should there be any casting here? like TO_INT or TO_FLOAT?
                      //   }else if($1->type->name == $4->type->name){
                      //     $$ = new assignE_astnode($1, $4);
                      //   }else if($1->type->name == "void*" && $4->type->derefpointers+$4->type->fields.size()+$4->type->npointers>0){
                      //     $$ = new assignE_astnode($1, $4);
                      //   }
                      //   else{
                      //     error(@1, "Incompatible assignment when assigning to type \""+$1->type->name+"\" from type \""+$3->type->name+"\"");
                      //   }
                      }else{
                        error(@1, "Left operand of assignment should have an lvalue");
                      } 
                    }
                    ;

assignment_statement: 
                    assignment_expression ';'
                    {
                      $$ = new assignS_astnode($1->lhs, $1->rhs);
                    }
                    ;

procedure_call: 
              IDENTIFIER '(' ')' ';'
              {
                for(auto abc: predefined){
                  // cout << abc.first << endl;
                  // cout << "1 "<<$1 << endl;
                  if(abc.first == $1){
                    foundinPredefined = true;
                    $$ = new proccall_astnode(new identifier_astnode($1));
                  }
                }
                if(foundinPredefined == false){
                  doneinLoop = false;
                  parameterLength = 0;
                  for(auto i: gst.entries){
                    if(i.first == $1 && i.second->varfuntype == "fun"){
                      for(auto j: i.second->child->entries){
                        if(j.second->scope == "param"){
                          parameterLength++;
                        }
                      }
                      if(parameterLength == 0){
                        doneinLoop = true;
                        $$ = new proccall_astnode(new identifier_astnode($1));
                      }else{
                        error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                      }
                    }
                  }
                  if(functionName == $1){
                    for(auto ij: st->entries){
                      if(ij.second->scope == "param"){
                        parameterLength++;
                      }
                    }
                    if(parameterLength == 0){
                      $$ = new proccall_astnode(new identifier_astnode($1));
                    }else{
                      error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                    }
                  }
                  else if(!doneinLoop){
                    error(@1, "Procedure \""+$1+"\"   not declared");
                  }
                }
                foundinPredefined = false;
              }
              | IDENTIFIER '(' expression_list ')' ';'
              {
                for(auto abc: predefined){
                  // cout << abc.first << endl;
                  // cout << "1 "<<$1 << endl;
                  if(abc.first == $1){
                    foundinPredefined = true;
                    $$ = new proccall_astnode(new identifier_astnode($1), $3->args);
                    // cout << "1" << $1 << endl;
                  }
                }
                if(foundinPredefined == false){
                  parameterLength = 0;
                  doneinLoop = false;
                  for(auto i: gst.entries){
                    if(i.first == $1 && i.second->varfuntype == "fun"){
                      for(auto j: i.second->child->entries){
                        if(j.second->scope == "param"){
                          parameterLength++;
                        }
                      }
                      if(parameterLength==$3->args.size()){
                        parameters = sortedParam(i.second->child);
                        for(int p=0;p<parameterLength;p++){
                          if(compForParamPassing(compatibleForParameterPassing(parameters[p], $3->args[p]->type))){
                            continue;
                          }else if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==3){
                            $3->args[p] = new op_unary_astnode("TO_INT", $3->args[p]);
                          }if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==4){
                            $3->args[p] = new op_unary_astnode("TO_FLOAT", $3->args[p]);
                          }
                          else{
                            error(@1, "Expected \""+convertToDeref(parameters[p])->name+"\" but argument is of type \""+convertToDeref($3->args[p]->type)->name+"\"");
                          }
                        }
                        doneinLoop = true;
                        $$ = new proccall_astnode(new identifier_astnode($1), $3->args);
                      }else if(parameterLength<$3->args.size()){
                        error(@1, "Procedure \""+$1+"\"  called with too many arguments");
                      }else if(parameterLength>$3->args.size()){
                        error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                      }
                    }
                  }
                  if(functionName == $1){
                    for(auto ij: st->entries){
                      if(ij.second->scope == "param"){
                        parameterLength++;
                      }
                    }
                    if(parameterLength==$3->args.size()){
                      parameters = sortedParam(st);
                      for(int p=0;p<parameterLength;p++){
                        if(compForParamPassing(compatibleForParameterPassing(parameters[p], $3->args[p]->type))){
                          continue;
                        }else if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==3){
                          $3->args[p] = new op_unary_astnode("TO_INT", $3->args[p]);
                        }if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==4){
                          $3->args[p] = new op_unary_astnode("TO_FLOAT", $3->args[p]);
                        }else{
                          error(@1, "Expected \""+convertToDeref(parameters[p])->name+"\" but argument is of type \""+convertToDeref($3->args[p]->type)->name+"\"");
                        }
                      }
                      $$ = new proccall_astnode(new identifier_astnode($1), $3->args);
                    }else if(parameterLength<$3->args.size()){
                      error(@1, "Procedure \""+$1+"\"  called with too many arguments");
                    }else if(parameterLength>$3->args.size()){
                      error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                    }
                  }
                  else if(!doneinLoop){
                    error(@1, "Procedure \""+$1+"\"   not declared");
                  }
                }
                foundinPredefined = false;
              }
              ;
               

expression: 
          logical_and_expression
          {
            $$ = $1;
          }
          | expression OR_OP logical_and_expression
          {
            if($1->type->name == "tring" || $3->type->name == "tring"){
              error(@1, "Invalid operand of ||,  not scalar or pointer");
            }
            $$ = new op_binary_astnode("OR_OP", $1, $3);
            $$->type = new Type("int");
          }
          ;
logical_and_expression: 
                      equality_expression
                      {
                        $$ = $1;
                      }
                      | logical_and_expression AND_OP equality_expression
                      {
                        if($1->type->name == "tring" || $3->type->name == "tring"){
                          error(@1, "Invalid operand of &&,  not scalar or pointer");
                        }
                        $$ = new op_binary_astnode("AND_OP", $1, $3);
                        $$->type = new Type("int");
                      }
                      ;
   
equality_expression: 
                  relational_expression
                  {
                    $$ = $1;
                  }
                  | equality_expression EQ_OP relational_expression
                  {
                    if($1->type->name == "int" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("EQ_OP_INT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "float" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("EQ_OP_FLOAT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "int" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("EQ_OP_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "float" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("EQ_OP_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                      $$->type = new Type("int");
                    }else if(compatibleForParameters($1->type, $3->type)){
                      $$ = new op_binary_astnode("EQ_OP_INT", $1, $3); // should there be any casting here? like TO_INT or TO_FLOAT?
                      $$->type = new Type("int");
                    }else if($1->type->name == "void*" && $3->isZero == true){
                      $$ = new op_binary_astnode("EQ_OP_INT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->isZero == true && $3->type->name == "void*"){
                      $$ = new op_binary_astnode("EQ_OP_INT", $1, $3);
                      $$->type = new Type("int");
                    }else{
                      error(@1, "Invalid operands types for binary ==, \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                    }
                  }
                  | equality_expression NE_OP relational_expression
                  {
                    if($1->type->name == "int" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("NE_OP_INT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "float" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("NE_OP_FLOAT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "int" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("NE_OP_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "float" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("NE_OP_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                      $$->type = new Type("int");
                    }else if(compatibleForParameters($1->type, $3->type)){
                      $$ = new op_binary_astnode("NE_OP_INT", $1, $3);
                      $$->type = new Type("int");
                    }
                    else{
                      error(@1, "Invalid operands types for binary !=, \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                    }
                  }
                  ;

relational_expression: 
                    additive_expression
                    {
                      $$ = $1;
                    }
                    | relational_expression '<' additive_expression
                    {
                      if($1->type->name == "int" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("LT_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("LT_OP_FLOAT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "int" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("LT_OP_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("LT_OP_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                        $$->type = new Type("int");
                      }else if(compatibleForParameters($1->type, $3->type)){
                        $$ = new op_binary_astnode("LT_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else{
                        error(@1, "Invalid operands types for binary < , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                      }
                    }
                    | relational_expression '>' additive_expression
                    {
                      if($1->type->name == "int" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("GT_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("GT_OP_FLOAT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "int" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("GT_OP_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("GT_OP_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                        $$->type = new Type("int");
                      }else if(compatibleForParameters($1->type, $3->type)){
                        $$ = new op_binary_astnode("GT_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else{
                        error(@1, "Invalid operand types for binary > , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                      }
                    }
                    | relational_expression LE_OP additive_expression
                    {
                      if($1->type->name == "int" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("LE_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("LE_OP_FLOAT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "int" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("LE_OP_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("LE_OP_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                        $$->type = new Type("int");
                      }else if(compatibleForParameters($1->type, $3->type)){
                        $$ = new op_binary_astnode("LE_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else{
                        error(@1, "Invalid operand types for binary <= , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                      }
                    }
                    | relational_expression GE_OP additive_expression
                    {
                      if($1->type->name == "int" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("GE_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("GE_OP_FLOAT", $1, $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "int" && $3->type->name == "float"){
                        $$ = new op_binary_astnode("GE_OP_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                        $$->type = new Type("int");
                      }else if($1->type->name == "float" && $3->type->name == "int"){
                        $$ = new op_binary_astnode("GE_OP_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                        $$->type = new Type("int");
                      }else if(compatibleForParameters($1->type, $3->type)){
                        $$ = new op_binary_astnode("GE_OP_INT", $1, $3);
                        $$->type = new Type("int");
                      }else{
                        error(@1, "Invalid operand types for binary >= , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                      }
                    }
                    ;

additive_expression: 
                  multiplicative_expression
                  {
                    $$ = $1;
                  }
                  | additive_expression '+' multiplicative_expression
                  {
                    if($1->type->name == "int" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("PLUS_INT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "float" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("PLUS_FLOAT", $1, $3);
                      $$->type = new Type("float");
                    }else if($1->type->name == "int" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("PLUS_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                      $$->type = new Type("float");
                    }else if($1->type->name == "float" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("PLUS_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                      $$->type = new Type("float");
                    }else if($1->type->name == "int" && checkPointerToAdd($3->type)){
                      $$ = new op_binary_astnode("PLUS_INT", $1, $3);
                      $$->type = evaluateTypeOfAdd($3->type);
                    }else if($3->type->name == "int" && checkPointerToAdd($1->type)){
                      $$ = new op_binary_astnode("PLUS_INT", $1, $3);
                      $$->type = evaluateTypeOfAdd($1->type);
                    }else{
                      error(@1, "Invalid operand types for binary + , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                    }
                  }
                  | additive_expression '-' multiplicative_expression
                  {
                    if($1->type->name == "int" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("MINUS_INT", $1, $3);
                      $$->type = new Type("int");
                    }else if($1->type->name == "float" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("MINUS_FLOAT", $1, $3);
                      $$->type = new Type("float");
                    }else if($1->type->name == "int" && $3->type->name == "float"){
                      $$ = new op_binary_astnode("MINUS_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                      $$->type = new Type("float");
                    }else if($1->type->name == "float" && $3->type->name == "int"){
                      $$ = new op_binary_astnode("MINUS_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                      $$->type = new Type("float");
                    }else if($3->type->name == "int" && checkPointerToAdd($1->type)){
                      $$ = new op_binary_astnode("MINUS_INT", $1, $3);
                      $$->type = evaluateTypeOfAdd($1->type);
                    }else if(compatibleForParameters($1->type, $3->type)){
                      $$ = new op_binary_astnode("MINUS_INT", $1, $3);
                      $$->type = new Type("int");
                    }else{
                      error(@1, "Invalid operand types for binary - , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                    }
                  }
                  ;

unary_expression: 
                postfix_expression
                { 
                  $$ = $1;
                }
                | unary_operator unary_expression
                {
                  if($1 == "ADDRESS"){
                    if($2->lvalue == false){
                      error(@1, "Operand of \"&\" should  have lvalue");
                    }
                    $$ = new op_unary_astnode($1, $2);
                    $$->type = insertDerefPointer($2->type);
                    $$->lvalue = false;
                  }else if($1 == "DEREF"){
                    if(canDeref($2->type) == false){
                      error(@1, "Invalid operand type \""+$2->type->name+"\" of  unary *");
                    }
                    $$ = new op_unary_astnode($1, $2);
                    $$->type = removePointer($2->type); 
                  }else if($1 == "UMINUS"){
                    if($2->type->name != "int" && $2->type->name != "float"){
                      error(@1, "Operand of unary - should be an int or float");
                    }
                    $$ = new op_unary_astnode($1, $2);
                    $$->type = $2->type;
                    $$->lvalue = false;
                  }else if($1 == "NOT"){
                    if(checklvalueForNOT($2->type) == false){
                      error(@1, "Operand of NOT should be an int or float or pointer");
                    }
                    $$ = new op_unary_astnode($1, $2);
                    $$->type = new Type("int"); //check this
                    $$->lvalue = false;
                  }
                }
                ;

multiplicative_expression: 
                        unary_expression
                        {
                          $$ = $1;
                        }
                        | multiplicative_expression '*' unary_expression
                        {
                          if($1->type->name == "int" && $3->type->name == "int"){
                            $$ = new op_binary_astnode("MULT_INT", $1, $3);
                            $$->type = new Type("int");
                          }else if($1->type->name == "float" && $3->type->name == "float"){
                            $$ = new op_binary_astnode("MULT_FLOAT", $1, $3);
                            $$->type = new Type("float");
                          }else if($1->type->name == "int" && $3->type->name == "float"){
                            $$ = new op_binary_astnode("MULT_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                            $$->type = new Type("float");
                          }else if($1->type->name == "float" && $3->type->name == "int"){
                            $$ = new op_binary_astnode("MULT_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                            $$->type = new Type("float");
                          }else{
                            error(@1, "Invalid operand types for binary * , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                          }
                        }
                        | multiplicative_expression '/' unary_expression
                        {if($1->type->name == "int" && $3->type->name == "int"){
                            $$ = new op_binary_astnode("DIV_INT", $1, $3);
                            $$->type = new Type("int");
                          }else if($1->type->name == "float" && $3->type->name == "float"){
                            $$ = new op_binary_astnode("DIV_FLOAT", $1, $3);
                            $$->type = new Type("float");
                          }else if($1->type->name == "int" && $3->type->name == "float"){
                            $$ = new op_binary_astnode("DIV_FLOAT", new op_unary_astnode("TO_FLOAT",$1), $3);
                            $$->type = new Type("float");
                          }else if($1->type->name == "float" && $3->type->name == "int"){
                            $$ = new op_binary_astnode("DIV_FLOAT", $1, new op_unary_astnode("TO_FLOAT",$3));
                            $$->type = new Type("float");
                          }else{
                            error(@1, "Invalid operand types for binary / , \""+$1->type->name+"\" and \""+$3->type->name+"\"");
                          }
                        }
                        ;

postfix_expression:
                  primary_expression
                  {
                    $$ = $1;
                  }
                  | postfix_expression '[' expression ']'
                  {
                    if(canSubscript($1->type) == false){
                      error(@1, "Subscripted value is neither array nor pointer");
                    }
                    if($3->type->name != "int"){
                      error(@1, "array subscript is not an integer");
                    }
                    // cout << $1->type->name << endl;
                    // cout << $3->type->name << endl;
                    $$ = new arrayref_astnode($1, $3);
                    $$->type = evaluateTypeOfSubscriptable($1->type);
                    // cout << $$->type->name << endl;
                  }
                  | IDENTIFIER '(' ')'
                  {
                    // cout << "here identifier" << $1 << endl;
                    for(auto abc: predefined){
                      // cout << abc.first << endl;
                      // cout << "1 "<<$1 << endl;
                      if(abc.first == $1){
                        foundinPredefined = true;
                        $$ = new funcall_astnode(new identifier_astnode($1));
                        $$->type = abc.second;
                        $$->lvalue = false;
                        // cout<<$$->type->name<<endl;
                      }
                    }
                    if(foundinPredefined == false){
                      parameterLength = 0;
                      doneinLoop = false;
                      for(auto i: gst.entries){
                        if(i.first == $1 && i.second->varfuntype == "fun"){
                          for(auto j: i.second->child->entries){
                            if(j.second->scope == "param"){
                              parameterLength++;
                            }
                          }
                          // cout << "parameterLength " << parameterLength << endl;
                          if(parameterLength == 0){
                            doneinLoop = true;
                            // cout << "here" << endl;
                            $$ = new funcall_astnode(new identifier_astnode($1));
                            $$->type = i.second->retType;
                            $$->lvalue = false;
                            // cout<<$$->type->name<<endl;
                          }else{
                            error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                          }
                        }
                      }
                      if(functionName == $1){
                        for(auto ij: st->entries){
                          if(ij.second->scope == "param"){
                            parameterLength++;
                          }
                        }
                        if(parameterLength == 0){
                          $$ = new funcall_astnode(new identifier_astnode($1));
                          $$->type = functionReturnType;
                          $$->lvalue = false;
                        }else{
                          error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                        }
                      }
                      else if(!doneinLoop){
                        error(@1, "Procedure \""+$1+"\"   not declared");
                      }
                    }
                    foundinPredefined = false;
                  }
                  | IDENTIFIER '(' expression_list ')'
                  {
                    for(auto abc: predefined){
                      // cout << abc.first << endl;
                      // cout << "1 "<<$1 << endl;
                      if(abc.first == $1){
                        foundinPredefined = true;
                        $$ = new funcall_astnode(new identifier_astnode($1), $3->args);
                        $$->type = abc.second;
                        $$->lvalue = false;
                      }
                    }
                    if(foundinPredefined == false){
                      parameterLength = 0;
                      doneinLoop = false;
                      for(auto i: gst.entries){
                        if(i.first == $1 && i.second->varfuntype == "fun"){
                          for(auto j: i.second->child->entries){
                            if(j.second->scope == "param"){
                              parameterLength++;
                            }
                          }
                          if(parameterLength==$3->args.size()){
                            parameters = sortedParam(i.second->child);
                            for(int p=0;p<parameterLength;p++){
                              if(compForParamPassing(compatibleForParameterPassing(parameters[p], $3->args[p]->type))){
                                continue;
                              }else if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==3){
                                $3->args[p] = new op_unary_astnode("TO_INT", $3->args[p]);
                              }if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==4){
                                $3->args[p] = new op_unary_astnode("TO_FLOAT", $3->args[p]);
                              }else{
                                error(@1, "Expected \""+convertToDeref(parameters[p])->name+"\" but argument is of type \""+convertToDeref($3->args[p]->type)->name+"\"");
                              }
                            }
                            doneinLoop = true;
                            $$ = new funcall_astnode(new identifier_astnode($1), $3->args);
                            $$->type = i.second->retType;
                            $$->lvalue = false;
                          }else if(parameterLength<$3->args.size()){
                            error(@1, "Procedure \""+$1+"\"  called with too many arguments");
                          }else if(parameterLength>$3->args.size()){
                            error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                          }
                        }
                      }
                      if(functionName == $1){
                        for(auto ij: st->entries){
                          if(ij.second->scope == "param"){
                            parameterLength++;
                          }
                        }
                        if(parameterLength==$3->args.size()){
                          parameters = sortedParam(st);
                          for(int p=0;p<parameterLength;p++){
                            if(compForParamPassing(compatibleForParameterPassing(parameters[p], $3->args[p]->type))){
                              continue;
                            }else if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==3){
                              $3->args[p] = new op_unary_astnode("TO_INT", $3->args[p]);
                            }if(compatibleForParameterPassing(parameters[p], $3->args[p]->type)==4){
                              $3->args[p] = new op_unary_astnode("TO_FLOAT", $3->args[p]);
                            }else{
                              error(@1, "Expected \""+convertToDeref(parameters[p])->name+"\" but argument is of type \""+convertToDeref($3->args[p]->type)->name+"\"");
                            }
                          }
                          $$ = new funcall_astnode(new identifier_astnode($1), $3->args);
                          $$->type = functionReturnType;
                          $$->lvalue = false;
                        }else if(parameterLength<$3->args.size()){
                          error(@1, "Procedure \""+$1+"\"  called with too many arguments");
                        }else if(parameterLength>$3->args.size()){
                          error(@1, "Procedure \""+$1+"\"  called with too few arguments");
                        }
                      }
                      else if(!doneinLoop){
                        error(@1, "Procedure \""+$1+"\"   not declared");
                      }
                    }
                    foundinPredefined = false;
                  }
                  | postfix_expression '.' IDENTIFIER
                  {
                    if(checkStruct($1->type) == false){
                      error(@1, "Left operand of \".\"  is not a  structure");
                    }
                    $$ = new member_astnode($1, new identifier_astnode($3));
                    // cout<<$1->type->name<<endl;
                    // cout<<$3<<endl;
                    for(auto i: gst.entries)
                    {
                      if(i.first == $1->type->name)
                      {
                        // cout<<"here3"<<endl;
                        for(auto j: i.second->child->entries){
                          if(j.first == $3){
                            // cout<<"here4"<<endl;
                            $$->type = j.second->retType;
                            // cout<<$$->type->name<<endl;
                            break;
                          }
                        }
                      }
                    }
                  }
                  | postfix_expression PTR_OP IDENTIFIER
                  {
                    if(checkPointerToStruct($1->type) == false){
                      error(@1, "Left operand of \"->\"  is not a pointer to structure");
                    }
                    $$ = new arrow_astnode($1, new identifier_astnode($3));
                    for(auto i: gst.entries)
                    {
                      if(i.first + "*" == $1->type->name  || i.first + "(*)" == $1->type->name || i.first == $1->type->name.substr(0, i.first.length()))
                      {
                        for(auto j: i.second->child->entries){
                          if(j.first == $3){
                            $$->type = j.second->retType;
                            break;
                          }
                        }
                      }
                    }
                  }
                  | postfix_expression INC_OP
                  {
                    if($1->lvalue == false || checklvalue($1->type) == false){
                      error(@1, "Operand of \"++\" should be a int, float or pointer");
                    }
                    $$ = new op_unary_astnode("PP", $1);
                    $$->type = $1->type;
                    $$->lvalue = false;
                  }
                  ;

primary_expression: 
                  IDENTIFIER
                  {
                    if(isDefined(gst, $1) || isDefinedPointer(st, $1) ){
                      $$ = new identifier_astnode($1);
                      for(auto i: gst.entries)
                      {
                        if(i.first == $1 || i.first == "struct "+$1)
                        {
                          $$->type = i.second->retType;
                          break;
                        }
                      }
                      for(auto j: st->entries)
                      {
                        if(j.first == $1 || j.first == "struct "+$1)
                        {
                          $$->type = j.second->retType;
                          break;
                        }
                      }
                      $$->lvalue = true;
                    }else{
                      error(@1, "Variable \""+$1+"\" not declared");
                    }
                  }
                  | INT_CONST
                  {
                    $$ = new intconst_astnode(stoi($1));
                    $$->type = new Type("int");
                    $$->lvalue = false;
                    if(stoi($1)==0){
                      $$->isZero = true;
                    }
                  }
                  | FLOAT_CONST
                  {
                    $$ = new floatconst_astnode(stof($1));
                    $$->type = new Type("float");
                    $$->lvalue = false;
                  }
                  | STRING_LITERAL
                  {
                    $$ = new stringconst_astnode($1);
                    $$->lvalue = false;
                    $$->type = new Type("tring", $1.size());
                  }
                  | '(' expression ')'
                  {
                    $$ = $2;
                  }
                  ;

expression_list: 
              expression
              {
                $$ = new proccall_astnode();
                $$->addArg($1);
              }
              | expression_list ',' expression
              {
                $$ = $1;
                $$->addArg($3);
              }
              ;

unary_operator: 
              '-'
              {
                $$ = "UMINUS";
              }
              | '!'
              {
                $$ = "NOT";
              }
              | '&'
              {
                $$ = "ADDRESS";
              }
              | '*'
              {
                $$ = "DEREF";
              }
              ;

selection_statement: 
                  IF '(' expression ')' statement ELSE statement
                  {
                    if($3->type->name == "int" || $3->type->name == "float" || $3->type->derefpointers+$3->type->npointers+$3->type->fields.size() > 0){
                      $$ = new if_astnode($3, $5, $7);
                    }else{
                      error(@1, "invalid condition in if");
                    }
                    
                  }
                  ;

iteration_statement: 
                  WHILE '(' expression ')' statement
                  {
                    if($3->type->name == "int" || $3->type->name == "float" || $3->type->derefpointers+$3->type->npointers+$3->type->fields.size() > 0){
                      $$ = new while_astnode($3, $5);
                    }else{
                      error(@1, "invalid condition in while");
                    }
                  }
                  | FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement
                  {
                    if($5->type->name == "int" || $5->type->name == "float" || $5->type->derefpointers+$3->type->npointers+$3->type->fields.size() > 0){
                      $$ = new for_astnode($3, $5, $7, $9);
                    }else{
                      error(@1, "invalid condition in for");
                    }
                  }
                  ;

declaration_list: 
                declaration
                {
                  $$ = new declarationListClass();
                  $$->addDeclaration($1);
                  // if(isStruct){
                  //   for(auto j: $1->declaratorList->declaratorList){
                  //     structSize += j->declaratorArray->type->size;
                  //     // cout<<structName<<" "<<structSize<<endl;
                  //   }
                  // }
                }
                | declaration_list declaration
                {
                  $$ = $1;
                  $$->addDeclaration($2);
                  // if(isStruct){
                  //   for(auto i: $$->declarationList){
                  //     for(auto j: i->declaratorList->declaratorList){
                  //       structSize += j->declaratorArray->type->size;
                  //       cout<<structName<<" "<<structSize<<endl;
                  //     }
                  //   }
                  // }
                }
                ;

declarator_list: 
              declarator
              {
                $$ = new declaratorListClass();
                $$->addDeclarator($1);
              }
              | declarator_list ','
              {
                
                type__ = new Type(type_,typeSize);
                // type__->print();
              }
              declarator
              {
                $$ = $1;
                $$->addDeclarator($4);
                // cout<<$4->declaratorArray->name<<endl;
                // $4->declaratorArray->type->print();
              }
              ;

declaration: 
          type_specifier declarator_list ';'
          {
            $$ = new declarationClass($1, $2);
            // if(isStruct){
            //   for(auto i: $2->declaratorList){
            //     structSize += i->declaratorArray->type->size;
            //   }
            // }
            if(isStruct){
              for(auto i: $2->declaratorList){
                // have to check if the variable is already defined
                if(i->declaratorArray->name == structName){
                  error(@1, "\""+structName+"\" is not defined");
                }
                if(checkVoid(i->declaratorArray->type)){
                  error(@1, "Cannot declare variable of type \"void\"");
                }
                for(auto j: st->entries){
                  if(j.first == i->declaratorArray->name){
                    error(@1, "\""+i->declaratorArray->name+"\" has a previous declaration");
                  }
                }
                structSize += i->declaratorArray->type->size;
                st->addEntry(new SymbolTableEntry(i->declaratorArray->name, "var", "local", i->declaratorArray->type->size, offset, i->declaratorArray->type->name, i->declaratorArray->type));
                offset += i->declaratorArray->type->size;
              }
            }else if(isFunction){
              for(auto i: $2->declaratorList){
                if(checkVoid(i->declaratorArray->type)){
                  error(@1, "Cannot declare variable of type \"void\"");
                }
                for(auto j: st->entries){
                  if(j.first == i->declaratorArray->name){
                    error(@1, "\""+i->declaratorArray->name+"\" has a previous declaration");
                  }
                }
                localoffset -= i->declaratorArray->type->size;
                st->addEntry(new SymbolTableEntry(i->declaratorArray->name, "var", "local", i->declaratorArray->type->size, localoffset, i->declaratorArray->type->name, i->declaratorArray->type));
              }
            }
          }
          ;
%%
void IPL::Parser::error(const location_type&l,const std::string &err_message){
  std::cout<<"Error at line "<<l.end.line<<": "<<err_message<<"\n";
  exit(1);
}
