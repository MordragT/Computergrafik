#include "skybox.h"

#include <QImage>

Skybox::Skybox()
{

    Q_ASSERT(initializeOpenGLFunctions());

    // VBO data (3 floats per position)
    static const GLfloat vertexData[] = {
        -1,  1, -1,
        -1, -1, -1,
         1, -1, -1,
         1,  1, -1,
        -1, -1,  1,
        -1,  1,  1,
         1, -1,  1,
         1,  1,  1,
    };

    //VBO
    // Create buffer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Copy data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    //VAO
    glGenVertexArrays(1, &m_vao);

    // Start recording buffer and attribute metadata
    glBindVertexArray(m_vao);

    // Associate earlier Vertex Buffer with this VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Stop recording
    glBindVertexArray(0);

    //IBO
    glBindVertexArray(m_vao);

    // construct and ll IBO with data
    // IBO data
    static const GLuint indicesData[] = {
        0, 1, 2, 2, 3, 0,
        4, 1, 0, 0, 5, 4,
        2, 6, 7, 7, 3, 2,
        4, 5, 7, 7, 6, 4,
        0, 3, 7, 7, 5, 0,
        1, 4, 2, 2, 4, 6,
    };

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
    glBindVertexArray(0);


    //Generate Cubemap
    glGenTextures(1, &m_cubeTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex);

    // right face
    QImage faceImg_right(":/right.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, faceImg_right.width(), faceImg_right.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<void const*>(faceImg_right.constBits()));

    // left face
    QImage faceImg_left(":/left.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, faceImg_left.width(), faceImg_left.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<void const*>(faceImg_left.constBits()));

    // top face
    QImage faceImg_top(":/top.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, faceImg_top.width(), faceImg_top.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<void const*>(faceImg_top.constBits()));

    // bottom face
    QImage faceImg_bottom(":/bottom.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, faceImg_bottom.width(), faceImg_bottom.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<void const*>(faceImg_bottom.constBits()));

    // back face
    QImage faceImg_back(":/back.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, faceImg_back.width(), faceImg_back.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<void const*>(faceImg_back.constBits()));

    // front face
    QImage faceImg_front(":/front.jpg");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, faceImg_front.width(), faceImg_front.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, reinterpret_cast<void const*>(faceImg_front.constBits()));

    // set ltering (interpolation) options
    // without these commands, _sampling will return black_
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    //Shader
    m_prog = new QOpenGLShaderProgram{};

    // addShader() compiles and attaches shader stages for linking
    m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vert");
    m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.frag");

    // now link attached stages
    m_prog->link();

    // abort program if any of these steps failed
    Q_ASSERT(m_prog->isLinked());
}

Skybox::~Skybox(){
    glDeleteTextures(1, &m_cubeTex);
}

void Skybox::render(const QMatrix4x4 &projection, QMatrix4x4 view){

    // temporarily disable writing depth values
    glDepthMask(GL_FALSE);

    // Erase translation in view matrix
    view.column(3) = QVector4D(0.0, 0.0, 0.0, 0.0);
    view.scale(10.0);

    // Bind VAO, shader, cube texture
    glBindVertexArray(m_vao);
    m_prog->bind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex);

    // Bind Program and ll uniforms
    m_prog->setUniformValue(0, projection);
    m_prog->setUniformValue(1, view);
    m_prog->setUniformValue(8, 0);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    // Release VAO
    glBindVertexArray(0);

    // Turn depth writes back on
    glDepthMask(GL_TRUE);

}
