#ifndef __JUDGE_H__
#define __JUDGE_H__

#include <mutex>
#include <condition_variable>

class Judge
{
public:
    void enter();

    void confirm();

    void leave();

    void waitForNotEntered() const;

    void waitForConfirmed() const;

private:
    mutable std::mutex mMutex;

    bool mHasEntered = false;
    mutable std::condition_variable mHasEnteredCondVar;

    bool mHasConfirmed = false;
    mutable std::condition_variable mHasConfirmedCondVar;
};

#endif