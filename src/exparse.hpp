#ifndef EXPARSE_H
#define EXPARSE_H

#include <string> // string
#include <unordered_map> // unordered_map
#include <cassert> // assert
#include <memory> // shared_ptr, make_shared
#include <cstddef> // size_t
#include <cstring> // strcmp
#include <vector> // vector
#include <map> // map
//#include <cstdlib> // atof

#include <gmpxx.h>

class Exparse
{
private:
    
    typedef mpq_class rational_t;
    typedef long long int int_t;
    
    enum Operation { add, subtract, multiply, divide, inverse};
    
    const rational_t rational_minus_one  = -1;
    const rational_t rational_zero = 0;
    const rational_t rational_one = 1;
    const rational_t rational_two = 2;
    const int_t integer_zero = 0;
    const int_t integer_one = 1;
    const int_t integer_two = 2;
    
    rational_t term_buffer;
    rational_t symbol_buffer;
    rational_t number_buffer;
    rational_t pow_buffer;
    
    int_t int_buffer;

    std::vector<int_t> symbol_orders_buffer;
    std::size_t symbol_order_buffer;
    bool symbol_order_altered = false;
    
    struct slice_t
    {
        std::shared_ptr<std::string> expression;
        std::size_t pos;
        std::size_t len;
        
        const char& operator[](const std::size_t index) const { return (*expression)[pos+index]; }
        
        friend inline bool operator==(const slice_t& lhs, const std::string& rhs)
        {
            if (lhs.len != rhs.length())
            return false;
            for (std::size_t i=0; i<lhs.len; ++i)
            {
                if( lhs[i] != rhs[i] )
                return false;
            }
            return true;
        }
        friend inline bool operator!=(const slice_t& lhs, const std::string& rhs){ return !(lhs == rhs); }
        friend std::ostream& operator<<(std::ostream& os, const slice_t& slice) { os << slice.expression->substr(slice.pos,slice.len); return os; }
    };
    
    void exparse_apply(rational_t& result, const rational_t& symbol, const Operation op)
    {
        if(symbol_order_altered)
        {
            symbol_order_altered = false;
            symbol_orders_buffer[symbol_order_buffer] += 1; // exparse_apply is called when no power is set
            return;
        }
        switch(op)
        {
            case add: result *= symbol; break;
            case subtract: result *= -symbol; break;
            case multiply: result *= symbol; break;
            case divide: result /= symbol; break;
            case inverse: result = symbol/result; break;
        }
    }
    
    void exparse_pow(rational_t& base, const int_t& exponent, rational_t& result, Operation op)
    {
        if(symbol_order_altered)
        {
            symbol_order_altered = false;
            symbol_orders_buffer[symbol_order_buffer] += exponent;
            return;
        }

        if (exponent < integer_zero)
        {
            exparse_apply(result, rational_one, inverse); // 1/result
            exparse_pow(base, -exponent, result, op);
            exparse_apply(result, rational_one, inverse); // 1/result
            return;
        }
        else if (exponent == integer_zero)
        {
            return; // nothing to do
        }
        else if (exponent == integer_one)
        {
            exparse_apply(result, base, op);
            return;
        }
        else if (exponent == integer_two)
        {
            exparse_apply(result, base*base, op);
            return;
        }
        
        // Set power buffer to one (all symbols will be multiplied on to power buffer)
        pow_buffer = rational_one;
        
        int_t tmp_exponent = exponent;
        while(tmp_exponent > 0)
        {
            if(tmp_exponent & 1) // exponent is odd
            pow_buffer *= base;
            base *= base;
            tmp_exponent = tmp_exponent >> 1;
        }
        exparse_apply(result, pow_buffer, op);
    }
    
    void to_number(slice_t& symbol, rational_t& result)
    {
        char store;
        if ( symbol.pos+symbol.len != symbol.expression->length() )
        {
            // Store character after symbol and replace with '\0'
            store =(*symbol.expression)[symbol.pos+symbol.len];
            (*symbol.expression)[symbol.pos+symbol.len] = '\0';
        }
        
        // Note: would be nicer to use find() but does not work correctly for pointer types
        bool found = false;
        // Check if symbol is in substitution table
        for( const std::pair<std::string,rational_t> substitution_table_element: substitution_table)
        {
            if ( strcmp(substitution_table_element.first.c_str(), symbol.expression->c_str()+symbol.pos) == 0)
            {
                result = substitution_table_element.second;
                found = true;
                break;
            }
        }
        // Check if symbol is in symbol_table
        if(!found)
        {
            for( std::size_t i = 0; i < symbol_table.size(); i++)
            {
                if ( strcmp(symbol_table[i].c_str(), symbol.expression->c_str()+symbol.pos) == 0)
                {
                    symbol_order_altered = true;
                    symbol_order_buffer = i;
                    found = true;
                    break;
                }
            }
        }

        if(!found)
        {
            // Parse symbol as rational
            result = rational_t(symbol.expression->c_str()+symbol.pos);
        }

        if ( symbol.pos+symbol.len != symbol.expression->length() )
        {
            // Restore character after symbol
            (*symbol.expression)[symbol.pos+symbol.len] = store;
        }
    }
    
    void to_int(slice_t& symbol, int_t& result)
    {
        if ( symbol.pos+symbol.len != symbol.expression->length() )
        {
            const char store =(*symbol.expression)[symbol.pos+symbol.len];
            (*symbol.expression)[symbol.pos+symbol.len] = '\0';
            result = std::atoll(symbol.expression->c_str()+symbol.pos);
            (*symbol.expression)[symbol.pos+symbol.len] = store;
        }
        else
        {
            result = std::atoll(symbol.expression->c_str()+symbol.pos);
        }
    }
    
