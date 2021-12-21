#include <iostream>
#include "exparse.hpp"
#include "series.hpp"

typedef std::vector<long long int> powerlist_t;
typedef mpqc_class rational_t;
typedef std::map<powerlist_t, rational_t> expression_t;

//template<typename T> using nested_series_t = secdecutil::Series<T>; // 1 variable
template<typename T> using nested_series_t = secdecutil::Series<secdecutil::Series<T>>; // 2 variable (etc...)

template<typename T>
struct ex_to_nested_series
{
    static T convert
    (
        expression_t expression,
        const std::vector<std::string>& expansion_parameters,
        const std::vector<int>& numbers_of_orders,
        const std::vector<int>& offsets = {},
        size_t current_regulator_index = 0
    )
    {
        assert(current_regulator_index == numbers_of_orders.size());
        assert(current_regulator_index == expansion_parameters.size());
        return expression.begin()->second;
    }
};

template<typename T>
struct ex_to_nested_series<secdecutil::Series<T>>
{
    static secdecutil::Series<T> convert
    (
        expression_t expression,
        const std::vector<std::string>& expansion_parameters,
        const std::vector<int>& numbers_of_orders,
        const std::vector<int>& offsets = {},
        size_t current_regulator_index = 0
    )
    {
        int offset = offsets.empty() ? 0 : offsets.at(current_regulator_index);
        std::vector<T> content;
        content.reserve(numbers_of_orders.at(current_regulator_index));
        for (int i = 0; i < numbers_of_orders.at(current_regulator_index); ++i)
        {
            // Get terms of order i in regulator current_regulator_index (dropping current regulator)
            powerlist_t lower_bound( expression.begin()->first.size(), 0);
            powerlist_t upper_bound( expression.begin()->first.size(), 0);
            lower_bound[0] = i;
            upper_bound[0] = i+1;
            expression_t::iterator itlow = expression.lower_bound(lower_bound);
            expression_t::iterator itup = expression.lower_bound(upper_bound);
            expression_t subexpression;
            for (auto it=itlow; it!=itup; ++it)
            {
                powerlist_t powerlist = powerlist_t(it->first.begin()+1, it->first.end()); // drop current regulator
                subexpression[powerlist] = it->second;
            }
            if(subexpression.empty())
                subexpression[powerlist_t( expression.begin()->first.size() - 1, 0)] = "0";
            content.push_back
            (
                ex_to_nested_series<T>::convert
                (
                    subexpression,
                    expansion_parameters, numbers_of_orders, offsets,
                    current_regulator_index + 1
                )
            );
        }
        return {
                    offset,
                    numbers_of_orders.at(current_regulator_index)+offset-1,
                    content, true, expansion_parameters.at(current_regulator_index)
                };
    }
};

int main()
{
    Exparse parser;
    
    parser.symbol_table = {"x","y"};

    parser.substitution_table =
    {
        {"a","5/6"},
        {"b","7/3"},
        {"c","11/5"}
    };
    
    std::string expression = "a+2*b^3+x+2*x+x*2+c^3+2*c^3+a*x+b^2*c^2*x^5+10*x^0+y+15*y^2";
    
    expression_t result = parser.parse_expression(expression);
    
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

    nested_series_t<rational_t> result_series =
        ex_to_nested_series<nested_series_t<rational_t>>::convert
        (
            result,
            parser.symbol_table,
            {7,3}
        );

    std::cout << result_series << std::endl;

    return 0;
}
