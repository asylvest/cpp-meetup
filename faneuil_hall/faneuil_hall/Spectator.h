#ifndef __SPECTATOR_H__
#define __SPECTATOR_H__

#include <memory>
#include <string>

#include "Judge.h"

class Spectator
{
public:
    Spectator(size_t id, std::shared_ptr<const Judge> judge);

    void enter();

    void spectate();

    void leave();

private:
    const std::string mId;
    const std::shared_ptr<const Judge> mJudge;
};

#endif