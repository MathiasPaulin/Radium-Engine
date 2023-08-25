#pragma once
#include <memory>
#include <optional>
#include <string>

#include <Core/RaCore.hpp>

#include <Core/Utils/Observable.hpp>

#include <Core/Types.hpp>
#include <Core/Utils/Color.hpp>

namespace Ra {
namespace Core {
namespace Material {

/// Texture management isn't implemented at all.

/// @brief Interface used for discribing materials, compute bsdf and sample reflectance ray.
class RA_CORE_API MaterialModel : public Utils::ObservableVoid
{
  public:
    /// MATERIAL DATA
    MaterialModel( const std::string& name = "", const std::string& type = "AbstractMaterial" );
    virtual ~MaterialModel() { detachAll(); };

    /// NAME
    inline const std::string& getName() const { return m_name; }
    inline void setName( const std::string& name ) { m_name = name; }

    /// TYPE
    inline std::string getType() const { return m_materialType; }
    inline void setType( const std::string& type ) { m_materialType = type; }

    /// DEBUG
    virtual void displayInfo() const;

    /// @brief Compute BSDF value for a set of directions and texture coordinates.
    /// @param w_i incident direction in world space.
    /// @param w_o outgoing direction in world space.
    /// @param normal geometric normal direction in world space.
    /// @param uv UV coordinates for texture mapping.
    /// @return The color value of the bsdf.
    virtual Utils::Color operator()( Vector3 w_i, Vector3 w_o, Vector3 normal, Vector2 uv ) = 0;

    /// @brief Sample reflectance direction for a set of directions.
    /// @param w_i incident direction in world space.
    /// @param normal normal to the surface in world space.
    /// @param tangent tangent to the surface, perpendicular to normal, in world space.
    /// @param bitangent bitangent to the surface, perpendicular to the normal and the tangent, in
    /// world space.
    /// @return optional pair composed of outgoing direction and probability density function value
    /// associated with the direction.
    virtual std::optional<std::pair<Vector3, Scalar>>
    sample( Vector3 w_i, Vector3 normal, Vector3 tangent, Vector3 bitangent ) = 0;

    /// @brief Compute probability density function value associated with a set of directions.
    /// @param w_i incident direction in world space.
    /// @param w_o outgoing direction in world space.
    /// @param normal normal to the surface in world space.
    /// @return Scalar probability density function value associated with the directions.
    virtual Scalar pdf( Vector3 w_i, Vector3 w_o, Vector3 normal ) = 0;

  private:
    std::string m_materialType;
    std::string m_name;
};

/**
 * \brief Convenient alias for material usage
 */

using MaterialModelPtr = std::shared_ptr<MaterialModel>;

inline MaterialModel::MaterialModel( const std::string& name, const std::string& type ) :
    m_materialType { type }, m_name { name } {}

} // namespace Material
} // namespace Core
} // namespace Ra
