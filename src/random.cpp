#include "random.hpp"

std::random_device Random::rd;
std::mt19937 Random::mt(Random::rd());

int Random::IntRange(int rangeStart, int rangeEnd) {
    std::uniform_int_distribution<> dist(rangeStart, rangeEnd);
    return dist(mt);
}

float Random::FloatRange(float rangeStart, float rangeEnd) {
    std::uniform_real_distribution<float> dist(rangeStart, rangeEnd);
    return dist(mt);
}
