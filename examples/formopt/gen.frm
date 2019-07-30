On wtimestats;

S a1,...,a6;

L test = (a1+a2+a3+a4+a5+a6)^20;
Format O4,saIter=5; * use 5 iterations for optimization
.sort

#optimize test
#Message "Optimized with Horner scheme: `optimscheme_'"
#Message "Number of operations in output: `optimvalue_'"

#write <expr.out> "%O"
#write <expr.out> "EXPR=%e",test
#clearoptimize
.end
