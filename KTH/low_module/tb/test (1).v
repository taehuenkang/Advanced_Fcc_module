`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/12/17 19:50:05
// Design Name: 
// Module Name: test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module test;
reg [7:0] udata_in;
reg [7:0] udata_out;
reg signed [7:0] sdata_in;
reg signed [7:0] sdata_out;
initial begin
udata_in = 250; sdata_in = -120;
#100; udata_out = udata_in; // unsiged basic
#100; sdata_out = sdata_in; // signed basic
#100; sdata_out = udata_in; // unsigned -> singed
#100; udata_out = sdata_in; // signed -> unsinged
#100; udata_in = -120; sdata_in = 250;
#100; udata_out = udata_in; // unsiged basic
#100; sdata_out = sdata_in; // signed basic
#100; sdata_out = udata_in; // unsigned -> singed
#100; udata_out = sdata_in; // signed -> unsinged
end
endmodule
