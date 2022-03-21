#pragma once

class Random {
private:
    static std::random_device rd;
    static std::mt19937 mt;

public:
    static int IntRange(int rangeStart, int rangeEnd);
    static int IntValue() { return IntRange(0, 1); }

    static float FloatRange(float rangeStart, float rangeEnd);
    static float FloatValue() { return FloatRange(0, 1); }
};

