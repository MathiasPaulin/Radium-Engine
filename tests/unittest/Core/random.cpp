#include <Core/Math/Math.hpp>
#include <Core/Random/BlinnPhongSphereSampler.hpp>
#include <Core/Random/CosineWeightedSphereSampler.hpp>
#include <Core/Random/UniformGenerator.hpp>
#include <Core/Random/UniformSphereSampler.hpp>
#include <Core/Types.hpp>

#include <catch2/catch.hpp>

using namespace Ra::Core;

/// @brief Fake random generator class to test the samplers
class FakeGenerator : public Random::UniformGenerator
{
  public:
    FakeGenerator() {};
    ~FakeGenerator() {};

    Scalar get1D() override { return m_1D; }

    Vector2 get2D() override { return m_2D; }

    Vector3 get3D() override { return m_3D; }

    VectorN getXD( int dim ) override { return m_XD; }

    void set1D( Scalar s ) { m_1D = s; }
    void set2D( Vector2 v ) { m_2D = v; }
    void set3D( Vector3 v ) { m_3D = v; }
    void setXD( VectorN v ) { m_XD = v; }

  private:
    Scalar m_1D;
    Vector2 m_2D;
    Vector3 m_3D;
    VectorN m_XD;
};

Vector3 uniformSamplerGetDir( FakeGenerator* fg ) {
    Vector3 dir;
    Vector2 u = fg->get2D();

    Scalar cosTheta = std::cos( u[0] );
    Scalar sinTheta = std::cos( Math::Pi / 2_ra - u[0] );
    Scalar phi      = 2 * Math::Pi * u[1];

    dir[0] = sinTheta * std::cos( phi );
    dir[1] = sinTheta * std::sin( phi );
    dir[2] = cosTheta;

    return dir;
}

Vector3 cosineWeightedSamplerGetDir( FakeGenerator* fg ) {
    Vector3 dir;
    Vector2 u = fg->get2D();

    Scalar cosTheta = std::cos( std::sqrt( u[0] ) );
    Scalar sinTheta = std::cos( Math::Pi / 2_ra - std::sqrt( u[0] ) );
    Scalar phi      = 2 * Math::Pi * u[1];

    dir[0] = sinTheta * std::cos( phi );
    dir[1] = sinTheta * std::sin( phi );
    dir[2] = cosTheta;

    return dir;
}

Vector3 blinnPhongSamplerGetDir( FakeGenerator* fg, Scalar shininess ) {
    Vector3 dir;
    Vector2 u = fg->get2D();

    Scalar cosTheta = std::pow( 1_ra - u[0], 1_ra / ( shininess + 2_ra ) );
    Scalar sinTheta = std::sqrt( 1_ra - cosTheta * cosTheta );
    Scalar phi      = 2 * Math::Pi * u[1];

    dir[0] = sinTheta * std::cos( phi );
    dir[1] = sinTheta * std::sin( phi );
    dir[2] = cosTheta;

    return dir;
}

Scalar blinnPhongPdf( Vector3 w_i, Vector3 w_o, Vector3 normal, Scalar shininess ) {
    w_i.normalize();
    w_o.normalize();

    Vector3 halfway = ( w_i + w_o ).normalized();
    Scalar cosTheta = normal.dot( halfway );

    return ( shininess + 2_ra ) * std::pow( cosTheta, shininess ) / ( 2_ra * Math::Pi );
}

Vector3 blinnPhongReflect( Vector3 inDir, Vector3 normal ) {
    inDir.normalize();
    normal.normalize();
    return ( -inDir + 2 * inDir.dot( normal ) * normal ).normalized();
}

