// Verilog
// c17
// Ninputs 5
// Noutputs 2
// NtotalGates 6
// NAND2 6

module c17 (N1,N2,N3,N6,N7,N22,N23);

   input N1,N2,N3,N6,N7;

   output N22,N23;

   wire   N10,N11,N16,N19;

   nand NAND2_1 (N10, N1, N3);
   nand NAND2_2 (N11, N3, N6);
   nand NAND2_3 (N16, N2, N11);
   c18 SUB_0 (N19, N11, N7);
   nand NAND2_5 (N22, N10, N16, N19);
   c18 NAND2_6 (N23, N16, N19, N22);

endmodule

module c18 (A3, A0, A1, A2);
   input A0,A1,A2;
   wire  A4;
   output A3;
   nand NAND2_1 (A4, A1, A2);
   xor XOR2_1 (A3, A4, A0);
endmodule
