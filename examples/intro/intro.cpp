#include <iostream>
#include <gmpxx.h>
#include "exparse.hpp"

int main()
{
    Exparse parser;

    parser.symbol_table =
    {
        {"x",5_mpq/6_mpq},
        {"y",7_mpq/3_mpq}
    };
    
    std::string expression = "x+2*y^3";
    
    mpq_class result = parser.parse_expression(expression);
    
    std::cout << result << std::endl;
    
    return 0;
}
