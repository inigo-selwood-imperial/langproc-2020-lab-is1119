#include "histogram.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

// Use a hash-table to maintain a word -> count mapping.
// This deliberately uses std::unordered_map rather than std::map,
// because we don't need to enumerate the entries in key order.
// This allows us to use a hash-table, so we get O(1) operations
// rather than O(log n).
typedef std::unordered_map<std::string, unsigned> Histogram;

// Define the instance of the variable that is declared in the histogram.hpp header
TokenValue yylval;

int main() {

    Histogram histogram;
    double sum = 0;

    bool tokens_consumed = false;
    while(tokens_consumed == false) {

        switch(TokenType(yylex())) {

            // No more tokens
            case TokenType::NONE:
                tokens_consumed = true;
                break;

            // We have a number. The value will be in yylval.numberValue
            case TokenType::NUMBER:
                sum += yylval.numberValue;
                break;

            // We have a string. The value is in a string _pointed to_ by
            // yylval.wordValue. Note that the string is allocated by the lexer, but
            // deallocated by us.
            case TokenType::WORD:
                if(yylval.wordValue) {
                    histogram[*yylval.wordValue] += 1;
                    delete yylval.wordValue;
                }
                break;

            // There are only three token types.
            default:
                assert(0);
        }
    }

    // Print out `sum` to std::cout with three decimal digits
    std::cout << std::fixed << std::setprecision(3) << sum << '\n';

    // Build a vector of (word, count) entries based on the hash-table
    std::vector<std::pair<std::string, unsigned>>
            sorted(histogram.begin(), histogram.end());

    // Create a custom sort order which orders by count first, then by string
    auto sort_order=[](const std::pair<std::string, unsigned> &a,
            const std::pair<std::string, unsigned> &b) {

        if(a.second > b.second)
            return true;
        if(a.second < b.second)
            return false;
        return a.first < b.first;
    };

    // Sort the array using our custom sort order
    std::sort(sorted.begin(), sorted.end(), sort_order);

    for(const auto &element : sorted)
        std::cout << '[' << element.first << "] " << element.second << '\n';

    return 0;
}
