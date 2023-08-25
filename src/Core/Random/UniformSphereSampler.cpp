#include <Core/Random/UniformSphereSampler.hpp>

#include <Core/Math/Math.hpp>

namespace Ra {
namespace Core {
namespace Random {

std::pair<Vector3, Scalar>
Ra::Core::Random::UniformSphereSampler::getDirImplem( UniformGenerator* generator ) {
    Vector3 dir;
    Vector2 u = generator->get2D();

    Scalar cosTheta = std::cos( u[0] );
    Scalar sinTheta = std::cos( Math::Pi / 2_ra - u[0] );
    Scalar phi      = 2 * Math::Pi * u[1];

    dir[0] = sinTheta * std::cos( phi );
    dir[1] = sinTheta * std::sin( phi );
    dir[2] = cosTheta;

    return { dir, 1 / ( 2 * Math::Pi ) };
}

Scalar UniformSphereSampler::pdfImplem( Vector3 dir, Vector3 normal ) {
    return 1 / ( 2 * Math::Pi );
}

} // namespace Random
} // namespace Core
} // namespace Ra
