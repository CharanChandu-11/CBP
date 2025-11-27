%{
#include <stdio.h>
#include <string.h>
int yywrap(void) { return 1; }
%}
%%
"<"[a-zA-Z_]+">"      { 
                        /* remove < > */
                        char temp[100];
                        strcpy(temp, yytext+1);
                        temp[strlen(temp)-1] = '\0';
                        printf("%s", temp);
                      }

"::="                 { printf(" : "); }

"|"                   { printf(" | "); }

";"                   { printf(" ;\n"); }

[ \t\n]               { ECHO; }

.                     { ECHO; }

%%

int main() {
    printf("Converted YACC rules:\n\n");
    yylex();
    return 0;
}

Input
<expr> ::= <expr> + <term> | <term> ;
<term> ::= <term> * <factor> | <factor> ;
<factor> ::= ( <expr> ) | id ;
OP:
expr : expr + term | term ;
term : term * factor | factor ;
factor : ( expr ) | id ;
