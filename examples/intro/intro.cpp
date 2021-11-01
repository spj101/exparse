#include <iostream>
#include <gmpxx.h>
#include "exparse.hpp"

int main()
{
    Exparse parser;
    
    parser.symbol_table = {"x"};

    parser.substitution_table =
    {
        {"a",5_mpq/6_mpq},
        {"b",7_mpq/3_mpq},
        {"c",11_mpq/5_mpq}
    };
    
    std::string expression = "a+2*b^3+x*a*b";
 
    std::map<std::vector<long long int>, mpq_class> result = parser.parse_expression(expression);
    
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
