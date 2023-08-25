#include <Core/Random/BlinnPhongSphereSampler.hpp>

#include <Core/Math/Math.hpp>

namespace Ra {
namespace Core {
namespace Random {

std::pair<Vector3, Scalar>
Ra::Core::Random::BlinnPhongSphereSampler::getDir( UniformGenerator* generator, Scalar shininess ) {
    Vector3 dir;
    Vector2 u = generator->get2D();

    Scalar cosTheta = std::pow( 1_ra - u[0], 1_ra / ( shininess + 2 ) );
    Scalar sinTheta = std::sqrt( 1_ra - cosTheta * cosTheta );
    Scalar phi      = 2 * Math::Pi * u[1];

    dir[0] = sinTheta * std::cos( phi );
    dir[1] = sinTheta * std::sin( phi );
    dir[2] = cosTheta;

    return { dir, ( shininess + 2 ) * std::pow( 1_ra - u[0], shininess ) / ( 2 * Math::Pi ) };
}

Scalar BlinnPhongSphereSampler::pdf( Vector3 w_i, Vector3 w_o, Vector3 normal, Scalar shininess ) {
    w_i.normalize();
    w_o.normalize();

    Vector3 halfway = ( w_i + w_o ).normalized();
    Scalar cosTheta = normal.dot( halfway );

    return ( shininess + 2 ) * std::pow( cosTheta, shininess ) / ( 2 * Math::Pi );
}

Vector3 BlinnPhongSphereSampler::reflect( Vector3 inDir, Vector3 normal ) {
    inDir.normalize();
    normal.normalize();
    return ( -inDir + 2 * inDir.dot( normal ) * normal ).normalized();
}

} // namespace Random
} // namespace Core
} // namespace Ra
