#include<RandomUtils.h>


namespace engine::utils{

RandomUtils::RandomUtils()
{

}

RandomUtils::~RandomUtils()
{

}

RandomUtils* getInstance()
{
    if(m_pInstance == nullptr){
        m_pInstance = new RandomUtils();
    }
    return m_pInstance;
}

}