module top(
    input logic clk,
    input logic rst,
    input logic sim_en,

    output logic black_win,
    output logic white_win,
    output logic draw,
    output logic sim_done
);

control control(
    .clk(clk),
    .rst(rst),
    .sim_en(sim_en),
    .valid_move(),
    .flood_done(),
    .sim_iteration_done(),
    .game_over(),

    .rand_move_en(),
    .play_stone_en(),
    .flood_en(),
    .swap_turn_en(),
    .update_score_en(),
    .increment_iteration_en(),    
    .sim_done(sim_done),
)

endmodule