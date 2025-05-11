`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/12/17 18:11:54
// Design Name: 
// Module Name: tb_mul
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
module tb_mul;
reg signed [7:0] r_node;
reg signed [7:0] r_wegt;
reg signed [7:0] r_bias;
wire signed [15:0] w_mul;
mul DUT(.node(r_node),
.wegt(r_wegt),
.bias(r_bias),
.mul(w_mul));
initial begin
r_node = 0; r_wegt = 0; r_bias = 0;
#100; r_node = -50; r_wegt = -50;
#100; r_node = 50; r_wegt = -50;
#100; r_node = -50; r_wegt = 50;
#100; r_node = 50; r_wegt = 50;r_bias = -101;
#100;
$finish;
end
endmodule
