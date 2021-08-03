#define CATCH_CONFIG_MAIN

#include "catch_amalgamated.hpp"
#include <limits>

enum class Tries {
    First,
    Second
};

enum Result {
    Strike,
    Spare,
    Miss
};

using Pins = unsigned short;

Result BowlerResult([[maybe_unused]] Tries tries, [[maybe_unused]] Pins knockedPins) {
    return Result::Miss;
}

constexpr Pins allPins = 10;

TEST_CASE("First ball knock all pins", "[Bowling]") {
    auto result = BowlerResult(Tries::First, allPins);
    REQUIRE(result == Result::Strike);
}

TEST_CASE("First ball knock less pins") {
    auto knocketPins = GENERATE(0, 1, 5, 9);
    auto result = BowlerResult(Tries::First, knocketPins);
    REQUIRE(result == Result::Miss);
}

TEST_CASE("First ball knock more than all pins", "[Bowling]") {
    Pins knocketPins = GENERATE(allPins + 1, allPins + 2, std::numeric_limits<Pins>::max(), std::numeric_limits<Pins>::max() - 1);
    auto result = BowlerResult(Tries::First, knocketPins);
    REQUIRE(result == Result::Miss);
}

TEST_CASE("Second ball knock all pins", "[Bowling]") {
    auto result = BowlerResult(Tries::Second, allPins);
    REQUIRE(result == Result::Spare);
}

TEST_CASE("Second ball knock less pins", "[Bowling]") {
    Pins knocketPins = GENERATE(0, 1, 5, 9);
    auto result = BowlerResult(Tries::Second, knocketPins);
    REQUIRE(result == Result::Miss);
}

TEST_CASE("Second ball knock more than all pins", "[Bowling]") {
    Pins knocketPins = GENERATE(allPins + 1, allPins + 2, std::numeric_limits<Pins>::max(), std::numeric_limits<Pins>::max() - 1);
    auto result = BowlerResult(Tries::Second, knocketPins);
    REQUIRE(result == Result::Miss);
}

/*
"frames" for the bowler.

In each frame, the bowler gets up to two tries to
knock down all ten pins.

If the first ball in a frame knocks down all ten pins,
this is called a "strike". The frame is over. The score
for the frame is ten plus the total of the pins knocked
down in the next two balls.

If the second ball in a frame knocks down all ten pins,
this is called a "spare". The frame is over. The score
for the frame is ten plus the number of pins knocked
down in the next ball.

If, after both balls, there is still at least one of the
ten pins standing the score for that frame is simply
the total number of pins knocked down in those two balls.

If you get a spare in the last (10th) frame you get one
more bonus ball. If you get a strike in the last (10th)
frame you get two more bonus balls.
These bonus throws are taken as part of the same turn.
If a bonus ball knocks down all the pins, the process
does not repeat. The bonus balls are only used to
calculate the score of the final frame.

The game score is the total of all frame scores.

Examples:

X indicates a strike
/ indicates a spare
- indicates a miss
| indicates a frame boundary
The characters after the || indicate bonus balls

X|X|X|X|X|X|X|X|X|X||XX
Ten strikes on the first ball of all ten frames.
Two bonus balls, both strikes.
Score for each frame == 10 + score for next two
balls == 10 + 10 + 10 == 30
Total score == 10 frames x 30 == 300

9-|9-|9-|9-|9-|9-|9-|9-|9-|9-||
Nine pins hit on the first ball of all ten frames.
Second ball of each frame misses last remaining pin.
No bonus balls.
Score for each frame == 9
Total score == 10 frames x 9 == 90

5/|5/|5/|5/|5/|5/|5/|5/|5/|5/||5
Five pins on the first ball of all ten frames.
Second ball of each frame hits all five remaining
pins, a spare.
One bonus ball, hits five pins.
Score for each frame == 10 + score for next one
ball == 10 + 5 == 15
Total score == 10 frames x 15 == 150

X|7/|9-|X|-8|8/|-6|X|X|X||81
Total score == 167
*/
