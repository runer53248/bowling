#define CATCH_CONFIG_MAIN

#include <array>
#include <limits>
#include "catch_amalgamated.hpp"

enum class Tries {
    First,
    Second
};

enum Result {
    Strike,
    Spare,
    Miss,
    Unresolved,
    Disqualification
};

using Pins = unsigned short;

Result BowlerResult([[maybe_unused]] Tries tries, [[maybe_unused]] Pins knockedPins) {
    return Result::Unresolved;
}

struct FrameResult {
    Result addBrawlerResult([[maybe_unused]] Pins knockedPins) {
        return Result::Unresolved;
    }

    Pins firstBallKnocketPins{};
    Pins secondBallKnocketPins{};
    Result generalResult{Result::Unresolved};
};

int getFramePoints([[maybe_unused]] const FrameResult& frameResult, [[maybe_unused]] const FrameResult& frameResult2 = {}, [[maybe_unused]] const FrameResult& frameResult3 = {}) {
    return 0;
}

using Frames = std::array<FrameResult, 11>;

Frames textToFrames([[maybe_unused]] const std::string& framesText) {
    return Frames{};
}

constexpr Pins allPins = 10;

TEST_CASE("First ball knock all pins", "[BowlerResult]") {
    auto result = BowlerResult(Tries::First, allPins);
    CHECK(result == Result::Strike);
}

TEST_CASE("First ball knock less pins", "[BowlerResult]") {
    auto knocketPins = GENERATE(0, 1, 5, allPins - 1);
    auto result = BowlerResult(Tries::First, knocketPins);
    CHECK(result == Result::Miss);
}

TEST_CASE("First ball knock more than all pins", "[BowlerResult]") {
    Pins knocketPins = GENERATE(allPins + 1, allPins + 2, std::numeric_limits<Pins>::max(), std::numeric_limits<Pins>::max() - 1);
    auto result = BowlerResult(Tries::First, knocketPins);
    CHECK(result == Result::Disqualification);
}

TEST_CASE("Second ball knock all pins", "[BowlerResult]") {
    auto result = BowlerResult(Tries::Second, allPins);
    CHECK(result == Result::Spare);
}

TEST_CASE("Second ball knock less pins", "[BowlerResult]") {
    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);
    auto result = BowlerResult(Tries::Second, knocketPins);
    CHECK(result == Result::Miss);
}

TEST_CASE("Second ball knock more than all pins", "[BowlerResult]") {
    Pins knocketPins = GENERATE(allPins + 1, allPins + 2, std::numeric_limits<Pins>::max(), std::numeric_limits<Pins>::max() - 1);
    auto result = BowlerResult(Tries::Second, knocketPins);
    CHECK(result == Result::Disqualification);
}

/////////////////////////////////////////////////////////////

TEST_CASE("Initial frameResult should be unresolved", "[FrameResult]") {
    FrameResult frameResult;
    CHECK(frameResult.generalResult == Result::Unresolved);
    CHECK(frameResult.firstBallKnocketPins == 0);
    CHECK(frameResult.secondBallKnocketPins == 0);
}

TEST_CASE("frameResult after first miss", "[FrameResult]") {
    FrameResult frameResult;
    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);

    auto result = frameResult.addBrawlerResult(knocketPins);

    CHECK(result == Result::Miss);
    CHECK(frameResult.generalResult == Result::Unresolved);
    CHECK(frameResult.firstBallKnocketPins == knocketPins);
    CHECK(frameResult.secondBallKnocketPins == 0);
}

TEST_CASE("frameResult after second miss", "[FrameResult]") {
    FrameResult frameResult;
    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);
    Pins knocketPins2 = GENERATE(0, 1, 5, allPins - 1);
    auto [first, second] = std::minmax(knocketPins, knocketPins2);

    frameResult.addBrawlerResult(first);
    auto result = frameResult.addBrawlerResult(second);

    CHECK(result == Result::Miss);
    CHECK(frameResult.generalResult == Result::Miss);
    CHECK(frameResult.firstBallKnocketPins == first);
    CHECK(frameResult.secondBallKnocketPins == second);
}

TEST_CASE("frameResult after spare", "[FrameResult]") {
    FrameResult frameResult;
    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);

    frameResult.addBrawlerResult(knocketPins);
    auto result = frameResult.addBrawlerResult(allPins);

    CHECK(result == Result::Spare);
    CHECK(frameResult.generalResult == Result::Spare);
    CHECK(frameResult.firstBallKnocketPins == knocketPins);
    CHECK(frameResult.secondBallKnocketPins == allPins);
}

