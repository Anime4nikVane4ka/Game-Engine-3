#include "BestTimeStorage.h"

#include <fstream>

namespace {
constexpr const char* BestTimeFile = "best_time.txt";
}

float BestTimeStorage::Load() {
    std::ifstream file(BestTimeFile);

    if (!file.is_open()) {
        return -1.0f;
    }

    float bestTime = -1.0f;
    file >> bestTime;

    return bestTime;
}

void BestTimeStorage::SaveIfBetter(float timeSeconds) {
    const float currentBest = Load();

    if (currentBest >= 0.0f && currentBest <= timeSeconds) {
        return;
    }

    std::ofstream file(BestTimeFile);

    if (file.is_open()) {
        file << timeSeconds;
    }
}