// Assume that plain rendering is based on very simple "material", without lighting.
struct PlainTextures
{
    int hasKd;
    int hasAlpha;

    sampler2D kd;
    sampler2D alpha;
};

struct Material
{
    vec4 kd;
    float alpha;
    PlainTextures tex;
};

uniform Material material;

// if this is really needed, include "DefaultLight.glsl"
//uniform Light light;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_texcoord;
layout (location = 2) in vec3 in_color;

out vec4 out_color;

void main()
{
    if (material.tex.hasAlpha == 1 && texture(material.tex.alpha, in_texcoord.st).r < 0.1)
    {
        discard;
    }
    
    if ( material.tex.hasKd == 1 )
    {
        out_color = vec4(texture(material.tex.kd, in_texcoord.st).rgb, 1);
    }
    else
    {
        out_color = vec4(in_color.rgb, 1.0);
    }
}

