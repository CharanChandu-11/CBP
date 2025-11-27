%{
#include <stdio.h>
#include <stdlib.h>

int result = 0;
int num1 = 0, num2 = 0;
char op = 0;

int yywrap(void);
%}

%%

[0-9]+      {
                if (num1 == 0 && op == 0)
                    num1 = atoi(yytext);
                else
                    num2 = atoi(yytext);
            }

[+\-*/]     { op = yytext[0]; }

[\n]        {
                if (op == 0) {
                    printf("Invalid Expression\n");
                } else {
                    switch (op) {
                        case '+': result = num1 + num2; break;
                        case '-': result = num1 - num2; break;
                        case '*': result = num1 * num2; break;
                        case '/': result = (num2 != 0) ? num1 / num2 : 0; break;
                    }
                    printf("Result = %d\n", result);
                }

                num1 = num2 = result = 0;
                op = 0;
            }

[ \t]       ;   // ignore spaces

.           { printf("Invalid Character: %s\n", yytext); }

%%

/* Proper definition */
int yywrap(void) {
    return 1;
}

int main() {
    printf("Enter arithmetic expressions (ex: 10+20):\n");
    yylex();
    return 0;
}
