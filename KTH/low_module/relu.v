`timescale 1ns / 1ps
module relu
#(
parameter IN_DATA_WITDH = 32
)
(
input [IN_DATA_WITDH-1:0] i_result,
output [IN_DATA_WITDH-1:0] o_result
);
assign o_result = (i_result[IN_DATA_WITDH-1] == 0) ? i_result : 0;

//if the sign bit is high, send zero on the output else send the input
endmodule