#pragma once

namespace engine::utils{
class RandomUtils{
public:
    
    RandomUtils* getInstance();

private:
    RandomUtils() = default;
    ~RandomUtils() = default;
    RandomUtils(const RandomUtils&) = delete;
    RandomUtils& operator=(const RandomUtils&) = delete;
s
private:
    static RandomUtils sm_pInstance;
    double randomDoubleInRange(double min, double max);
}
}
