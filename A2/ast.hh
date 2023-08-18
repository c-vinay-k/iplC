#ifndef AST_HH
#define AST_HH
#include "symbtab.hh"
enum typeExp
{
    Empty_astnode,
    Seq_astnode,
    AssignS_astnode,
    Return_astnode,
    If_astnode,
    While_astnode,
    For_astnode,
    Proccall_astnode,
    Op_binary_astnode,
    Op_unary_astnode,
    AssignE_astnode,
    Funcall_astnode,
    Intconst_astnode,
    Floatconst_astnode,
    Stringconst_astnode,
    IDENTIFIER_astnode,
    Arrayref_astnode,
    Member_astnode,
    Arrow_astnode,
};
using namespace std;


class abstract_astnode{
    public:
        virtual void print(int blanks) = 0;
        enum typeExp astnode_type;
        virtual ~abstract_astnode() {};
};

class statement_astnode : public abstract_astnode{
    public:
        virtual void print(int blanks) = 0;
        virtual ~statement_astnode() {};
};

class exp_astnode : public abstract_astnode{
    public:
        virtual void print(int blanks) = 0;
        virtual ~exp_astnode() {};
        Type* type;
        bool lvalue = true;
        bool isZero = false;
        exp_astnode();
        exp_astnode(Type* type);
};

class ref_astnode : public exp_astnode{
    public:
        virtual void print(int blanks) = 0;
        virtual ~ref_astnode() {};
};

class empty_astnode : public statement_astnode{
    public:
        empty_astnode();
        void print(int blanks);
};

class seq_astnode : public statement_astnode{
    public:
        vector<statement_astnode*> statements;
        seq_astnode();
        seq_astnode(vector<statement_astnode*> statements);
        void addStatement(statement_astnode* statement);
        void print(int blanks);
};

class assignS_astnode : public statement_astnode{
    public:
        exp_astnode* lhs;
        exp_astnode* rhs;
        assignS_astnode();
        assignS_astnode(exp_astnode* lhs, exp_astnode* rhs);
        void print(int blanks);
};

class return_astnode : public statement_astnode{
    public:
        exp_astnode* exp;
        return_astnode();
        return_astnode(exp_astnode* exp);
        void print(int blanks);
};

class identifier_astnode : public ref_astnode{
    public:
        string name;
        identifier_astnode();
        identifier_astnode(string name);
        void print(int blanks);
};

class proccall_astnode : public statement_astnode{
    public:
        identifier_astnode* fname;
        vector<exp_astnode*> args;
        proccall_astnode();
        proccall_astnode(identifier_astnode* fname, vector<exp_astnode*> args);
        proccall_astnode(identifier_astnode* fname);
        void addArg(exp_astnode* arg);
        void print(int blanks);
};

class if_astnode : public statement_astnode{
    public:
        exp_astnode* cond;
        statement_astnode* then_;
        statement_astnode* else_;
        if_astnode();
        if_astnode(exp_astnode* cond, statement_astnode* then_, statement_astnode* else_);
        void print(int blanks);
};

class while_astnode : public statement_astnode{
    public:
        exp_astnode* cond;
        statement_astnode* body;
        while_astnode();
        while_astnode(exp_astnode* cond, statement_astnode* body);
        void print(int blanks);
};

class for_astnode : public statement_astnode{
    public:
        exp_astnode* init;
        exp_astnode* cond;
        exp_astnode* incr;
        statement_astnode* body;
        for_astnode();
        for_astnode(exp_astnode* init, exp_astnode* cond, exp_astnode* incr, statement_astnode* body);
        void print(int blanks);
};

class op_binary_astnode : public exp_astnode{
    public:
        string op;
        exp_astnode* lhs;
        exp_astnode* rhs;
        op_binary_astnode();
        op_binary_astnode(string op, exp_astnode* lhs, exp_astnode* rhs);
        void print(int blanks);
};

class op_unary_astnode : public exp_astnode{
    public:
        string op;
        exp_astnode* exp;
        op_unary_astnode();
        op_unary_astnode(string op, exp_astnode* exp);
        void print(int blanks);
};

class assignE_astnode : public exp_astnode{
    public:
        exp_astnode* lhs;
        exp_astnode* rhs;
        assignE_astnode();
        assignE_astnode(exp_astnode* lhs, exp_astnode* rhs);
        void print(int blanks);
};

class funcall_astnode : public exp_astnode{
    public:
        identifier_astnode* fname;
        vector<exp_astnode*> args;
        funcall_astnode();
        funcall_astnode(identifier_astnode* fname, vector<exp_astnode*> args);
        funcall_astnode(identifier_astnode* fname);
        void addArg(exp_astnode* arg);
        void print(int blanks);
};

class intconst_astnode : public exp_astnode{
    public:
        int value;
        intconst_astnode();
        intconst_astnode(int value);
        void print(int blanks);
};

class floatconst_astnode : public exp_astnode{
    public:
        float value;
        floatconst_astnode();
        floatconst_astnode(float value);
        void print(int blanks);
};

class stringconst_astnode : public exp_astnode{
    public:
        string value;
        stringconst_astnode();
        stringconst_astnode(string value);
        void print(int blanks);
};

class arrayref_astnode : public ref_astnode{
    public:
        exp_astnode* array;
        exp_astnode* index;
        arrayref_astnode();
        arrayref_astnode(exp_astnode* array, exp_astnode* index);
        void print(int blanks);
};

class member_astnode : public ref_astnode{
    public:
        exp_astnode* exp;
        identifier_astnode* mem;
        member_astnode();
        member_astnode(exp_astnode* exp, identifier_astnode* mem);
        void print(int blanks);
};

class arrow_astnode : public ref_astnode{
    public:
        exp_astnode* exp;
        identifier_astnode* mem;
        arrow_astnode();
        arrow_astnode(exp_astnode* exp, identifier_astnode* mem);
        void print(int blanks);
};

#endif
