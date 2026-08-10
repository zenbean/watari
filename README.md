# watari
Lightweight Go engine written in C++ that interfaces with the Sabaki UI

**Current state:** When prompted by Sabaki, the engine runs a multithreaded Monte Carlo Tree Search for the best next move. Currently is able to evaluate 8000 iterations in 5 seconds.

## setup
Ubuntu 22.04

C++ 17

gcc 11.4.0

Sabaki Version: v0.60.0

## running the engine
Compile the engine:
```bash
./compile.sh
```

Install [Sabaki UI](https://github.com/SabakiHQ/Sabaki) and attach Watari as an engine

Set path: `wsl`

Set argument: `\<wsl path\>/watari/build/watari`

## features
### Monte Carlo Tree Search
This game engine uses a multithreaded MCTS algorithm with root parallelisation, where each thread builds an independent search tree from the same initial root state. This approach avoids the heavy overhead of memory sharing and synchronisation locks across threads. An added advantage of this parallelism is wider trajectory diversity; exploring different independent paths increases the probability of finding a strong final action. Moving from single threaded to dual threaded architecture resulted in a significant ~40x computational speedup, achieving 8000 iterations in 5 seconds

### Disjoint Set Union
Initially designed using DFS graph traversals but it quickly became clear it would not be a sustainable approach if this was to be scaled up for a larger board size. To search the entire board for every single move is highly inefficient and will become a bottleneck later in the project when it is scaled to an AI engine rather than just a Go game engine.

Moving to Disjoint Set Union reduced the number of searches needed as the board state, stone state, liberty states, etc.. since they are all tracked throughout the course of a game. This also had an added benefit of reducing complexity of the logic, making it much easier to debug and write tests.

### Zobrist Hashing
Zobrist hashing is done by randomly generating bitstrings for each possible element of a board game, i.e. for each combination of a piece and a position.
This is a common strategy used in chess engine programming as a way to avoid re-analysing repeated positions to speed up the searching process of the best possible move.

In Go, that will be 2 colours $\times$ 81 board positions (for a 9x9 board).
By doing this, any board configuration can be represented by a random bitstring that is generated when the program is initalised.
By using zobrist hashing, repeated board states can be found more quickly and AI engine implementation will be more lightweight
The mechanism of zobrist hashing ultimately relies on bitwise XOR operation, by exploiting the characteristic:

$$
A \oplus B \oplus C = A \text{ \\  \\ \\ \\ if C = B}
$$

Where A, B and C are random bitstrings that were generated when initialising the zobrist hash table. The current board state is represented by the running total of the XOR operations, and only this value needs to be stored and kept track of to detect repeated board states.

By representing the initial empty board state with an empty hash of 0. We can track the board state thorugh a sequence of moves:
1. $0 \oplus A = A$
2. $A \oplus B = U$
3. $U \oplus C \oplus B = V$
4. $V\oplus B \oplus C = U$
#### Explanation:
1. **First player:** places black stone at index M. Value in LUT corresponds to A
2. **Second player:** places white stone at index N. Value in LUT corresponds to B. The resulting board state is U
3. **First player:** places black stone at index R (LUT value C) that captures white stone at index N (LUT value B). Effect of B is erased, simplifies to $A\oplus C$, the new board state is V
4. **Second player:** places white stone at index N (LUT value B) to recapture black stone at index R (LUT value C). This move is Ko rule violation, results in a repeated board state of U

Board state is tracked by performing bitwise XOR operation on all board state changes that occur due to the stones (placement, resulting captures and black/white turn). From the tracked history of the running total of hash results, this repeated board state (U) will be spotted immediately and flagged as an illegal Ko move. This avoids us needing to scan every board position to compare stone placements and storing entire board representations but instead can be compactly represented by a single hash value.

For further optimisation, zobrist hashing is implemented using lazy initialisation. This is possible because the hash table and turn bitstring values don't change throughout a game or if a new board is started, therefore it can be generated and defined at the beginning of the program once.

### Sabaki UI
This Go game engine can be plugged into Sabaki UI, a ready made electron based UI. It requires the game engine to be able to parse GTP input and outputs. GTP is a common used standard used in game engines, whereas SGF is common used standard of recording moves in Go. Both format parsing are implemented in Watari.

## definitions
**Stones**:
These are the black and white playing pieces

**Group:**
A chain of stones of the same colour

**Liberties:**
Number of liberties correspond to the number of unoccupied adjacent positions next to a group. Liberty of each stone in a group accumulates

**Ko:**
Ko is required to prevent infinite loops of board states, there are many variations of this rule to tackle this infinite loop issue. This game engine uses Situational SuperKo. Prohibits the repetition of a position or board state with the same player to play. This is the standard used by American and British Go Association, and in AlphaGo. Can be translated as 'consequence'

## game rules
Go is played on grid boards (typically 9x9, 13x13, or 19x19). Black and White take turns placing stones at the intersection points of the grid, or passing their turn. The game ends when two successive passes are played.

This game engine enforces situational superko, where a repetition of past board states with the same player to play is deemed an illegal move.

A player’s score is the number of points of their colour, plus the number of empty points that reach only their colour. The player with the higher score at the end of the game is the winner. Equal scores result in a tie. (From [Tromp-Taylor rules](https://tromp.github.io/go.html))

