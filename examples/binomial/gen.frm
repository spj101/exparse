On wtimestats;

S a1,...,a6;

L test = (a1+a2+a3+a4+a5+a6)^20;
.sort

#write <expr.out> "%E",test
#write <expr.frm> "L test = %e",test
.end
