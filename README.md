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
  * A C++11 compatible C++ compiler.
  * [GNU GMP Library](https://gmplib.org/) (tested for version 6.1.2)
  * (Optional, only for running certain examples) [FORM](https://github.com/vermaseren/form) (tested for version 4.2.1)

The exparse library is header only. Simply put the single header file somewhere reachable from your project or directly into your project tree itself then `#include "exparse.hpp"` in your project.

## Usage

Example: (taken from `examples/intro`) Parse the expression `x+2*y^3` for `x=5/6` and `y=7/3`

```cpp
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
```

Compile:
```shell
$ c++ -std=c++11 -O3 -I<path/to/exparse.hpp> intro.cpp -lgmpxx -lgmp -o intro
```

Output:
```shell
1417/54
```

For further examples see the [examples folder](examples).

A single instance of `exparse` should not be used concurrently by multiple threads. For efficiency reasons, an internal buffer which is not threadsafe is used during parsing. However, it is fine to use several instances of `exparse` concurrently.

## API Documentation

### Public Fields

`std::unordered_map<std::string,mpq_class> symbol_table;`

An unordered map between variables (represented by a string) and their values (represented by a rational number). The symbol table is used during expression parsing to replace variables by their value.

### Public Member Functions

`mpq_class parse_expression(std::string& expression)`

Parse the expression (represented by a string) and return a rational number.

## Authors

  * Stephen Jones (@spj101)
