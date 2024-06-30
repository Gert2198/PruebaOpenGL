#include "Texture.h"

#include <stb/stb_image.h>

Texture::Texture(const std::string &path) : m_RendererID(0), m_Path(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) {
    
    stbi_set_flip_vertically_on_load(1); 
    // OpenGL tiene establecido el primer pixel como el de abajo a la izquierda (bottom left)
    // Si el primer pixel de la imagen es el de arriba a la izquierda y no usamos esta funcion, este pixel se dibujaría abajo a la izquierda,
    // dando como resultado que la imagen este invertida verticalmente. Esto depende de la forma en la que se almacene la imagen

    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // El 4 hace referencia a los canales que queremos para cada pixel.
    // En nuestro caso queremos RGBA, por lo que el numero de canales tiene que ser 4

    GLDebug(glGenTextures(1, &m_RendererID));
    if (m_repetitions.find(m_RendererID) == m_repetitions.end())
        m_repetitions[m_RendererID] = 1;
    else 
        m_repetitions[m_RendererID]++;
    
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // El filtro que le aplicamos cuando minimizamos la imagen
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // Lo mismo pero cuando ampliamos
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // Wrap horizontal, que no extienda el error?
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // Wrap vertical

    GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
    
    // Libero la imagen en el destructor
    
}
Texture::Texture(const Texture& texture) {
    m_RendererID = texture.m_RendererID;
    m_Width = texture.m_Width;
    m_Height = texture.m_Height;
    m_BPP = texture.m_BPP;
    m_LocalBuffer = texture.m_LocalBuffer;
    m_Path = texture.m_Path;
    m_repetitions[m_RendererID]++;
}
Texture::~Texture() {
    m_repetitions[m_RendererID]--;
    if (m_repetitions[m_RendererID] == 0) {
        if (m_LocalBuffer) stbi_image_free(m_LocalBuffer); // Puede dar lugar a errores, CUIDAO
        GLDebug(glDeleteTextures(1, &m_RendererID));
    }
}

void Texture::bind(unsigned int slot) const {
    if (m_boundTexture != m_RendererID) {
        if (m_activeSlot != slot) {
            GLDebug(glActiveTexture(GL_TEXTURE0 + slot));
            m_activeSlot = slot;
        }
        GLDebug(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        m_boundTexture = m_RendererID;
    }
}
void Texture::unbind() const {
    if (m_boundTexture != 0) {
        GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
        m_boundTexture = 0;
    }
}