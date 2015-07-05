#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>

#include "Immigrant.h"
#include "Spectator.h"
#include "Judge.h"

namespace
{
void runImmigrant(size_t id, std::shared_ptr<const Judge> judge)
{
    Immigrant immigrant(id, judge);

    // Wait a random amount of time before entering so that not all
    // immigrants make it in the first judge trip
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> distribution(1, 10);
    std::this_thread::sleep_for(std::chrono::seconds(distribution(gen)));

    immigrant.enter();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    immigrant.checkIn();
    immigrant.sitDown();
    immigrant.swear();
    immigrant.getCertificate();
    immigrant.leave();
}

void runSpectator(size_t id, std::shared_ptr<const Judge> judge)
{
    Spectator spectator(id, judge);

    // Wait a random amount of time before entering so that not all
    // spectators make it in the first judge trip
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> distribution(1, 20);
    std::this_thread::sleep_for(std::chrono::seconds(distribution(gen)));

    spectator.enter();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    spectator.spectate();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    spectator.leave();
}

void runJudge(std::shared_ptr<Judge> judge)
{
    static const size_t NUM_TRIPS = 3;

    for (size_t ii = 0; ii < NUM_TRIPS; ++ii)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        judge->enter();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        judge->confirm();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        judge->leave();
    }
}
}

int main(int argc, char** argv)
{
    try
    {
        static const size_t NUM_IMMIGRANTS = 30;
        static const size_t NUM_SPECTATORS = 20;

        auto judge = std::make_shared<Judge>();

        // Kick everything off
        std::vector<std::thread> immigrants;
        for (size_t ii = 0; ii < NUM_IMMIGRANTS; ++ii)
        {
            immigrants.emplace_back(runImmigrant, ii, judge);
        }

        std::vector<std::thread> spectators;
        for (size_t ii = 0; ii < NUM_SPECTATORS; ++ii)
        {
            spectators.emplace_back(runSpectator, ii, judge);
        }

        std::thread judgeThread(runJudge, judge);

        // Wait for everything
        for (auto& ii : immigrants)
        {
            ii.join();
        }
     
        for (auto& ii : spectators)
        {
            ii.join();
        }

        judgeThread.join();

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Kaboom: " << ex.what();
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!\n";
        return 1;
    }
}