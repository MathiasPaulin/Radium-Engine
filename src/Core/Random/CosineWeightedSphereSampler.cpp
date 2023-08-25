#include <Core/Random/CosineWeightedSphereSampler.hpp>

#include <Core/Math/Math.hpp>

namespace Ra {
namespace Core {
namespace Random {

std::pair<Vector3, Scalar>
Ra::Core::Random::CosineWeightedSphereSampler::getDirImplem( UniformGenerator* generator ) {
    Vector3 dir;
    Vector2 u = generator->get2D();

    Scalar cosTheta = std::cos( std::sqrt( u[0] ) );
    Scalar sinTheta = std::cos( Math::Pi / 2_ra - std::sqrt( u[0] ) );
    Scalar phi      = 2 * Math::Pi * u[1];

    dir[0] = sinTheta * std::cos( phi );
    dir[1] = sinTheta * std::sin( phi );
    dir[2] = cosTheta;

    return { dir, u[0] / Math::Pi };
}

Scalar CosineWeightedSphereSampler::pdfImplem( Vector3 dir, Vector3 normal ) {
    dir.normalize();
    return dir.dot( normal ) / Math::Pi;
}

} // namespace Random
} // namespace Core
} // namespace Ra
