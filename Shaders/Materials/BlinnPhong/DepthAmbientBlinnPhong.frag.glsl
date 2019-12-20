#include "TransformStructs.glsl"
#include "BlinnPhong.glsl"

layout (location = 0) out vec4 out_ambient;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_diffuse;
layout (location = 3) out vec4 out_specular;

#include "VertexAttribInterface.frag.glsl"

//------------------- main ---------------------
void main() {

    // Discard non fully opaque fragments
    vec4 bc = getDiffuseColor(material, getPerVertexTexCoord().xy);
    if (toDiscard(material, bc))
    discard;

    vec3 binormal       = getWorldSpaceBiTangent();
    vec3 normalWorld    = getNormal(material, getPerVertexTexCoord().xy,
    getWorldSpaceNormal(), getWorldSpaceTangent(), binormal);

    out_ambient    = vec4(bc.rgb * 0.01, 1.0);
    out_normal     = vec4(normalWorld * 0.5 + 0.5, 1.0);
    out_diffuse    = vec4(bc.xyz, 1.0);
    out_specular   = vec4(getSpecularColor(material, getPerVertexTexCoord().xy), 1.0);
}
