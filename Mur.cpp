//Définition de la classe Mur

#include "Mur.h"
#include <iostream>

class MaterialMur : public Material
{
public:
    MaterialMur() : Material("MatMur")
    {
        // vertex shader
        std::string srcVertexShader =
            "#version 300 es\n"
            "uniform mat4 matP;\n"
            "uniform mat4 matVM;\n"
            "in vec3 glVertex;\n"
            "in vec3 glColor;\n"
            "out vec3 frgColor;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = matP * matVM * vec4(glVertex, 1.0);\n"
            "    frgColor = glColor;\n"
            "}";

        // fragment shader
        std::string srcFragmentShader =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec3 frgColor;\n"
            "out vec4 glFragColor;\n"
            "void main()\n"
            "{\n"
            "    glFragColor = vec4(frgColor, 1.0);\n"
            "}";

        setShaders(srcVertexShader, srcFragmentShader);
    }
};

/** constructeur */
Mur::Mur(float height, float width) : Mesh("Mur")
{
    this->height = height;
    this->width = width;
    this->m_hasDoor = false;

    m_Material = new MaterialMur();
    setMaterials(m_Material);

    //Material ?

    // sommets
    /*
        (-a, +b) p3 ____ p0 (+a, +b)
        (-a, -b) p2/___/ p1 (+a, -b)
    */
    const float a = -width / 2.0f; //- à cause du repère xyz
    const float b = -height / 2.0f;

    Vertex *P0 = new Vertex(this, +a, 0.0f, +b);
    Vertex *P1 = new Vertex(this, +a, 0.0f, -b);
    Vertex *P2 = new Vertex(this, -a, 0.0f, -b);
    Vertex *P3 = new Vertex(this, -a, 0.0f, +b);

    // couleurs
    P0->setColor(0.0, 0.5, 1.0); // P0 bleu
    P1->setColor(0.0, 0.5, 1.0); // P1 bleu
    P2->setColor(0.0, 0.5, 1.0); // P2 bleu
    P3->setColor(0.0, 0.5, 1.0); // P3 bleu

    // quads
    addQuad(P1, P0, P3, P2);
    addQuad(P2, P3, P0, P1);
}

/**
     * dessiner le mur
     * @param matP : matrice de projection
     * @param matMV : matrice view*model (caméra * position objet)
     */
void Mur::onRender(const mat4 &matP, const mat4 &matMV)
{

    /** dessin OpenGL **/

    if (!m_hasDoor)
    {
        onDraw(matP, matMV);
    }

    // Son ?
}

/**
     * retourne la position % scèce du cube
     * @return vec2 position
     */
vec2 &Mur::getPosition()
{
    return m_position;
}

/**
     * affecte la position % scène du cube
     * @param vec2 pos position
     */
void Mur::setPosition(vec2 pos)
{
    vec2::copy(m_position, pos);

    const float a = -width / 2.0f; //- à cause du repère xyz
    const float b = -height / 2.0f;

    getVertexList().at(0)->setCoords(+a - m_position[0], 0.0f, +b - m_position[1]);
    getVertexList().at(1)->setCoords(+a - m_position[0], 0.0f, -b - m_position[1]);
    getVertexList().at(2)->setCoords(-a - m_position[0], 0.0f, -b - m_position[1]);
    getVertexList().at(3)->setCoords(-a - m_position[0], 0.0f, +b - m_position[1]);
}

Mur::~Mur()
{
    // liberation du materiau
    delete m_Material;

    // libération des ressources openal
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}