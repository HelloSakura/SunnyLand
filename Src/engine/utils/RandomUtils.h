#pragma once

namespace engine::utils{

class RandomUtils{
public:
    static RandomUtils& getInstance();
    double randomDoubleInRange(double min, double max);

private:
    RandomUtils();
    ~RandomUtils();
    RandomUtils(const RandomUtils&) = delete;
    RandomUtils& operator=(const RandomUtils&) = delete;

private:
    static RandomUtils sm_pInstance;
};
};
