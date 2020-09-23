#pragma once
// adapted from https://github.com/paroj/nv_dds/blob/master/nv_dds.h

#include <string>
#include <deque>
#include <istream>

#include <assert.h>
#include <stdint.h>

#include <Engine/Renderer/OpenGL/OpenGL.hpp>

namespace Ra {
namespace Engine {

namespace DDS {
enum TextureType {
    TextureNone,
    TextureFlat, // 1D, 2D textures
    Texture3D,
    TextureCubemap
};


class CSurface
{
  public:
    CSurface();
    CSurface( unsigned int w,
              unsigned int h,
              unsigned int d,
              unsigned int imgsize,
              const uint8_t* pixels );
    CSurface( const CSurface& copy );
    CSurface& operator=( const CSurface& rhs );
    virtual ~CSurface();

    operator uint8_t*() const;

    virtual void create( unsigned int w,
                         unsigned int h,
                         unsigned int d,
                         unsigned int imgsize,
                         const uint8_t* pixels );
    virtual void clear();

    unsigned int get_width() const { return m_width; }
    unsigned int get_height() const { return m_height; }
    unsigned int get_depth() const { return m_depth; }
    unsigned int get_size() const { return m_size; }

  private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_depth;
    unsigned int m_size;

    uint8_t* m_pixels;
};

class CTexture : public CSurface
{
    friend class CDDSImage;

  public:
    CTexture();
    CTexture( unsigned int w,
              unsigned int h,
              unsigned int d,
              unsigned int imgsize,
              const uint8_t* pixels );
    CTexture( const CTexture& copy );
    CTexture& operator=( const CTexture& rhs );
    ~CTexture();

    void create( unsigned int w,
                 unsigned int h,
                 unsigned int d,
                 unsigned int imgsize,
                 const uint8_t* pixels );
    void clear();

    const CSurface& get_mipmap( unsigned int index ) const {
        assert( !m_mipmaps.empty() );
        assert( index < m_mipmaps.size() );

        return m_mipmaps[index];
    }

    void add_mipmap( const CSurface& mipmap ) { m_mipmaps.push_back( mipmap ); }

    unsigned int get_num_mipmaps() const { return (unsigned int)m_mipmaps.size(); }

  protected:
    CSurface& get_mipmap( unsigned int index ) {
        assert( !m_mipmaps.empty() );
        assert( index < m_mipmaps.size() );

        return m_mipmaps[index];
    }

  private:
    std::deque<CSurface> m_mipmaps;
};

class CDDSImage
{
  public:
    CDDSImage();
    ~CDDSImage();

    void
    create_textureFlat( GLenum format, unsigned int components, const CTexture& baseImage );
    void
    create_texture3D( GLenum format, unsigned int components, const CTexture& baseImage );
    void create_textureCubemap( GLenum format,
                                unsigned int components,
                                const CTexture& positiveX,
                                const CTexture& negativeX,
                                const CTexture& positiveY,
                                const CTexture& negativeY,
                                const CTexture& positiveZ,
                                const CTexture& negativeZ );

    void clear();

    void load( std::istream& is, bool flipImage = true );
    void load( const std::string& filename, bool flipImage = true );
    void save( const std::string& filename, bool flipImage = true );

    void upload_texture1D() const;
    void upload_texture2D( uint32_t imageIndex = 0, gl::GLenum target = GL_TEXTURE_2D ) const;
    void upload_texture3D() const;
    void upload_textureCubemap() const;

    operator uint8_t*() const {
        assert( m_valid );
        assert( !m_images.empty() );

        return m_images[0];
    }

    unsigned int get_width() const {
        assert( m_valid );
        assert( !m_images.empty() );

        return m_images[0].get_width();
    }

    unsigned int get_height() const {
        assert( m_valid );
        assert( !m_images.empty() );

        return m_images[0].get_height();
    }

    unsigned int get_depth() const {
        assert( m_valid );
        assert( !m_images.empty() );

        return m_images[0].get_depth();
    }

    unsigned int get_size() const {
        assert( m_valid );
        assert( !m_images.empty() );

        return m_images[0].get_size();
    }

    unsigned int get_num_mipmaps() const {
        assert( m_valid );
        assert( !m_images.empty() );

        return m_images[0].get_num_mipmaps();
    }

    const CSurface& get_mipmap( unsigned int index ) const {
        assert( m_valid );
        assert( !m_images.empty() );
        assert( index < m_images[0].get_num_mipmaps() );

        return m_images[0].get_mipmap( index );
    }

    const CTexture& get_cubemap_face( unsigned int face ) const {
        assert( m_valid );
        assert( !m_images.empty() );
        assert( m_images.size() == 6 );
        assert( m_type == TextureCubemap );
        assert( face < 6 );

        return m_images[face];
    }

    const CTexture& get_surface( unsigned int layer ) const {
        assert( m_valid );
        assert( layer < m_images.size() );
        return m_images[layer];
    }

    unsigned int get_components() const { return m_components; }
    gl::GLenum get_format() const { return m_format; }
    TextureType get_type() const { return m_type; }

    bool is_compressed() const;

    bool is_cubemap() const { return ( m_type == TextureCubemap ); }
    bool is_volume() const { return ( m_type == Texture3D ); }
    bool is_valid() const { return m_valid; }

    bool is_dword_aligned() const {
        assert( m_valid );

        int dwordLineSize = get_dword_aligned_linesize( get_width(), m_components * 8 );
        int curLineSize   = get_width() * m_components;

        return ( dwordLineSize == curLineSize );
    }

  private:
    unsigned int clamp_size( unsigned int size );
    unsigned int size_dxtc( unsigned int width, unsigned int height );
    unsigned int size_rgb( unsigned int width, unsigned int height );

    // calculates 4-byte aligned width of image
    unsigned int get_dword_aligned_linesize( unsigned int width, unsigned int bpp ) const {
        return ( ( width * bpp + 31 ) & -32 ) >> 3;
    }

    void flip( CSurface& surface );
    void flip_texture( CTexture& texture );

    void write_texture( const CTexture& texture, std::ostream& os );

    gl::GLenum m_format;
    unsigned int m_components;
    TextureType m_type;
    bool m_valid;

    std::deque<CTexture> m_images;
};
}  //namespace dds
}  // namespace Engine
}  // Ra
