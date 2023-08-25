#include <Core/Material/BlinnPhongMaterialModel.hpp>
#include <Core/Utils/Log.hpp>

#include <algorithm>

namespace Ra {
namespace Core {
namespace Material {
void BlinnPhongMaterialModel::displayInfo() const {
    using namespace Core::Utils; // log
    auto print = []( bool ok, const std::string& name, const auto& value ) {
        if ( ok ) { LOG( logINFO ) << name << value; }
        else { LOG( logINFO ) << name << "NO"; }
    };

    LOG( logINFO ) << "======== MATERIAL INFO ========";
    print( true, " Type           : ", getType() );
    print( true, " Kd             : ", m_kd.transpose() );
    print( true, " Ks             : ", m_ks.transpose() );
    print( true, " Ns             : ", m_ns );
    print( true, " Opacity        : ", m_alpha );
    print( hasDiffuseTexture(), " Kd Texture     : ", m_texDiffuse );
    print( hasSpecularTexture(), " Ks Texture     : ", m_texSpecular );
    print( hasShininessTexture(), " Ns Texture     : ", m_texShininess );
    print( hasNormalTexture(), " Normal Texture : ", m_texNormal );
    print( hasOpacityTexture(), " Alpha Texture  : ", m_texOpacity );
}

Utils::Color Material::BlinnPhongMaterialModel::operator()( Vector3 w_i,
                                                            Vector3 w_o,
                                                            Vector3 normal,
                                                            Vector2 uv ) {
    // diffuse lambertien component
    Utils::Color diffuse = m_kd / M_PI;

    // Blinn-Phong specular component
    Vector3 halfway = w_i + w_o;
    halfway.normalize();
    Scalar specularIntensity =
        ( m_ns + 2.0f ) / ( 2.0f * M_PI ) * std::pow( normal.dot( halfway ), m_ns );
    Utils::Color specular = m_ks * specularIntensity;

    // Combine the diffuse and specular components
    Utils::Color bsdf = diffuse + specular;

    return bsdf;
}

std::optional<std::pair<Vector3, Scalar>>
BlinnPhongMaterialModel::sample( Vector3 w_i, Vector3 normal, Vector3 tangent, Vector3 bitangent ) {
    Vector3 halfway;

    Scalar distrib = m_generator.get()->get1D();

    // diffuse part
    if ( distrib < m_diffuseLuminance ) {
        std::pair<Vector3, Scalar> smpl =
            Core::Random::CosineWeightedSphereSampler::getDir( m_generator.get() );
        Vector3 wo(
            smpl.first.dot( tangent ), smpl.first.dot( bitangent ), smpl.first.dot( normal ) );
        std::pair<Vector3, Scalar> result { wo, smpl.second };

        return result;
    }
    // specular part
    else if ( distrib < m_diffuseLuminance + m_specularLuminance ) {
        std::pair<Vector3, Scalar> smpl =
            Core::Random::BlinnPhongSphereSampler::getDir( m_generator.get(), m_ns );
        Vector3 localMicroFacetNormal = smpl.first;
        Vector3 microFacetNormal( localMicroFacetNormal.dot( tangent ),
                                  localMicroFacetNormal.dot( bitangent ),
                                  localMicroFacetNormal.dot( normal ) );
        Vector3 reflected = Core::Random::BlinnPhongSphereSampler::reflect( w_i, microFacetNormal );
        std::pair<Vector3, Scalar> result { reflected, smpl.second };

        return result;
    }
    else { // no next dir
        return {};
    }
}

Scalar BlinnPhongMaterialModel::pdf( Vector3 w_i, Vector3 w_o, Vector3 normal ) {
    return std::clamp( m_diffuseLuminance *
                               Core::Random::CosineWeightedSphereSampler::pdf( w_o, normal ) +
                           m_specularLuminance *
                               Core::Random::BlinnPhongSphereSampler::pdf( w_i, w_o, normal, m_ns ),
                       0_ra,
                       1_ra );
}

void BlinnPhongMaterialModel::computeLuminance() {
    Vector3 rgbToLuminance { 0.2126_ra, 0.7152_ra, 0.0722_ra };
    Scalar dIntensity   = m_kd.rgb().dot( rgbToLuminance );
    Scalar sIntensity   = m_ks.rgb().dot( rgbToLuminance );
    Scalar diffSpecNorm = std::max( 1_ra, dIntensity + sIntensity );

    m_diffuseLuminance  = dIntensity / diffSpecNorm;
    m_specularLuminance = sIntensity / diffSpecNorm;
}

} // namespace Material
} // namespace Core
} // namespace Ra
