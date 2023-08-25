\page develcorematerials Core Materials
[TOC]

A Ra::Core::Material::MaterialModel is a way to control the appearance of an object when rendering.

For now, a Ra::Core::Material::MaterialModel only provides the _Bidirectional Scattering Distribution function (BSDF)_ to be applied on an object.

This _BSDF_ is composed of three methods.

# Methods of the Ra::Core::Material

## Evaluate BSDF

This method, wich is defined as the operator `()` by the interface in Ra::Core::Material::MaterialModel gives the color value of the object for given ingoing, outgoing and surface normal directions.

## Sample BSDF

This method, named `sample()` by the interface in Ra::Core::Material::MaterialModel gives the outgoing direction according to the choosen reflectance model for a given ingoing and surface normal direction.

A Ra::Core::Material::MaterialModel use a Ra::Core::Random::SphereSampler fed with a Ra::Core::Random::UniformGenerator in order to sample. To learn more about samplers and generators, please read the [Random](@ref develcorerandom) section.

## Probability density function corresponding to BSDF

This method, named `pdf()` by the interface in Ra::Core::Material::MaterialModel gives the probability for a pair of ingoing and outgoing direction to exist.
