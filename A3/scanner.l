%{
// This part goes directly into the generated parser

#include "scanner.hh"
#include "parser.tab.hh"

// YY_DECL is bound to the type of the scanner. The default is 
// #define YY_DECL int Scanner::yylex()
// which is generated anyway.

//If you want the type of the generated scanner to be different, do this

#undef  YY_DECL
#define YY_DECL int IPL::Scanner::yylex( IPL::Parser::semantic_type * const lval, IPL::Parser::location_type *location )

//We want these to be the first action after a token is recognized. 

#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
#define token_type IPL::Parser::token

// token_type is a short hand for IPL::Parser::token

// using token_type = IPL::Parser::token;

%}


/* %option debug */
%option nodefault
%option yyclass="Scanner"
%option noyywrap
%option c++

single_line_comment [/][/][.]*
multi_line_comment [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]
digit [0-9]
digits {digit}+
int_constant   [1-9][0-9]*|0[bB][01]*|0[0-7]*|0[xX][0-9a-fA-F]*
float_constant ({digits}(\.{digit}*)?([eE][+-]?{digits})?)|((\.{digits})([eE][+-]?{digits})?)
identifier  [a-zA-Z_][a-zA-Z0-9_]* 
string_literal  \"(([^\"]|((\\\")|(\\\n)|(\\\t)))*)\"


%%

%{ /** Code executed at the beginning of yylex **/

      yylval = lval;
      loc = location;

%}

[ \t]                                //skip
[\n]                                 {loc->lines();}     
{single_line_comment}                //skip
{multi_line_comment}                 //skip

struct                                    {   
       yylval->emplace< std::string >( YYText() );
       return token_type::STRUCT;
}

void                                      {   
       yylval->emplace< std::string >( YYText() );
       return token_type::VOID;
}

return         {   
       yylval->emplace< std::string >( YYText() );
       return token_type::RETURN;
}

int                                        {   
       yylval->emplace< std::string >( YYText() );
       return token_type::INT;
}

float          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::FLOAT;
}

if          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::IF;
}

else          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::ELSE;
}

while          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::WHILE;
}

for          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::FOR;
}

"=="          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::EQ_OP;
}

"!="          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::NE_OP;
}

"||"         {   
       yylval->emplace< std::string >( YYText() );
       return token_type::OR_OP;
}

"&&"         {   
       yylval->emplace< std::string >( YYText() );
       return token_type::AND_OP;
}

"<="          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::LE_OP;
}

">="          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::GE_OP;
}

"++"          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::INC_OP;
}

"->"          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::PTR_OP;
}

{identifier}          {   
       yylval->emplace< std::string >( YYText() );
       return token_type::IDENTIFIER;
}

{int_constant}                           {   
       yylval->emplace< std::string >( YYText() );
       return token_type::INT_CONST;
}

{float_constant}                         {   
       yylval->emplace< std::string >( YYText() );
       return token_type::FLOAT_CONST;  
}

{string_literal}                   {   
       yylval->emplace< std::string >( YYText() );
       return token_type::STRING_LITERAL;    
} 


"+"                                         return '+';
"-"                                         return '-';
","                                         return ',';
"("                                         return '(';
")"                                         return ')';
"{"                                         return '{';
"}"                                         return '}';
"["                                         return '[';
"]"                                         return ']';
":"                                         return ':';
"="                                         return '=';
"!"                                         return '!'; 
"&"                                         return '&';
"|"                                         return '|';
";"                                         return ';';
"*"                                         return '*';
">"                                         return '>';
"/"                                         return '/';
"<"                                         return '<';
"."                                         return '.';
<<EOF>>         yyterminate();

.                    {   
       yylval->emplace< std::string >( YYText() );
       return token_type::OTHERS;
}


%%

