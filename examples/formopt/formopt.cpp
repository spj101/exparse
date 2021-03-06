#include <iostream>
#include <string>
#include <fstream> // ifstream

#include <gmpxx.h>

#include "exparse.hpp"

int main()
{
    
    Exparse parser;
    
    parser.symbol_table =
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
    
    std::string line;
    while (std::getline(infile, line))
    {
        std::size_t delimiter_pos = line.find("=");
        std::string name = line.substr(0, delimiter_pos);
        std::string expression = line.substr(delimiter_pos+1, line.length()-delimiter_pos-2); // Last character must be ;
        mpq_class result = parser.parse_expression(expression);
        parser.symbol_table[name] = result;
        std::cout << name << " = " << result << std::endl;
    }
    
    infile.close();
    
    return 0;
}
