\page develcorerandom Core Random
[TOC]

The `Ra::Core::Random` namespace contain two interfaces, Ra::Core::Random::UniformGenerator and Ra::Core::Random::SphereSampler.

# Generators

A generator is an implementation of the interface Ra::Core::Random::UniformGenerator. It provides an uniform random distribution between 0 and 1. The interface provides four methods to get a random scalar or a 2D, 3D or nD vector. By default, the vectors are built by calling n times the `get1D()` method.

# Samplers

A sampler follow the interface Ra::Core::Random::SphereSampler. A sphere sampler generate a direction from the center of the sphere to a point on the surface of the sphere according to a probability distribution. To achieve this, it has two methods. A method `getDir()` that maps an uniform generator into the probability distribution to generate a direction in the canonical frame and a method `pdf()` that compute the probability density function value of a direction with a given normal direction (both directions are in world frame).

There is an example app called PlotImportanceSampler that can write samples from several samplers in a json file. The json file can then be read by a python script called plot-samples.py.
To launch this script do

~~~{.bash}
python3 /path/to/plot-samples.py /path/to/jsonfile.json
~~~
