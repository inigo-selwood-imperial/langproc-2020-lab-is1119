%option noyywrap

%{

// Bring in our declarations for token types and the yylval variable
#include "histogram.hpp"

// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

std::string *parse_string(const std::string &yytext);
double parse_fraction(const std::string &yytext);

%}

letters     ([a-z]|[A-Z])+
string      \[[^\]]*\]

integer     (0|([1-9][0-9]*))
decimal     -?{integer}(\.[0-9]*)?
fraction    -?{integer}\/{integer}

word        {letters}|{string}

number      {decimal}|{fraction}

%%

{letters}   { std::cerr << "letters: '" << yytext << "'\n"; yylval.wordValue = new std::string(yytext); return TokenType::WORD; }
{string}    { std::cerr << "string: '" << yytext << "'\n"; yylval.wordValue = parse_string(yytext); return TokenType::WORD; }

{decimal}   { std::cerr << "decimal: '" << yytext << "'\n"; yylval.numberValue = std::stod(yytext); return TokenType::NUMBER; }
{fraction}  { std::cerr << "fraction: '" << yytext << "'\n"; yylval.numberValue = parse_fraction(yytext); return TokenType::NUMBER; }

\n          { std::cerr << "newline\n"; }
.           ;

%%

std::string *parse_string(const std::string &yytext) {
    return new std::string(yytext.substr(1, yytext.length() - 2));
}

double parse_fraction(const std::string &yytext) {
    const size_t symbol_index = yytext.find_first_of('/');
    const double numerator = std::stod(yytext.substr(0, symbol_index));
    const double denominator =
            std::stod(yytext.substr(symbol_index + 1, std::string::npos));
    return numerator / denominator;
}

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s) {

    // s is the text that wasn't matched
    /* fprintf(stderr, "Flex Error: %s\n", s); */
    exit(1);
}
