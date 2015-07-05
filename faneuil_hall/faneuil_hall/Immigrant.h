#ifndef __IMMIGRANT_H__
#define __IMMIGRANT_H__

#include <memory>
#include <string>
#include <mutex>
#include <condition_variable>

#include "Control.h"

class Judge;

class Immigrant
{
public:
    Immigrant(size_t id, std::shared_ptr<const Judge> judge);

    void enter();

    void checkIn();

    void sitDown();

    void swear();

    void getCertificate();

    void leave();

    static void waitForAllEnteredToCheckIn();

private:
    const std::string mId;
    const std::shared_ptr<const Judge> mJudge;

    struct CheckInInfo
    {
        size_t mEnteredButNotCheckedIn = 0;

        std::mutex mMutex;
        std::condition_variable mCondVar;
    };

    static CheckInInfo mCheckInInfo;
};

#endif