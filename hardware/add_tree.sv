module add_tree
#(
    parameter int NUM_INPUTS = 8,
    parameter int INPUT_WIDTH = 361
)(
    input logic clk,
    input logic rst,
    input logic en,
    input logic [INPUT_WIDTH-1:0] inputs[NUM_INPUTS],
    output logic [INPUT_WIDTH+$clog2(NUM_INPUTS)-1:0] sum
);

generate
    if (INPUT_WIDTH < 1) begin
        $fatal(1, "error: INPUT_WIDTH must be positive");
    end
    if (NUM_INPUTS < 1) begin
        $fatal(1, "error: NUM_INPUTS must be positive");
    end else if (NUM_INPUTS == 1) begin
    end
endgenerate

endmodule