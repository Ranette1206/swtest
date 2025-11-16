#include "Core/Random.h"

#include <stdexcept>

namespace sw::core
{

std::random_device& Random::getRandomDevice()
{
    static std::random_device rd;
    return rd;
}

std::mt19937& Random::getGenerator()
{
    static std::mt19937 gen(getRandomDevice()());
    return gen;
}

size_t Random::getRandomIndex(size_t max)
{
    if (max == 0)
        throw std::invalid_argument("max cannot be 0");

    std::uniform_int_distribution<size_t> dist(0, max - 1);
    return dist(getGenerator());
}

int32_t Random::getRandomInt(int32_t min, int32_t max)
{
    if (min > max)
        throw std::invalid_argument("min cannot be greater than max");

    std::uniform_int_distribution<int32_t> dist(min, max);
    return dist(getGenerator());
}

size_t Random::getRandomSize(size_t min, size_t max)
{
    if (min > max)
        throw std::invalid_argument("min cannot be greater than max");

    std::uniform_int_distribution<size_t> dist(min, max);
    return dist(getGenerator());
}

} // namespace sw::core
