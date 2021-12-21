#include "catch.hpp"
#include "exparse.hpp"

TEST_CASE( "Operations" , "[mpqc]" )
{
    mpqc_class one_char("1");
    mpqc_class hq_char("1/2","1/4");
    mpqc_class a_mpq("2","3");
    mpqc_class b_mpq("4","5");
    mpqc_class a_char("2","3");
    mpqc_class c_double(123.,456.);

    SECTION( "Comparator Operators" )
    {
        REQUIRE( a_char == mpqc_class("2","3") );
        REQUIRE( a_char != mpqc_class("4","5") );
    };
    
    SECTION( "Constructors" )
    {
        REQUIRE( mpqc_class() == mpqc_class("0","0") ); // no args => (0,0)
        REQUIRE( one_char == mpqc_class("1","0") );
        REQUIRE( hq_char == mpqc_class("1/2","1/4") );
        REQUIRE( mpq_get_d(c_double.re) == Approx(123.));
        REQUIRE( mpq_get_d(c_double.im) == Approx(456.));
    };
    
    SECTION( "Unary Operators" )
    {
        REQUIRE( -a_mpq == mpqc_class("-2","-3") );
        REQUIRE( +a_mpq == mpqc_class("2","3") );
    };
    
    SECTION( "Assignment Operators [+]" )
    {
        a_mpq += b_mpq;
        REQUIRE( a_mpq == mpqc_class("6","8") );
    };
    
    SECTION( "Assignment Operators [-]" )
    {
        a_mpq -= b_mpq;
        REQUIRE( a_mpq == mpqc_class("-2","-2") );
    };
    
    SECTION( "Assignment Operators [*]" )
    {
        a_mpq *= b_mpq;
        REQUIRE( a_mpq == mpqc_class("-7","22") );
    };
    
    SECTION( "Assignment Operators [/]" )
    {
        a_mpq /= b_mpq;
        REQUIRE( a_mpq == mpqc_class("23/41","2/41") );
    };
    
    SECTION( "Binary Operators (2)" )
    {
        REQUIRE( a_mpq + b_mpq == mpqc_class("6","8") );
        REQUIRE( a_mpq - b_mpq == mpqc_class("-2","-2") );
        REQUIRE( a_mpq * b_mpq == mpqc_class("-7","22") );
        REQUIRE( a_mpq / b_mpq == mpqc_class("23/41","2/41") );
    };
};
