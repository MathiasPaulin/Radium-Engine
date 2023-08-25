#pragma once

#include <Core/Random/SphereSampler.hpp>

namespace Ra {
namespace Core {
namespace Random {

/// @brief Uniform implementation of SphereSampler.
class RA_CORE_API UniformSphereSampler : public SphereSampler<UniformSphereSampler>
{
  public:
    UniformSphereSampler() {};
    ~UniformSphereSampler() {};

    /// @brief Implementation of getDir method from SphereSampler. Also called by the same static
    /// getDir method.
    static std::pair<Vector3, Scalar> getDirImplem( UniformGenerator* generator );

    /// @brief Implementation of pdf method from SphereSampler. Also called by the same static pdf
    /// method.
    static Scalar pdfImplem( Vector3 dir, Vector3 normal );
};

} // namespace Random
} // namespace Core
} // namespace Ra
