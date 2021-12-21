#include <iostream>
#include <string>
#include <fstream> // ifstream

#include "exparse.hpp"

int main()
{
    
    Exparse parser;
    
    parser.substitution_table =
    {
        {"a1","252097800623/7732459473917"},
        {"a2","790645490059/13730453361421"},
        {"a3","1344326694119/19819454169467"},
        {"a4","1906555030411/25971199844383"},
        {"a5","2474799787573/32170948946749"},
        {"a6","3047695447799/38409597323033"}
    };

    std::ifstream infile("expr.txt");
    
    if(!infile)
    {
        std::cout << "error opening file" << std::endl;
        return 1;
    }
    
    std::string expression;
    while (std::getline(infile, expression))
    {
        std::cout << "expression.length(): " << expression.length() << std::endl;
        mpqc_class result = parser.parse_expression(expression).begin()->second;
        std::cout << "result:" << result << std::endl;
    }
    
    infile.close();
    
    return 0;
}
