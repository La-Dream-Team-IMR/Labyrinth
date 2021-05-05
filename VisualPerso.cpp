//Définition de la classe Mur

#include "VisualPerso.h"
#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

class MaterialPerso : public Material
{
public:
    MaterialPerso() : Material("MatMur")
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
VisualPerso::VisualPerso(float height, float width) : Mesh("VisualPerso")
{
    this->height = height;
    this->width = width;

    m_Material = new MaterialPerso();
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
    P0->setColor(1.0, 0.75, 0.79);  // P0 rose
    P1->setColor(0.58, 0.0, 0.827); // P1 violet
    P2->setColor(0.0, 0.75, 1.0);   // P2 bleu ciel
    P3->setColor(0.0, 0.0, 1.0);    // P3 bleu foncé

    // quads
    addQuad(P1, P0, P3, P2);
    //addQuad(P2, P3, P0, P1);
}

/**
     * dessiner le mur
     * @param matP : matrice de projection
     * @param matMV : matrice view*model (caméra * position objet)
     */
void VisualPerso::onRender(const mat4 &matP, const mat4 &matMV)
{

    /** dessin OpenGL **/
    mat4 out;
    mat4::translate(out, matMV, vec3::fromValues(-m_position[0], 0, -m_position[1]));

    mat4::rotateY(out, out, Utils::radians(10) * Utils::getTime() * 10);
    onDraw(matP, out);
}

/**
     * retourne la position % scèce du cube
     * @return vec2 position
     */
vec2 &VisualPerso::getPosition()
{
    return m_position;
}

/**
     * affecte la position % scène du cube
     * @param vec2 pos position
     */
void VisualPerso::setPosition(vec2 pos)
{
    vec2::copy(m_position, pos);
}

VisualPerso::~VisualPerso()
{
    // liberation du materiau
    delete m_Material;
}