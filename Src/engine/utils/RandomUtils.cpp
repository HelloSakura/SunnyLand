#include<RandomUtils.h>
#include<ctime>
#include<cstdlib>

namespace engine::utils{


RandomUtils RandomUtils::sm_pInstance;
RandomUtils& RandomUtils::getInstance()
{
    return sm_pInstance;
}

RandomUtils::RandomUtils()
{
    srand(static_cast<unsigned int>(time(NULL)));   
}

RandomUtils::~RandomUtils()
{

}

double RandomUtils::randomDoubleInRange(double min, double max)
{
    return min + (max - min) * static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

}