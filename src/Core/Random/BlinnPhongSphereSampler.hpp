#pragma once

#include <Core/Random/CosineWeightedSphereSampler.hpp>

namespace Ra {
namespace Core {
namespace Random {

/// @brief Implementation of SphereSampler following the Blinn-Phong reflectance model. Inherit from
/// CosineWeightedSphereSampler to compute the lambertian component.
class RA_CORE_API BlinnPhongSphereSampler : public CosineWeightedSphereSampler
{
  public:
    BlinnPhongSphereSampler() {};
    ~BlinnPhongSphereSampler() {};

    /// @brief Static method to sample the sphere according to Blinn-Phong model. All computations
    /// are done in canonical frame.
    /// @param generator Uniform random generator used to sample.
    /// @param shininess Blinn-Phong exponent.
    /// @return Returns a pair made of sampled direction and probability density function value
    /// associated.
    static std::pair<Vector3, Scalar> getDir( UniformGenerator* generator, Scalar shininess );

    /// @brief Static method to compute probability density function value associated with a
    /// outgoing direction and a normal vector according to Blinn-Phong reflectance model.
    /// @param w_i ingoing direction in world frame.
    /// @param w_o Outgoing direction in world frame.
    /// @param normal Normal vector in world frame, gives the orientation of the sampled hemisphere.
    /// @param shininess Blinn-Phong exponent.
    /// @return Scalar probability.
    static Scalar pdf( Vector3 w_i, Vector3 w_o, Vector3 normal, Scalar shininess );

    /// @brief Compute the reflected direction of the inDir direction by the surface represented by
    /// the normal vector.
    /// @param inDir ingoing direction, must be in the same frame than normal.
    /// @param normal normal to the surface that reflect, must be in the same frame than inDir.
    /// @return reflected direction in the same frame than parameters.
    static Vector3 reflect( Vector3 inDir, Vector3 normal );
};

} // namespace Random
} // namespace Core
} // namespace Ra
