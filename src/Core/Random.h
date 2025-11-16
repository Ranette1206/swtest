#pragma once

#include <cstdint>
#include <random>
#include <vector>

namespace sw::core
{

class Random
{
public:
    // Random number generator in range [0, max)
    static size_t getRandomIndex(size_t max);

    // Random number generator in range [min, max]
    static int32_t getRandomInt(int32_t min, int32_t max);

    // Random number generator in range [min, max] for size_t
    static size_t getRandomSize(size_t min, size_t max);

    // Select random element from vector
    template <typename T>
    static const T& getRandomElement(const std::vector<T>& container)
    {
        if (container.empty())
            throw std::runtime_error("Container is empty");

        size_t index = getRandomIndex(container.size());
        return container[index];
    }

    // Select random element from vector (non-const version)
    template <typename T>
    static T& getRandomElement(std::vector<T>& container)
    {
        if (container.empty())
            throw std::runtime_error("Container is empty");

        size_t index = getRandomIndex(container.size());
        return container[index];
    }

private:
    static std::random_device& getRandomDevice();
    static std::mt19937& getGenerator();
};

} // namespace sw::core
