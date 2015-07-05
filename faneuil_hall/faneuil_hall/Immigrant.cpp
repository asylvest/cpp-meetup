#include <stdio.h>

#include "Immigrant.h"
#include "Judge.h"

Immigrant::CheckInInfo Immigrant::mCheckInInfo;

Immigrant::Immigrant(size_t id, std::shared_ptr<const Judge> judge) :
    mId("Immigrant " + std::to_string(id)),
    mJudge(judge)
{
}

void Immigrant::enter()
{
    fprintf(stderr, "%s: Trying to enter and waiting for judge to leave\n", mId.c_str());
    // Can't enter when judge is inside
    mJudge->waitForNotEntered();
    fprintf(stderr, "%s: Judge left so almost entered\n", mId.c_str());

    std::unique_lock<std::mutex> lock(mCheckInInfo.mMutex);
    ++mCheckInInfo.mEnteredButNotCheckedIn;
    fprintf(stderr, "%s: Entered\n", mId.c_str());
}

void Immigrant::checkIn()
{
    fprintf(stderr, "%s: Checking in\n", mId.c_str());
    std::unique_lock<std::mutex> lock(mCheckInInfo.mMutex);
    if (--mCheckInInfo.mEnteredButNotCheckedIn == 0)
    {
        // Wake up the judge if needed
        mCheckInInfo.mCondVar.notify_all();
    }

    fprintf(stderr, "%s: Checked in\n", mId.c_str());
}

void Immigrant::sitDown()
{
    // Not dependent on anything?
    fprintf(stderr, "%s: Sitting down\n", mId.c_str());
}

void Immigrant::swear()
{
    // Not dependent on anything?
    fprintf(stderr, "%s: Swearing\n", mId.c_str());
}

void Immigrant::getCertificate()
{
    // Need to wait for judge to confirm
    fprintf(stderr, "%s: Waiting to get confirmed\n", mId.c_str());
    mJudge->waitForConfirmed();
    fprintf(stderr, "%s: Got certificate\n", mId.c_str());
}

void Immigrant::leave()
{
    // Can't leave until the judge does
    fprintf(stderr, "%s: Waiting for judge to leave\n", mId.c_str());
    mJudge->waitForNotEntered();
    fprintf(stderr, "%s: Leaving\n", mId.c_str());
}

void Immigrant::waitForAllEnteredToCheckIn()
{
    std::unique_lock<std::mutex> lock(mCheckInInfo.mMutex);

    mCheckInInfo.mCondVar.wait(lock, [] { return mCheckInInfo.mEnteredButNotCheckedIn == 0; });
}