TEST_CASE( "Core/Random/UniformSphereSampler" ) {
    SECTION( "getDir" ) {
        auto fg = FakeGenerator();
        fg.set2D( { 0_ra, 0_ra } );
        auto p        = Random::UniformSphereSampler::getDir( &fg );
        Vector3 d     = p.first;
        Vector3 dtest = uniformSamplerGetDir( &fg );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );

        fg.set2D( { 1_ra, 1_ra } );
        p     = Random::UniformSphereSampler::getDir( &fg );
        d     = p.first;
        dtest = uniformSamplerGetDir( &fg );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );

        fg.set2D( { 0.5_ra, 0.5_ra } );
        p     = Random::UniformSphereSampler::getDir( &fg );
        d     = p.first;
        dtest = uniformSamplerGetDir( &fg );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );
    }

    SECTION( "pdf" ) {
        Vector3 normal, dir;
        Scalar usspdf;

        normal = { 0_ra, 0_ra, 1_ra };
        dir    = { 0_ra, 0_ra, 1_ra };
        usspdf = Random::UniformSphereSampler::pdf( dir, normal );
        REQUIRE( Math::areApproxEqual( usspdf, 1_ra / ( 2_ra * Math::Pi ) ) );

        dir    = { 0_ra, 1_ra, 0_ra };
        usspdf = Random::UniformSphereSampler::pdf( dir, normal );
        REQUIRE( Math::areApproxEqual( usspdf, 1_ra / ( 2_ra * Math::Pi ) ) );

        dir    = { 1_ra, 0_ra, 0_ra };
        usspdf = Random::UniformSphereSampler::pdf( dir, normal );
        REQUIRE( Math::areApproxEqual( usspdf, 1_ra / ( 2_ra * Math::Pi ) ) );
    }
}

TEST_CASE( "Core/Random/CosineWeightedSphereSampler" ) {
    SECTION( "getDir" ) {
        auto fg = FakeGenerator();
        fg.set2D( { 0_ra, 0_ra } );
        auto p        = Random::CosineWeightedSphereSampler::getDir( &fg );
        Vector3 d     = p.first;
        Vector3 dtest = cosineWeightedSamplerGetDir( &fg );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );

        fg.set2D( { 1_ra, 1_ra } );
        p     = Random::CosineWeightedSphereSampler::getDir( &fg );
        d     = p.first;
        dtest = cosineWeightedSamplerGetDir( &fg );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );

        fg.set2D( { 0.5_ra, 0.5_ra } );
        p     = Random::CosineWeightedSphereSampler::getDir( &fg );
        d     = p.first;
        dtest = cosineWeightedSamplerGetDir( &fg );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );
    }

    SECTION( "pdf" ) {
        Vector3 normal, dir;
        Scalar usspdf;

        normal = { 0_ra, 0_ra, 1_ra };
        dir    = { 0_ra, 0_ra, 1_ra };
        usspdf = Random::CosineWeightedSphereSampler::pdf( dir, normal );
        REQUIRE( Math::areApproxEqual( usspdf, 1_ra / Math::Pi ) );

        dir    = { 0_ra, 1_ra, 0_ra };
        usspdf = Random::CosineWeightedSphereSampler::pdf( dir, normal );
        REQUIRE( Math::areApproxEqual( usspdf, 0_ra ) );

        dir    = { 1_ra, 0_ra, 0.3_ra };
        usspdf = Random::CosineWeightedSphereSampler::pdf( dir, normal );
        REQUIRE( Math::areApproxEqual( usspdf, dir.normalized().dot( normal ) / Math::Pi ) );
    }
}

