#include "Judge.h"
#include "Immigrant.h"

void Judge::enter()
{
    fprintf(stderr, "Judge: Entering\n");
    std::unique_lock<std::mutex> lock(mMutex);
    mHasEntered = true;
    mHasConfirmed = false;
    mHasEnteredCondVar.notify_all();
}

void Judge::confirm()
{
    // Can't confirm until all immigrants that have entered have checked-in
    fprintf(stderr, "Judge: Waiting to confirm\n");
    Immigrant::waitForAllEnteredToCheckIn();

    std::unique_lock<std::mutex> lock(mMutex);
    mHasConfirmed = true;
    mHasConfirmedCondVar.notify_all();
    fprintf(stderr, "Judge: Confirmed\n");
}

void Judge::leave()
{
    fprintf(stderr, "Judge: Leaving\n");
    std::unique_lock<std::mutex> lock(mMutex);
    mHasEntered = false;
    mHasEnteredCondVar.notify_all();
}

void Judge::waitForNotEntered() const
{
    std::unique_lock<std::mutex> lock(mMutex);
    mHasEnteredCondVar.wait(lock, [this] { return !mHasEntered; });
}

void Judge::waitForConfirmed() const
{
    std::unique_lock<std::mutex> lock(mMutex);
    mHasConfirmedCondVar.wait(lock, [this] { return mHasConfirmed; });
}