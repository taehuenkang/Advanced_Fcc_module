`timescale 1ns / 1ps
module relu
#(
parameter IN_DATA_WIDTH = 32
)
(
input signed [IN_DATA_WIDTH-1:0] din,
output signed [IN_DATA_WIDTH-1:0] dout
);
assign dout = (din[IN_DATA_WIDTH-1] == 0)? din : 0; 
//if the sign bit is high, send zero on the output else send the input
endmodule

