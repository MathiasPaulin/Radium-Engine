// Include the samplers and a random generator
#include <Core/Random/BlinnPhongSphereSampler.hpp>
#include <Core/Random/CosineWeightedSphereSampler.hpp>
#include <Core/Random/MersenneTwisterGenerator.hpp>
#include <Core/Random/UniformSphereSampler.hpp>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

int main( int /*argc*/, char** /*argv*/ ) {
    using namespace Ra::Core;
    using json = nlohmann::json;

    //! [Creating the random generator to feed the samplers]
    Random::MersenneTwisterGenerator generator = Random::MersenneTwisterGenerator();

    //! [Creating the samplers]
    std::vector<Vector3> uSamplesDir, cSamplesDir, bSamplesDir4, bSamplesDir16, bSamplesDir64,
        bSamplesDir128;

    //! [Sampling (here 500 times) with each sampler]
    for ( int i = 0; i < 500; i++ ) {
        uSamplesDir.push_back( Random::UniformSphereSampler::getDir( &generator ).first );
        cSamplesDir.push_back( Random::CosineWeightedSphereSampler::getDir( &generator ).first );
        bSamplesDir4.push_back( Random::BlinnPhongSphereSampler::getDir( &generator, 4 ).first );
        bSamplesDir16.push_back( Random::BlinnPhongSphereSampler::getDir( &generator, 16 ).first );
        bSamplesDir64.push_back( Random::BlinnPhongSphereSampler::getDir( &generator, 64 ).first );
        bSamplesDir128.push_back(
            Random::BlinnPhongSphereSampler::getDir( &generator, 128 ).first );
    }

    //! [Creating a json format to write the samples in and write to a file named "samples.json"]
    json j = { { "UniformSamples", uSamplesDir },
               { "CosineWeightedSamples", cSamplesDir },
               { "BlinnPhongSamples 4", bSamplesDir4 },
               { "BlinnPhongSamples 16", bSamplesDir16 },
               { "BlinnPhongSamples 64", bSamplesDir64 },
               { "BlinnPhongSamples 128", bSamplesDir128 } };

    std::ofstream o( "samples.json" );

    o << std::setw( 4 ) << j << std::endl;

    o.close();

    //! [Samples can now be displayed with the python script named "plot-samples.py"]

    return 0;
}
