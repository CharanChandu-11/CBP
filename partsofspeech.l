%{
#include <stdio.h>
%}

%%
is|am|are|was|were           { printf("%s → Verb\n", yytext); }
run|walk|eat|go|play         { printf("%s → Action Verb\n", yytext); }
he|she|it|they|we|you|I|i      { printf("%s → Pronoun\n", yytext); }
the|a|an                     { printf("%s → Article\n", yytext); }
and|or|but                   { printf("%s → Conjunction\n", yytext); }
quick|slow|happy|sad         { printf("%s → Adjective\n", yytext); }
quickly|slowly               { printf("%s → Adverb\n", yytext); }
cat|dog|apple|school         { printf("%s → Noun\n", yytext); }

[ \t\n]+                     ;   /* ignore whitespace */

.                            { printf("%s → Unknown\n", yytext); }

%%

int main() {
    printf("Enter sentence:\n");
    yylex();
    return 0;
}

int yywrap() { return 1; }
