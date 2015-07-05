#include "Spectator.h"

Spectator::Spectator(size_t id, std::shared_ptr<const Judge> judge) :
    mId("Spectator #" + std::to_string(id)),
    mJudge(judge)
{
}

void Spectator::enter()
{
    // Can't enter when judge is inside
    fprintf(stderr, "%s: Trying to enter\n", mId.c_str());
    mJudge->waitForNotEntered();
    fprintf(stderr, "%s: Entered\n", mId.c_str());
}

void Spectator::spectate()
{
    // No constraints
    fprintf(stderr, "%s: Ooooooh!  Ahh!!\n", mId.c_str());
}

void Spectator::leave()
{
    // No constraints
    fprintf(stderr, "%s: Leaving\n", mId.c_str());
}