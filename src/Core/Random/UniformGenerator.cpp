#include <Core/Random/UniformGenerator.hpp>

namespace Ra {
namespace Core {
namespace Random {

Vector2 Ra::Core::Random::UniformGenerator::get2D() {
    return {get1D(), get1D()};
}

Vector3 Ra::Core::Random::UniformGenerator::get3D() {
    return {get1D(), get1D(), get1D()};
}

VectorN Ra::Core::Random::UniformGenerator::getXD( int dim ) {
    VectorN result;

    result.resize(dim, 1);

    for(int i = 0; i < dim; i++) {
        result[i] = get1D();
    }

    return result;
}

} // namespace Random
} // namespace Core
} // namespace Ra
