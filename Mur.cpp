//Définition de la classe Mur

#include "Mur.h"
#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

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
void Mur::onRender(const mat4 &matP, const mat4 &matMV)
{

    /** dessin OpenGL **/

    if (!m_hasDoor)
    {
        onDraw(matP, matMV);
    }

    else
    {/*
        // ouverture du flux audio à placer dans le buffer
        buffer = alutCreateBufferFromFile(soundpathname.c_str());
        if (buffer == AL_NONE)
        {
            std::cerr << "unable to open file " << soundpathname << std::endl;
            alGetError();
            throw std::runtime_error("file not found or not readable");
        }

        // lien buffer -> source
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);

        // propriétés de la source à l'origine
        alSource3f(source, AL_POSITION, - m_position[0], 0, - m_position[1]); // on positionne la source à (0,0,0) par défaut
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        alSourcei(source, AL_LOOPING, AL_TRUE);
        // dans un cone d'angle [-inner/2,inner/2] il n'y a pas d'attenuation
        alSourcef(source, AL_CONE_INNER_ANGLE, 20);
        // dans un cone d'angle [-outer/2,outer/2] il y a une attenuation linéaire entre 0 et le gain
        alSourcef(source, AL_CONE_OUTER_GAIN, 0);
        alSourcef(source, AL_CONE_OUTER_ANGLE, 80);
        // à l'extérieur de [-outer/2,outer/2] il y a une attenuation totale

        alSourcePlay(source);*/
    }
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