#include <Core/Random/MersenneTwisterGenerator.hpp>

namespace Ra {
namespace Core {
namespace Random {

MersenneTwisterGenerator::MersenneTwisterGenerator() :
    m_randomEngine( std::mt19937( std::time( nullptr ) ) ) {}

Scalar Ra::Core::Random::MersenneTwisterGenerator::get1D() {
    return m_unifDistributionRand( m_randomEngine );
}

} // namespace Random
} // namespace Core
} // namespace Ra
