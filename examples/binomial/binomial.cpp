#include <iostream>
#include <string>
#include <fstream> // ifstream

#include <gmpxx.h>

#include "exparse.hpp"

int main()
{
    
    Exparse parser;
    
    parser.substitution_table =
    {
        {"a1",252097800623_mpq/7732459473917_mpq},
        {"a2",790645490059_mpq/13730453361421_mpq},
        {"a3",1344326694119_mpq/19819454169467_mpq},
        {"a4",1906555030411_mpq/25971199844383_mpq},
        {"a5",2474799787573_mpq/32170948946749_mpq},
        {"a6",3047695447799_mpq/38409597323033_mpq}
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
        mpq_class result = parser.parse_expression(expression).begin()->second;
        std::cout << "result:" << result << std::endl;
    }
    
    infile.close();
    
    return 0;
}
