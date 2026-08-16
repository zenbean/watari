// FSM controlling action taken
module control(
    input logic clk,
    input logic rst,
    input logic sim_en,
    input logic valid_move,
    input logic flood_done,
    input logic sim_iteration_done, // all games simulated
    input logic game_over, // one random game finished

    output logic rand_move_en,
    output logic play_stone_en,
    output logic flood_en,
    output logic swap_turn_en,
    output logic update_score_en,
    output logic increment_iteration_en,
    output logic sim_done
);

typedef enum logic [6:0] { 
    INITIAL = 7'b0000001, 
    GEN_MOVE = 7'b0000010, 
    PLAY_STONE = 7'b0000100,
    VALIDATE = 7'b0001000,
    CAP_CHECK = 7'b0010000, 
    SWAP_TURN = 7'b0100000,
    SCORING = 7'b1000000 
    } current_state, next_state;


// next state logic
always_comb begin
    next_state = current_state;
    case(current_state)
        INITIAL: next_state=(sim_en==1'b1)?GEN_MOVE:INITIAL;
        GEN_MOVE: next_state=VALIDATE;
        VALIDATE: next_state=(valid_move==1'b1)?PLAY_STONE:GEN_MOVE;
        PLAY_STONE: next_state=CAP_CHECK;
        CAP_CHECK: next_state=(flood_done==1'b1)?SWAP_TURN:CAP_CHECK;
        SWAP_TURN: next_state=(game_over==1'b1)?SCORING:GEN_MOVE;
        SCORING: next_state=(sim_iteration_done==1'b1)?INITIAL:GEN_MOVE;
        default: next_state=INITIAL;
    endcase
end

// state register
always_ff @(posedge clk) begin
    if (rst) begin
        current_state<=INITIAL;
    end else begin
        current_state <= next_state;
    end
end

// output logic
always_comb begin
    rand_move_en=1'b0;
    play_stone_en=1'b0;
    flood_en=1'b0;
    swap_turn_en=1'b0;
    update_score_en=1'b0;
    increment_iteration_en=1'b0;
    sim_done=1'b0;

    case(current_state)
        INITIAL: begin
            sim_done=1'b1;
        end
        GEN_MOVE: begin
            rand_move_en=1'b1;
        end
        VALIDATE: begin
        end
        PLAY_STONE: begin
            play_stone_en=1'b1;
        end
        CAP_CHECK: begin
            flood_en=1'b1;
        end
        SWAP_TURN: begin
            swap_turn_en=1'b1;
        end
        SCORING: begin
            update_score_en=1'b1;
            increment_iteration_en=1'b1;
        end
    endcase
end

endmodule