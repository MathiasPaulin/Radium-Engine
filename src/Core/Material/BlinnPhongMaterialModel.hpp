#pragma once

#include <Core/Material/SimpleMaterialModel.hpp>
#include <Core/Random/BlinnPhongSphereSampler.hpp>
#include <Core/Random/UniformGenerator.hpp>
#include <Core/Utils/Color.hpp>

#include <ctime>

namespace Ra {
namespace Core {
namespace Material {

// RADIUM SUPPORTED MATERIALS

/// @brief Implementation of MaterialModel according to Blinn-Phong reflectance model.
class RA_CORE_API BlinnPhongMaterialModel : public SimpleMaterialModel
{
  public:
    explicit BlinnPhongMaterialModel(
        const std::string& name = "",
        std::shared_ptr<Core::Random::UniformGenerator> generator =
            std::make_shared<Core::Random::MersenneTwisterGenerator>() ) :
        SimpleMaterialModel( name, "BlinnPhong", generator ) {}

    ~BlinnPhongMaterialModel() override = default;

    /// DEBUG
    void displayInfo() const override;

    /// QUERY

    bool hasSpecularTexture() const { return m_hasTexSpecular; }

    bool hasShininessTexture() const { return m_hasTexShininess; }

    bool hasNormalTexture() const { return m_hasTexNormal; }

    Utils::Color operator()( Vector3 w_i, Vector3 w_o, Vector3 normal, Vector2 uv ) override;
    std::optional<std::pair<Vector3, Scalar>>
    sample( Vector3 w_i, Vector3 normal, Vector3 tangent, Vector3 bitangent ) override;
    Scalar pdf( Vector3 w_i, Vector3 w_o, Vector3 normal ) override;

    inline Utils::Color getSpecularColor() const { return m_ks; }
    inline Scalar getShininess() const { return m_ns; }
    inline Scalar getDiffuseLuminance() const { return m_diffuseLuminance; }
    inline Scalar getSpecularLuminance() const { return m_specularLuminance; }
    inline std::string getTexSpecular() const { return m_texSpecular; }
    inline std::string getTexShininess() const { return m_texShininess; }
    inline std::string getTexNormal() const { return m_texNormal; }

    inline void setDiffuseColor( Utils::Color color ) {
        m_kd    = color;
        m_alpha = color.alpha();
        computeLuminance();
    }

    inline void setSpecularColor( Utils::Color color ) {
        m_ks = color;
        computeLuminance();
    }

    inline void setShininess( Scalar specular ) { m_ns = specular; }
    inline void setTexSpecular( std::string texSpecular ) {
        m_texSpecular    = texSpecular;
        m_hasTexSpecular = true;
    }
    inline void setTexShininess( std::string texShininess ) {
        m_texShininess    = texShininess;
        m_hasTexShininess = true;
    }
    inline void setTexNormal( std::string texNormal ) {
        m_texNormal    = texNormal;
        m_hasTexNormal = true;
    }

  private:
    /// @brief Compute luminance values for diffuse color and specular color.
    void computeLuminance();

    Core::Utils::Color m_ks { 0.3_ra, 0.3_ra, 0.3_ra };
    Scalar m_ns { 64_ra };
    Scalar m_diffuseLuminance;
    Scalar m_specularLuminance;
    std::string m_texSpecular;
    std::string m_texShininess;
    std::string m_texNormal;
    bool m_hasTexSpecular { false };
    bool m_hasTexShininess { false };
    bool m_hasTexNormal { false };
};

} // namespace Material
} // namespace Core
} // namespace Ra
