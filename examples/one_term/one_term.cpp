#include <iostream>
#include "exparse.hpp"

int main()
{
    Exparse parser;
    
    parser.symbol_table = {};

    parser.substitution_table =
    {
        {"a","5/6"},
        {"b","7/3"},
        {"c","11/5"}
    };
    
    std::string expression = "a+2*b^3+a*b";
 
    std::map<std::vector<long long int>, mpqc_class> result = parser.parse_expression(expression);

    std::cout << result.begin()->second << std::endl;

    return 0;


}
