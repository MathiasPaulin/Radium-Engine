#pragma once

#include <Core/Random/UniformGenerator.hpp>
#include <Core/Types.hpp>

namespace Ra {
namespace Core {
namespace Random {

/// @brief Interface used to sample a hemisphere. This class is a CRTP in order to benefit from
/// polymorphism and static methods.
/// @tparam T is the class that implement this interface.
template <class T>
class RA_CORE_API SphereSampler
{
  public:
    SphereSampler() {};
    ~SphereSampler() {};

    /// @brief Samples the sphere. Static method that calls the implemented one from the implemented
    /// class T. All computations are done in canonical frame.
    /// @param generator Uniform random generator used for sampling.
    /// @return Returns a pair made of sampled direction and probability density function value
    /// associated.
    static std::pair<Vector3, Scalar> getDir( UniformGenerator* generator ) {
        return T::getDirImplem( generator );
    }

    /// @brief Compute probability density function value associated with a outgoing direction and a
    /// normal vector. Static method that calls the implemented one from the implemented class T.
    /// @param dir Outgoing direction in world frame.
    /// @param normal Normal vector in world frame, gives the orientation of the sampled hemisphere.
    /// @return Scalar probability.
    static Scalar pdf( Vector3 dir, Vector3 normal ) { return T::pdfImplem( dir, normal ); }
};

} // namespace Random
} // namespace Core
} // namespace Ra
