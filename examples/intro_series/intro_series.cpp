#include <iostream>
#include <gmpxx.h>
#include "exparse.hpp"

#include "series.hpp"

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
    
    std::string expression = "a+2*b^3+x+2*x+x*2+c^3+2*c^3+a*x+b^2*c^2*x^5+10*x^0";
    
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
        std::cout << ": " << it->second << " (" << it->second.get_d() << ")" << std::endl;
    }

    if(parser.symbol_table.size() == 1)
    {
        // Convert univariate series to secdecutil::Series
        secdecutil::Series<double> sum = secdecutil::Series<double>(0,0,{0.}, false, parser.symbol_table[0]);
        for(auto it = result.cbegin(); it != result.cend(); ++it)
        {
            secdecutil::Series<double> term = secdecutil::Series<double>(it->first[0],it->first[0],{it->second.get_d()},false, parser.symbol_table[0]);
            sum += term;
        }
        std::cout << sum << std::endl;
        std::cout << 1.0/sum << std::endl;
    }

    return 0;
}
