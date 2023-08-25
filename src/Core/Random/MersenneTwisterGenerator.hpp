#pragma once

#include <Core/Random/UniformGenerator.hpp>

#include <ctime>

namespace Ra {
namespace Core {
namespace Random {

/// @brief Implementation of the UniformGenerator class using the std Mersenne Twister Generator
/// (std::mt19937).
class RA_CORE_API MersenneTwisterGenerator : public UniformGenerator
{
  public:
    MersenneTwisterGenerator();
    ~MersenneTwisterGenerator() override = default;

    Scalar get1D() override;

  private:
    std::mt19937 m_randomEngine;
};

} // namespace Random
} // namespace Core
} // namespace Ra
