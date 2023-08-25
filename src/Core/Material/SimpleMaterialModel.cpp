#include <Core/Material/SimpleMaterialModel.hpp>

#include <Core/Utils/Log.hpp>

namespace Ra {
namespace Core {
namespace Material {

void SimpleMaterialModel::displayInfo() const {
    using namespace Core::Utils; // log
    auto print = []( bool ok, const std::string& name, const auto& value ) {
        if ( ok ) { LOG( logINFO ) << name << value; }
        else { LOG( logINFO ) << name << "NO"; }
    };

    LOG( logINFO ) << "======== MATERIAL INFO ========";
    print( true, " Type           : ", getType() );
    print( true, " Kd             : ", m_kd.transpose() );
    print( true, " Opacity        : ", m_alpha );
    print( hasDiffuseTexture(), " Kd Texture     : ", m_texDiffuse );
    print( hasOpacityTexture(), " Alpha Texture  : ", m_texOpacity );
}

Utils::Color
SimpleMaterialModel::operator()( Vector3 w_i, Vector3 w_o, Vector3 normal, Vector2 uv ) {
    return m_kd;
}
std::optional<std::pair<Vector3, Scalar>>
SimpleMaterialModel::sample( Vector3 w_i, Vector3 normal, Vector3 tangent, Vector3 bitangent ) {
    return {};
}
Scalar SimpleMaterialModel::pdf( Vector3 w_i, Vector3 w_o, Vector3 normal ) {
    return 0_ra;
}

void LambertianMaterialModel::displayInfo() const {
    using namespace Core::Utils; // log
    auto print = []( bool ok, const std::string& name, const auto& value ) {
        if ( ok ) { LOG( logINFO ) << name << value; }
        else { LOG( logINFO ) << name << "NO"; }
    };

    LOG( logINFO ) << "======== MATERIAL INFO ========";
    print( true, " Type           : ", getType() );
    print( true, " Kd             : ", m_kd.transpose() );
    print( true, " Opacity        : ", m_alpha );
    print( hasDiffuseTexture(), " Kd Texture     : ", m_texDiffuse );
    print( hasNormalTexture(), " Normal Texture : ", m_texNormal );
    print( hasOpacityTexture(), " Alpha Texture  : ", m_texOpacity );
}

Utils::Color Material::LambertianMaterialModel::operator()( Vector3 w_i,
                                                            Vector3 w_o,
                                                            Vector3 normal,
                                                            Vector2 uv ) {
    if ( w_i.dot( normal ) <= 0 || w_o.dot( normal ) <= 0 ) { return Utils::Color::Black(); }

    return m_kd / M_PI;
}

std::optional<std::pair<Vector3, Scalar>>
LambertianMaterialModel::sample( Vector3 w_i, Vector3 normal, Vector3 tangent, Vector3 bitangent ) {
    // sample point on hemisphere with cosine-weighted distribution
    std::pair<Vector3, Scalar> smpl =
        Core::Random::CosineWeightedSphereSampler::getDir( m_generator.get() );

    // transform sampled point from local to world coodinate system
    Vector3 w_o( smpl.first.dot( tangent ), smpl.first.dot( bitangent ), smpl.first.dot( normal ) );
    std::pair<Vector3, Scalar> result { w_o, smpl.second };

    return result;
}

Scalar LambertianMaterialModel::pdf( Vector3 w_i, Vector3 w_o, Vector3 normal ) {
    return Core::Random::CosineWeightedSphereSampler::pdf( w_o, normal );
}

} // namespace Material
} // namespace Core
} // namespace Ra
