#include <iostream>
#include "exparse.hpp"

int main()
{
    Exparse parser;
    
    parser.symbol_table = {"x"};

    parser.substitution_table =
    {
        {"a",mpqc_class("5/6","0")},
        {"b",mpqc_class("7/3","0")},
        {"c",mpqc_class("0","11/5")}
    };
    
    std::string expression = "a+2*b^3+x*a*b+c";
 
    std::map<std::vector<long long int>, mpqc_class> result = parser.parse_expression(expression);
    
    // Print symbols in order declared
    for(auto symbol : parser.symbol_table)
        std::cout << symbol << " ";
    std::cout << std::endl;
    
    // Print exponent vectors : coefficient
    for(auto it = result.cbegin(); it != result.cend(); ++it)
    {
        for(auto elem : it->first)
            std::cout << elem << " ";
        std::cout << ": " << it->second << std::endl;
    }

    return 0;
}
