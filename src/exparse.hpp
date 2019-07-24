#ifndef EXPARSE_H
#define EXPARSE_H

#include <fstream> // ifstream
#include <sstream> // sstream
#include <string> // string
#include <vector> // vector
#include <algorithm> // remove_if
#include <cassert> // assert
#include <cstdlib> // atof
#include <cstddef> // size_t
#include <memory> // shared_ptr, make_shared
#include <chrono> // chrono

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
        for (const std::pair<std::string,rational_t>& possible_symbol : symbol_table)
        {
            if( symbol == possible_symbol.first )
            {
                result = possible_symbol.second;
                return;
            }
        }
        
        if ( symbol.pos+symbol.len != symbol.expression->length() )
        {
            const char store =(*symbol.expression)[symbol.pos+symbol.len];
            (*symbol.expression)[symbol.pos+symbol.len] = '\0';
            result = rational_t(symbol.expression->c_str()+symbol.pos);
            //        result = std::atof(symbol.expression->c_str()+symbol.pos);
            (*symbol.expression)[symbol.pos+symbol.len] = store;
        }
        else
        {
            result = rational_t(symbol.expression->c_str()+symbol.pos);
            //        result = std::atof(symbol.expression->c_str()+symbol.pos);
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
    
    void parse_term(slice_t& term, rational_t& result)
    {
        std::size_t term_pos = term.pos;
        std::size_t term_len = term.len;
        
        // Set term to one (all symbols will be multiplied on to term)
        result = rational_one;
        
        // Parse term
        std::size_t reading_point = 0;
        for (std::size_t i=0; i<term.len; ++i)
        {
            if( term[i] == '*' || term[i] == '/')
            {
                // Parse term
                term.pos = term_pos+ reading_point;
                term.len = i-reading_point;
                parse_symbol(term, result);
                
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
                parse_symbol(term, result);
                
                // Update reading point
                reading_point = i+1;
                
                // Reset term to original parameters
                term.pos = term_pos;
                term.len = term_len;
            }
        }
    }
    
    void parse_line(slice_t& line, rational_t& result)
    {
        std::size_t line_pos = line.pos;
        std::size_t line_len = line.len;
        
        // Set result to zero (all terms will be added to result)
        result = rational_zero;

        // Parse line
        std::size_t reading_point = 0;
        for (std::size_t i=0; i<line.len; ++i)
        {
            if( i != 0 && (line[i] == '+' || (line[i] == '-' && line[i-1] != '^') ))
            {
                // Parse term
                line.pos = line_pos + reading_point;
                line.len = i-reading_point;
                parse_term(line,term_buffer);
                result += term_buffer;
                
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
                
                parse_term(line,term_buffer);
                result += term_buffer;
                
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

    std::vector<std::pair<std::string,rational_t>> symbol_table;
    
    rational_t parse_expression(std::string& expression)
    {
        rational_t result = rational_zero;
        
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

