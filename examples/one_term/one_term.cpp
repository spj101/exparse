#include <iostream>
#include <gmpxx.h>
#include "exparse.hpp"

int main()
{
    Exparse parser;
    
    parser.symbol_table = {};

    parser.substitution_table =
    {
        {"a",5_mpq/6_mpq},
        {"b",7_mpq/3_mpq},
        {"c",11_mpq/5_mpq}
    };
    
    std::string expression = "a+2*b^3+a*b";
 
    std::map<std::vector<long long int>, mpq_class> result = parser.parse_expression(expression);

    std::cout << result.begin()->second << std::endl;

    return 0;


}
