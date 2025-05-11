`timescale 1ns / 1ps

module tb_relu;

  // Parameters
  parameter IN_DATA_WIDTH = 32;

  // Signals
  reg [IN_DATA_WIDTH-1:0] i_result;
  wire [IN_DATA_WIDTH-1:0] o_result;

  // Instantiate relu module
  relu #(IN_DATA_WIDTH) uut (
    .i_result(i_result),
    .o_result(o_result)
  );

  // Initial block for test stimulus
  initial begin
    // Test case 1: Positive input
    i_result = 32'b01000001;
    #10; // Wait for 10 time units
    // Expected output: dout should be equal to din (no change)
    $display("Test case 1 - i_result: %b, o_result: %b", i_result, o_result);

    // Test case 2: Negative input
    i_result= 32'b11010101010101010101010101010101;
    #10; 
    // Expected output: dout should be 32'b00000000
    $display("Test case 2 - i_result: %b, o_result: %b", i_result, o_result);

    // Test case 3: Zero input
    i_result = 32'b00000000;
    #10; // Wait for 10 time units
    // Expected output: dout should be equal to din (no change)
    $display("Test case 3 - i_result: %b, o_result: %b", i_result, o_result);
    
    // Test case 4: Another positive input
    i_result = 32'b01001001;
    #10; // Wait for 10 time units
    // Expected output: dout should be equal to din (no change)
    $display("Test case 4 - i_result: %b, o_result: %b", i_result, o_result);

    $finish; // Finish simulation
  end

endmodule