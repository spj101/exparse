#include "catch.hpp"
#include "exparse.hpp"

#include <gmpxx.h>

TEST_CASE( "Operations" , "[exparse]" )
{
    typedef mpq_class rational_t;
    
    std::string n = "5/7";
    std::string pn = "+5/7";
    std::string mn = "-5/7";
    std::string n1 = "5^1/7^1";
    std::string nsq = "5^2/7^2";
    std::string nmsq = "5^-2/7^-2";
    std::string mnsq = "-5^2/7^2";
    std::string mnmsq = "-5^-2/7^-2";
    
    std::string x = "x";
    std::string px = "+x";
    std::string mx = "-x";
    std::string x1 = "x^1";
    std::string xsq = "x^2";
    std::string xmsq = "x^-2";
    std::string mxsq = "-x^2";
    std::string mxmsq = "-x^-2";
    
    std::string npn = "3/2+5/7";
    std::string nmn = "3/2-5/7";
    std::string ntn = "3/2*5/7";
    // std::string ndn = "3/2/5/7"; // forbidden
    std::string pnpn = "+3/2+5/7";
    std::string mnpn = "-3/2+5/7";
    std::string inmn = "-3^-1/2^-1-5/7";
    
    std::string xpy = "x+y";
    std::string xmy = "x-y";
    std::string xty = "x*y";
    std::string xdy = "x/y";
    std::string pxpy = "+x+y";
    std::string mxpy = "-x+y";
    std::string ixmy = "-x^-1-y";
    
    SECTION( "Unary operations (number)" )
    {
        Exparse parser;
        
        REQUIRE( parser.parse_expression(n) == 5_mpq/7_mpq );
        REQUIRE( parser.parse_expression(pn) == 5_mpq/7_mpq );
        REQUIRE( parser.parse_expression(mn) == -5_mpq/7_mpq );
        REQUIRE( parser.parse_expression(n1) == 5_mpq/7_mpq );
        REQUIRE( parser.parse_expression(nsq) == 25_mpq/49_mpq );
        REQUIRE( parser.parse_expression(nmsq) == 49_mpq/25_mpq );
        REQUIRE( parser.parse_expression(mnsq) == -25_mpq/49_mpq );
        REQUIRE( parser.parse_expression(mnmsq) == -49_mpq/25_mpq );
        
    };
    
    SECTION( "Unary operations (symbol)" )
    {
        Exparse parser;
        parser.symbol_table =
        {
            {"x",3_mpq/2_mpq}
        };
        
        REQUIRE( parser.parse_expression(x) == 3_mpq/2_mpq );
        REQUIRE( parser.parse_expression(px) == 3_mpq/2_mpq );
        REQUIRE( parser.parse_expression(mx) == -3_mpq/2_mpq );
        REQUIRE( parser.parse_expression(x1) == 3_mpq/2_mpq );
        REQUIRE( parser.parse_expression(xsq) == 9_mpq/4_mpq );
        REQUIRE( parser.parse_expression(xmsq) == 4_mpq/9_mpq );
        REQUIRE( parser.parse_expression(mxsq) == -9_mpq/4_mpq );
        REQUIRE( parser.parse_expression(mxmsq) == -4_mpq/9_mpq );
        
    };
    
    SECTION( "Binary operations (number)" )
    {
        Exparse parser;
        
        REQUIRE( parser.parse_expression(npn) == 31_mpq/14_mpq );
        REQUIRE( parser.parse_expression(nmn) == 11_mpq/14_mpq );
        REQUIRE( parser.parse_expression(ntn) == 15_mpq/14_mpq );
        REQUIRE( parser.parse_expression(pnpn) == 31_mpq/14_mpq );
        REQUIRE( parser.parse_expression(mnpn) == -11_mpq/14_mpq );
        REQUIRE( parser.parse_expression(inmn) == -29_mpq/21_mpq );
        
    };
    
    SECTION( "Binary operations (symbol)" )
    {
        Exparse parser;
        parser.symbol_table =
        {
            {"x",3_mpq/2_mpq},
            {"y",5_mpq/7_mpq}
        };
        
        REQUIRE( parser.parse_expression(xpy) == 31_mpq/14_mpq );
        REQUIRE( parser.parse_expression(xmy) == 11_mpq/14_mpq );
        REQUIRE( parser.parse_expression(xty) == 15_mpq/14_mpq );
        REQUIRE( parser.parse_expression(xdy) == 21_mpq/10_mpq );
        REQUIRE( parser.parse_expression(pxpy) == 31_mpq/14_mpq );
        REQUIRE( parser.parse_expression(mxpy) == -11_mpq/14_mpq );
        REQUIRE( parser.parse_expression(ixmy) == -29_mpq/21_mpq );

        
    };
    
};

TEST_CASE( "Large Numbers" , "[exparse]" )
{
    typedef mpq_class rational_t;
    
    std::string n = "71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699";
    std::string pn = "+71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699";
    std::string mn = "-71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699";
    std::string nsq = "71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473^50/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699^50";
    std::string nmsq = "71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473^-50/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699^-50";
    std::string mnsq = "-71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473^50/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699^50";
    std::string mnmsq = "-71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473^-50/193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699^-50";
    
    SECTION( "Unary operations (number)" )
    {
        Exparse parser;
        
        mpq_class num = 71000043264436482494390107093282688489573565181808197812372585399504446035316648916494607917902486755089085920784044945470018034238513473_mpq;
        mpq_class den = 193009412417169166913227997343359008248674987410944756537819741916377891027577630518186101344938906882203617750986183368800846654482519887699_mpq;
        
        mpq_class num10 = num*num*num*num*num*num*num*num*num*num;
        mpq_class den10 = den*den*den*den*den*den*den*den*den*den;
        
        mpq_class num50 = num10*num10*num10*num10*num10;
        mpq_class den50 = den10*den10*den10*den10*den10;
        
        REQUIRE( parser.parse_expression(n) == num/den );
        REQUIRE( parser.parse_expression(pn) == num/den );
        REQUIRE( parser.parse_expression(mn) == -num/den );
        REQUIRE( parser.parse_expression(nsq) == num50/den50 );
        REQUIRE( parser.parse_expression(nmsq) == den50/num50 );
        REQUIRE( parser.parse_expression(mnsq) == -num50/den50 );
        REQUIRE( parser.parse_expression(mnmsq) == -den50/num50 );
        
    };
    
};