TEST_CASE("frameResult after second miss have less knocket pins", "[FrameResult]") {
    FrameResult frameResult;
    Pins knocketPins = GENERATE(0, 2, 6, allPins - 2);
    Pins knocketPins2 = GENERATE(1, 3, 5, allPins - 1);
    auto [first, second] = std::minmax(knocketPins, knocketPins2);

    frameResult.addBrawlerResult(second);
    auto result = frameResult.addBrawlerResult(first);

    CHECK(result == Result::Miss);
    CHECK(frameResult.generalResult == Result::Disqualification);
    CHECK(frameResult.firstBallKnocketPins == second);
    CHECK(frameResult.secondBallKnocketPins == 0);
}

TEST_CASE("frameResult after strike", "[FrameResult]") {
    FrameResult frameResult;

    auto result = frameResult.addBrawlerResult(allPins);

    CHECK(result == Result::Strike);
    CHECK(frameResult.generalResult == Result::Strike);
    CHECK(frameResult.firstBallKnocketPins == allPins);
    CHECK(frameResult.secondBallKnocketPins == 0);
}

TEST_CASE("frameResult after strike try strike same frame", "[FrameResult]") {
    FrameResult frameResult;
    Pins knocketPins = GENERATE(0, 2, 6, allPins - 1, allPins);

    frameResult.addBrawlerResult(allPins);
    frameResult.addBrawlerResult(knocketPins);

    CHECK(frameResult.generalResult == Result::Disqualification);
    CHECK(frameResult.firstBallKnocketPins == allPins);
    CHECK(frameResult.secondBallKnocketPins == 0);
}

/////////////////////////////////////////////////////////////

TEST_CASE("getFramePoints from unresolved frameResult", "[getFramePoints]") {
    FrameResult frameResult;
    auto points = getFramePoints(frameResult);
    CHECK(points == 0);
}

TEST_CASE("getFramePoints from miss frameResult", "[getFramePoints]") {
    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);
    Pins knocketPins2 = GENERATE(0, 1, 5, allPins - 1);
    auto [first, second] = std::minmax(knocketPins, knocketPins2);
    FrameResult frameResult{first, second, Result::Miss};

    auto points = getFramePoints(frameResult);
    CHECK(points == second);
}

TEST_CASE("getFramePoints from strike frameResult falowed by misses", "[getFramePoints]") {
    FrameResult strikeframeResult{10, 0, Result::Strike};
    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);
    Pins knocketPins2 = GENERATE(0, 1, 5, allPins - 1);
    auto [first, second] = std::minmax(knocketPins, knocketPins2);
    FrameResult frameResult{first, second, Result::Miss};

    auto points = getFramePoints(strikeframeResult, frameResult);
    CHECK(points == allPins + first + second);
}

TEST_CASE("getFramePoints from strike frameResult falowed by strikes", "[getFramePoints]") {
    FrameResult strikeframeResult{10, 0, Result::Strike};

    auto points = getFramePoints(strikeframeResult, strikeframeResult, strikeframeResult);
    CHECK(points == allPins + allPins + allPins);
}

TEST_CASE("getFramePoints from spare frameResult falowed by misses", "[getFramePoints]") {
    Pins prespareKnockedPins = GENERATE(0, 1, 5, allPins - 1);
    FrameResult spareframeResult{prespareKnockedPins, 10, Result::Spare};

    Pins knocketPins = GENERATE(0, 1, 5, allPins - 1);
    Pins knocketPins2 = GENERATE(0, 1, 5, allPins - 1);
    auto [first, second] = std::minmax(knocketPins, knocketPins2);
    FrameResult frameResult{first, second, Result::Miss};

    auto points = getFramePoints(spareframeResult, frameResult);
    CHECK(points == allPins + first);
}

TEST_CASE("getFramePoints from spare frameResult falowed by strikes", "[getFramePoints]") {
    Pins prespareKnockedPins = GENERATE(0, 1, 5, allPins - 1);
    FrameResult spareframeResult{prespareKnockedPins, 10, Result::Spare};
    FrameResult strikeframeResult{10, 0, Result::Strike};

    auto points = getFramePoints(spareframeResult, strikeframeResult, strikeframeResult);
    CHECK(points == allPins + allPins);
}

/////////////////////////////////////////////////////////////

TEST_CASE("Initial frames should be unresolved", "[Frames]") {
    Frames frames;
    for (const auto& frame : frames) {
        CHECK(frame.generalResult == Result::Unresolved);
        CHECK(frame.firstBallKnocketPins == 0);
        CHECK(frame.secondBallKnocketPins == 0);
    }
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