    void parse_symbol(slice_t& symbol, rational_t& result)
    {
        // std::size_t symbol_pos = symbol.pos;
        // std::size_t symbol_len = symbol.len;

        // Parse operator
        Operation op = add;
        if( symbol[0] == '+')
        {
            symbol.pos++;
            symbol.len--;
            op = add;
        }
        else if( symbol[0] == '-')
        {
            symbol.pos++;
            symbol.len--;
            op = subtract;
        }
        else if( symbol[0] == '*')
        {
            symbol.pos++;
            symbol.len--;
            op = multiply;
        }
        else if( symbol[0] == '/')
        {
            symbol.pos++;
            symbol.len--;
            op = divide;
        }
        
        // Parse power
        for (std::size_t i=0; i<symbol.len; ++i)
        {
            if( symbol[i] == '^')
            {
                slice_t base_slice = {symbol.expression,symbol.pos,i};
                slice_t exponent_slice = {symbol.expression,symbol.pos+i+1,symbol.len-(i+1)};
                
                to_number(base_slice,number_buffer);
                to_int(exponent_slice,int_buffer);
                exparse_pow(number_buffer, int_buffer, result, op);
                
                return;
            }
        }
        
        // Parse non-power
        to_number(symbol,number_buffer);
        exparse_apply(result,number_buffer,op);
        return;
    }
    
    void parse_term(slice_t& term)
    {
        std::size_t term_pos = term.pos;
        std::size_t term_len = term.len;
        
        // Set term to one (all symbols will be multiplied on to term)
        term_buffer = rational_one;

        // Reset symbol_orders_buffer
        symbol_orders_buffer.assign(symbol_table.size() ,0);
        
        // Parse term
        std::size_t reading_point = 0;
        for (std::size_t i=0; i<term.len; ++i)
        {
            if( term[i] == '*' || term[i] == '/')
            {
                // Parse term
                term.pos = term_pos+ reading_point;
                term.len = i-reading_point;
                parse_symbol(term, term_buffer);
                
                // Update reading point
                reading_point = i;
                
                // Reset term to original parameters
                term.pos = term_pos;
                term.len = term_len;
            }
            else if ( i == term.len-1 )
            {
                // Parse term
                term.pos = term_pos+ reading_point;
                term.len = i+1-reading_point;
                parse_symbol(term, term_buffer);
                
                // Update reading point
                reading_point = i+1;
                
                // Reset term to original parameters
                term.pos = term_pos;
                term.len = term_len;
            }
        }
    }

    void add_term(std::map<std::vector<int_t>,rational_t>& result)
    {
        std::map<std::vector<int_t>,rational_t>::iterator lb = result.lower_bound(symbol_orders_buffer);
        if(lb != result.end() && !result.key_comp()(symbol_orders_buffer, lb->first))
        {
            // symbol_orders exists in result, add term to result
            lb->second += term_buffer;
        }
        else
        {
            // symbol_orders does not exist in result, set term equal to term
            result.insert(lb, std::map<std::vector<int_t>,rational_t>::value_type(symbol_orders_buffer, term_buffer));
        }

        // Reset buffers
    }

    void parse_line(slice_t& line, std::map<std::vector<int_t>,rational_t>& result)
    {
        std::size_t line_pos = line.pos;
        std::size_t line_len = line.len;

        symbol_orders_buffer.assign(symbol_table.size(), 0); // initialise symbol_orders_buffer

        // Parse line
        std::size_t reading_point = 0;
        for (std::size_t i=0; i<line.len; ++i)
        {
            if( i != 0 && (line[i] == '+' || (line[i] == '-' && line[i-1] != '^') ))
            {
                // Parse term
                line.pos = line_pos + reading_point;
                line.len = i-reading_point;

                parse_term(line);
                add_term(result);

                // Update reading point
                reading_point = i;
                
                // Reset line to original parameters
                line.pos = line_pos;
                line.len = line_len;
            }
            else if (i == line.len-1)
            {
                // Parse term
                line.pos = line_pos + reading_point;
                line.len = i+1-reading_point;
                
                parse_term(line);
                add_term(result);

                // Update reading point
                reading_point = i+1;
                
                // Reset line to original parameters
                line.pos = line_pos;
                line.len = line_len;
            }
        }
    }
    
    void parse_sanity(const slice_t& line)
    {
        for (std::size_t i=0; i<line.expression->length(); ++i)
        {
            assert(line[i]!='(');
            assert(line[i]!=')');
            assert(line[i]!=' ');
            assert(line[i]!=';');
        }
        assert(line[line.expression->length()-1] != '+');
        assert(line[line.expression->length()-1] != '-');
        assert(line[line.expression->length()-1] != '*');
        assert(line[line.expression->length()-1] != '^');
    }
    
public:

    std::vector<std::string> symbol_table;
    std::unordered_map<std::string,rational_t> substitution_table;

    std::map<std::vector<int_t>, rational_t> parse_expression(const std::string& expression)
    {
        std::map<std::vector<int_t>, rational_t> result;
        
        slice_t line;
        line.expression = std::make_shared<std::string>(expression);
        line.pos = 0;
        line.len = line.expression->length();
        
        parse_line(line, result);
        
        return result;
    }
    
    // Constructor
    Exparse()
    {
        // Allow buffer sizes to be set?
        
        // Rational buffers
        term_buffer = rational_zero;
        symbol_buffer = rational_zero;
        number_buffer = rational_zero;
        pow_buffer = rational_zero;
        
        // Integer buffers
        int_buffer = integer_one;
    }
    
};

#endif

