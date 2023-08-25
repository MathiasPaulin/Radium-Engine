#pragma once

#include <Core/Types.hpp>

#include <random>

namespace Ra {
namespace Core {
namespace Random {

/// @brief Interface used for implementing uniform random generator.
class RA_CORE_API UniformGenerator
{
  public:
    UniformGenerator() {};
    virtual ~UniformGenerator() = 0;

    /// @brief Virtual method used to get a one dimention random scalar.
    /// @return random Scalar between 0 and 1.
    virtual Scalar get1D() = 0;

    /// @brief Virtual method used to get a two dimension random vector.
    /// default implementation calls get1D() twice.
    /// @return random Vector2 between 0 and 1.
    virtual Vector2 get2D();
    /// @brief Virtual method used to get a three dimension random vector.
    /// default implementation calls get1D() three times.
    /// @return random Vector3 between 0 and 1.
    virtual Vector3 get3D();
    /// @brief  Virtual method used to get a X dimension random vector.
    /// default implementation calls get1D() X times.
    /// @param dim int dimention of the vector.
    /// @return random VectorN (with N corresponding to dim) between 0 and 1.
    virtual VectorN getXD( int dim );

  protected:
    std::uniform_real_distribution<Scalar> m_unifDistributionRand { 0, 1 };
};

} // namespace Random
} // namespace Core
} // namespace Ra
