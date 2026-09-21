module top(
    input logic clk,
    input logic rst,
    input logic sim_en,
    input logic initial_colour, // 1: black, 0: white
    input logic [1:0] board_size,
    input logic [4:0] komi,
    input logic [15:0] iterations,
    input logic [360:0] black_board,
    input logic [360:0] white_board,
    output logic [15:0] black_win, // 16 bit output for UCT calculation in software
    output logic [15:0] white_win,
    output logic [15:0] drawed,
    output logic sim_done
);

logic [360:0] current_black_board;
logic [360:0] current_white_board;
logic [360:0] active_mask; // for flexible board sizes
logic [15:0] current_iterations;
logic [15:0] black_wins; // per simulation
logic [15:0] white_wins;
logic [15:0] draws;
logic [8:0] black_score; // accumulation
logic [8:0] white_score;
logic [8:0] pass_index;
logic [8:0] ko_point; // done in board_logic.sv
logic [8:0] rand_move;
logic [1:0] pass_counter;
logic active_colour;

wire game_over = (pass_counter == 2'd2);
wire valid_move;
wire flood_done;
wire sim_iteration_done;
wire rand_move_en;
wire play_stone_en;
wire flood_en;
wire swap_en;
wire update_score_en;
wire increment_iteration_en;
// validation flags
wire is_empty = (~current_black_board[rand_move] & ~current_white_board[rand_move]);
wire in_bounds = active_mask[rand_move];
wire not_ko = (rand_move != ko_point);
wire pass_move = (rand_move == pass_index);

assign valid_move = pass_move | (is_empty&in_bounds&not_ko);
assign black_win = black_wins;
assign white_win = white_wins;
assign drawed = draws;
assign sim_iteration_done = (current_iterations==iterations);

// bitmask for flexible board sizes
always_comb begin
    case(board_size)
        2'b00: begin
            active_mask = {280'b0, 81'{1'b1}};
            pass_index = 9'd81;
        end
        2'b01: begin
            active_mask = {192'b0, 169'{1'b1}};
            pass_index = 9'd169;
        end
        2'b10: begin
            active_mask = 361'{1'b1};
            pass_index = 9'd361;
        end
        default: begin
            active_mask = 361'{1'b1};
            pass_index = 9'd361;
        end
    endcase
end

always_ff @(posedge clk) begin
    if (sim_en==1'b0 | rst) begin
        current_black_board <= black_board;
        current_white_board <= white_board;
        active_colour<=initial_colour;
        pass_counter<=2'b0;
        current_iterations<=16'd0;
        black_wins<=16'b0;
        white_wins<=16'b0;
        draws<=16'b0;
    end else begin
        // restart to prep for simulation down a different root
        if(update_score_en) begin
            if (black_score > white_score + komi) begin
                black_wins <= black_wins + 16'b1;
            end else if (white_score + komi > black_score) begin
                white_wins <= white_wins + 16'b1;
            end else begin
                draws <= draws + 16'b1;
            end

            current_black_board <= black_board;
            current_white_board <= white_board;
            active_colour <= initial_colour;
            pass_counter<=2'b0;
        end
        if(play_stone_en) begin
            if (pass_move) begin
                pass_counter <= pass_counter + 2'b1;
            end else begin
                if (active_colour==1'b1) begin
                    current_black_board[rand_move] <= valid_move;
                end else begin
                    current_white_board[rand_move] <= valid_move;
                end
            end

        end
        if(swap_en) begin
            active_colour<=~active_colour;
        end
        if(current_iterations<iterations) begin
            if(increment_iteration_en) begin
                current_iterations <= current_iterations + 16'b1;
            end
        end
    end
end

control control(
    .clk(clk),
    .rst(rst),
    .sim_en(sim_en),
    .valid_move(valid_move),
    .flood_done(flood_done),
    .sim_iteration_done(sim_iteration_done),
    .game_over(game_over),

    .rand_move_en(rand_move_en),
    .play_stone_en(play_stone_en),
    .flood_en(flood_en),
    .swap_turn_en(swap_en),
    .update_score_en(update_score_en),
    .increment_iteration_en(increment_iteration_en),
    .sim_done(sim_done)
);

gen_random_move gen_random_move(
    .clk(clk),
    .rst(rst),
    .en(rand_move_en),

    .rand_move(rand_move)
);

board_count count(
    
)


endmodule