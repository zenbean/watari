// 9 bit lfsr generating random moves and validating them
module gen_random_move(
    input logic clk,
    input logic rst,
    input logic en,

    output [8:0] rand_move
);

logic [8:0] shift_reg;

always_ff @(posedge clk) begin
    if (rst == 1'b1) begin
        shift_reg <= 9'd5;
    end else begin
        shift_reg <= shift_reg;
        if(en==1'b1) begin
            shift_reg <= {shift_reg[7:0],(shift_reg[8]^shift_reg[4]^shift_reg[3]^shift_reg[2]^1)};
        end
    end
end

assign rand_move = shift_reg;

endmodule