TEST_CASE( "Core/Random/BlinnPhongSphereSampler" ) {
    SECTION( "getDir" ) {
        auto fg = FakeGenerator();
        fg.set2D( { 0_ra, 0_ra } );
        auto p        = Random::BlinnPhongSphereSampler::getDir( &fg, 8_ra );
        Vector3 d     = p.first;
        Vector3 dtest = blinnPhongSamplerGetDir( &fg, 8_ra );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );

        fg.set2D( { 1_ra, 1_ra } );
        p     = Random::BlinnPhongSphereSampler::getDir( &fg, 32_ra );
        d     = p.first;
        dtest = blinnPhongSamplerGetDir( &fg, 32_ra );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );

        fg.set2D( { 0.5_ra, 0.5_ra } );
        p     = Random::BlinnPhongSphereSampler::getDir( &fg, 128_ra );
        d     = p.first;
        dtest = blinnPhongSamplerGetDir( &fg, 128_ra );
        REQUIRE( Math::areApproxEqual( d.x(), dtest.x() ) );
        REQUIRE( Math::areApproxEqual( d.y(), dtest.y() ) );
        REQUIRE( Math::areApproxEqual( d.z(), dtest.z() ) );
    }

    SECTION( "pdf" ) {
        Vector3 normal;
        Scalar pdf, testpdf;

        normal = { 0_ra, 0_ra, 1_ra };
        pdf    = Random::BlinnPhongSphereSampler::pdf(
            { 0_ra, 0_ra, 1_ra },
            Random::BlinnPhongSphereSampler::reflect( { 0_ra, 0_ra, 1_ra }, normal ),
            normal,
            8_ra );
        testpdf =
            blinnPhongPdf( { 0_ra, 0_ra, 1_ra },
                           Random::BlinnPhongSphereSampler::reflect( { 0_ra, 0_ra, 1_ra }, normal ),
                           normal,
                           8_ra );
        REQUIRE( Math::areApproxEqual( pdf, testpdf ) );

        pdf = Random::BlinnPhongSphereSampler::pdf(
            { 0_ra, 1_ra, 0_ra }, { 0.02_ra, -0.97_ra, 0.01_ra }, normal, 4_ra );
        testpdf =
            blinnPhongPdf( { 0_ra, 1_ra, 0_ra }, { 0.02_ra, -0.97_ra, 0.01_ra }, normal, 4_ra );
        REQUIRE( Math::areApproxEqual( pdf, testpdf ) );

        pdf = Random::BlinnPhongSphereSampler::pdf(
            { 1_ra, 0_ra, 0.5_ra }, { -1.01_ra, 0.03_ra, 0.49_ra }, normal, 32_ra );
        testpdf =
            blinnPhongPdf( { 1_ra, 0_ra, 0.5_ra }, { -1.01_ra, 0.03_ra, 0.49_ra }, normal, 32_ra );
        REQUIRE( Math::areApproxEqual( pdf, testpdf ) );
    }

    SECTION( "reflect" ) {
        Vector3 normal = { 0_ra, 0_ra, 1_ra };

        Vector3 reflected =
            Random::BlinnPhongSphereSampler::reflect( { 0_ra, 0_ra, -1_ra }, normal );
        Vector3 testreflect = blinnPhongReflect( { 0_ra, 0_ra, -1_ra }, normal );
        REQUIRE( Math::areApproxEqual( reflected.x(), testreflect.x() ) );
        REQUIRE( Math::areApproxEqual( reflected.y(), testreflect.y() ) );
        REQUIRE( Math::areApproxEqual( reflected.z(), testreflect.z() ) );

        reflected   = Random::BlinnPhongSphereSampler::reflect( { 1_ra, 0_ra, 0_ra }, normal );
        testreflect = blinnPhongReflect( { 1_ra, 0_ra, 0_ra }, normal );
        REQUIRE( Math::areApproxEqual( reflected.x(), testreflect.x() ) );
        REQUIRE( Math::areApproxEqual( reflected.y(), testreflect.y() ) );
        REQUIRE( Math::areApproxEqual( reflected.z(), testreflect.z() ) );

        reflected = Random::BlinnPhongSphereSampler::reflect( { 0.5_ra, 0.5_ra, -0.5_ra }, normal );
        testreflect = blinnPhongReflect( { 0.5_ra, 0.5_ra, -0.5_ra }, normal );
        REQUIRE( Math::areApproxEqual( reflected.x(), testreflect.x() ) );
        REQUIRE( Math::areApproxEqual( reflected.y(), testreflect.y() ) );
        REQUIRE( Math::areApproxEqual( reflected.z(), testreflect.z() ) );
    }
}
