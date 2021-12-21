[![Build Status](https://travis-ci.org/spj101/exparse.svg?branch=master)](https://travis-ci.org/spj101/exparse)
[![Coverage Status](https://coveralls.io/repos/github/spj101/exparse/badge.svg?branch=master)](https://coveralls.io/github/spj101/exparse?branch=master)

# exparse

A polynomial to rational number parser.  

Supports:
  * operations `+ - * / ^`  
  * integer powers only
  * brackets are not supported

## Installation

Prerequisites:
  * A C++17 compatible C++ compiler.
  * [GNU GMP Library](https://gmplib.org/) (tested for version 6.1.2)
  * (Optional, only for running certain examples) [FORM](https://github.com/vermaseren/form) (tested for version 4.2.1)

The exparse library is header only. Simply put the single header file somewhere reachable from your project or directly into your project tree itself then `#include "exparse.hpp"` in your project.

## Usage

Example: (taken from `examples/intro`) Parse the expression `x+2*y^3` for `x=5/6` and `y=7/3`

```cpp
#include <iostream>
#include "exparse.hpp"

int main()
{
    Exparse parser;

    parser.symbol_table = {"x"};

    parser.substitution_table =
    {
        {"a","5/6"},
        {"b","7/3"},
        {"c","11/5"}
    };

    std::string expression = "a+2*b^3+x*a*b";

    std::map<std::vector<long long int>, mpqc_class> result = parser.parse_expression(expression);

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

    return 0;
}
```

Compile:
```shell
$ c++ -std=c++17 -O3 -I<path/to/exparse.hpp> intro.cpp -lgmp -o intro
```

Output:
```shell
x
0 : (1417/54,0)
1 : (35/18,0)
```

For further examples see the [examples folder](examples).

A single instance of `exparse` should not be used concurrently by multiple threads. For efficiency reasons, an internal buffer which is not threadsafe is used during parsing. However, it is fine to use several instances of `exparse` concurrently.

## API Documentation

### Public Fields

`std::vector<std::string> symbol_table;`

A vector of the names of each series variable. The code will output the coefficients of each term in the series.

`std::unordered_map<std::string,mpqc_class> substitution_table;`

An unordered map between variables (represented by a string) and their values (represented by a rational number). The substitution table is used during expression parsing to replace variables by their value.

### Public Member Functions

`std::map<std::vector<int_t>, rational_t> parse_expression(const std::string& expression)`

Parse the expression (represented by a string) and return a map of symbol powers to rational numbers. Each element of the map represents a term in the expression, the symbol powers represent the power of each symbol in the `symbol_table` and the rational numbers represent their coefficient.

## Authors

  * Stephen Jones (@spj101)
