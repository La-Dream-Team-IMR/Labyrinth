#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <utils.h>

#include "Scene.h"
#include "labyrinthe/Case.h"
#include "labyrinthe/Labyrinthe.h"
#include "Perso.h"
#include "Mur.h"

using namespace std;

/** constructeur */
Scene::Scene()
{
    premier = true;
    lab = new Labyrinthe(3);
    v_perso = new Mur(1, 1);

    //v_lab = new VisualLab(lab);

    const auto size = lab->getSize();
    //v_cases = vector<VisualCase>(size*size);

    indexSource = 0;
    string soundpathname = "data/chouette2.wav";
    string soundMur = "data/white_noise.wav";

    std::cout << "__________" << std::endl;
    for (uint8_t i = 0; i < size; ++i)
    {
        for (uint8_t j = 0; j < size; ++j)
        {
            auto c = lab->getPosition(i, j);

            if (!c.South)
                std::cout << "_";
            else
                std::cout << " ";

            if (!c.East)
                std::cout << "|";
            else
                std::cout << " ";

            sources[indexSource] = c.South ? initSound(soundpathname, j * 4, 0, 2 + i * 4) : initSound(soundMur, j * 4, 0, 2 + i * 4);
            indexSource++;
            sources[indexSource] = c.East ? initSound(soundpathname, 2 + j * 4, 0, i * 4) : initSound(soundMur, 2 + j * 4, 0, i * 4);
            indexSource++;
        }

        std::cout << std::endl;
    }

    /*
    // créer les objets à dessiner
    m_Cube = new Cube("data/white_noise.wav");
    //m_Cube->setPosition(vec3::fromValues(0.5, 0.0, 0.0));
    m_Ground = new Ground();

    // caractéristiques de la lampe
    m_Light = new Light();
    /*m_Light->setColor(500.0, 500.0, 500.0);
    m_Light->setPosition(0.0,  16.0,  13.0, 1.0);
    m_Light->setDirection(0.0, -1.0, -1.0, 0.0);
    m_Light->setAngles(30.0, 40.0);*/

    perso = new Perso();
    /*for (int i = 0; i < 256; i++)
    {
        sources[i] = initSound("data/white_noise.wav", 0, 0, 0);
    }*/
    //sources[1] = initSound("data/white_noise.wav", 0, 0, 0);
    //sources[2] = initSound("data/white_noise.wav", 0, 0, 0);
    //sources[3] = initSound("data/white_noise.wav", 0, 0, 0);

    // couleur du fond : gris foncé
    glClearColor(0.4, 0.4, 0.4, 0.0);

    // activer le depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // initialiser les matrices
    m_MatP = mat4::create();
    m_MatV = mat4::create();
    m_MatVM = mat4::create();
    m_MatTMP = mat4::create();
    /*
    // gestion vue et souris
    m_Azimut = 0.0;
    m_Elevation = 0.0;
    m_Distance = 0.0;*/
    m_Center = vec3::create();
    m_Clicked = false;

    alListener3f(AL_POSITION, perso->pos_x * 4, 0, perso->pos_y * 4);
    alListener3f(AL_DIRECTION, 0, 0, 1);
    action();
}

/**
 * appelée quand la taille de la vue OpenGL change
 * @param width : largeur en nombre de pixels de la fenêtre
 * @param height : hauteur en nombre de pixels de la fenêtre
 */
void Scene::onSurfaceChanged(int width, int height)
{
    // met en place le viewport
    glViewport(0, 0, width, height);

    // matrice de projection (champ de vision)
    mat4::perspective(m_MatP, Utils::radians(25.0), (float)width / height, 0.1, 100.0);
}

/**
 * appelée quand on enfonce un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseDown(int btn, double x, double y)
{
    if (btn != GLFW_MOUSE_BUTTON_LEFT)
        return;
    m_Clicked = true;
    m_MousePrecX = x;
    m_MousePrecY = y;
}

/**
 * appelée quand on relache un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseUp(int btn, double x, double y)
{
    m_Clicked = false;
}

/**
 * appelée quand on bouge la souris
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseMove(double x, double y)
{
    if (!m_Clicked)
        return;
    /*
    m_Azimut += (x - m_MousePrecX) * 0.1;
    m_Elevation += (y - m_MousePrecY) * 0.1;
    if (m_Elevation > 90.0)
        m_Elevation = 90.0;
    if (m_Elevation < -90.0)
        m_Elevation = -90.0;
    m_MousePrecX = x;
    m_MousePrecY = y;
    */
}

/**
 * appelée quand on appuie sur une touche du clavier
 * @param code : touche enfoncée
 */
void Scene::onKeyDown(int code)
{
    // construire la matrice inverse de l'orientation de la vue à la souris
    mat4::identity(m_MatTMP);
    //mat4::rotateY(m_MatTMP, m_MatTMP, Utils::radians(-m_Azimut));
    //mat4::rotateX(m_MatTMP, m_MatTMP, Utils::radians(-m_Elevation));

    // vecteur indiquant le décalage à appliquer au pivot de la rotation
    vec3 offset = vec3::create();
    switch (code)
    {
    /*
    case GLFW_KEY_W: // avant
        m_Distance *= exp(-0.01);
        break;
    case GLFW_KEY_S: // arrière
        m_Distance *= exp(+0.01);
        break;
        */
    case GLFW_KEY_A: // droite
        vec3::transformMat4(offset, vec3::fromValues(+0.1, 0, 0), m_MatTMP);
        break;
    case GLFW_KEY_D: // gauche
        vec3::transformMat4(offset, vec3::fromValues(-0.1, 0, 0), m_MatTMP);
        break;
    case GLFW_KEY_S: // haut
        vec3::transformMat4(offset, vec3::fromValues(0, 0, -0.1), m_MatTMP);
        break;
    case GLFW_KEY_W: // bas
        vec3::transformMat4(offset, vec3::fromValues(0, 0, +0.1), m_MatTMP);
        break;

    case GLFW_KEY_RIGHT:
        std::cout << "Droite" << std::endl;
        actionDroite();
        action();
        break;
    case GLFW_KEY_LEFT:
        std::cout << "Gauche" << std::endl;
        actionGauche();
        action();
        break;
    case GLFW_KEY_UP:
        std::cout << "Face" << std::endl;
        actionFace();
        action();
        break;
    case GLFW_KEY_DOWN:
        std::cout << "Arrière" << std::endl;
        actionArriere();
        action();
        break;
    case GLFW_KEY_ENTER:
        action();
        break;
    default:
        return;
    }

    // appliquer le décalage au centre de la rotation
    vec3::add(m_Center, m_Center, offset);
}

/**
 * Appelé quand appuie sur touche directionnel
 */
void Scene::action()
{
    Case c = lab->getPosition(perso->pos_x, perso->pos_y);
    cout << c.North << " " << c.East << " " << c.South << " " << c.West << " " << endl << perso->pos_x << " " << perso->pos_y << endl;

    alListener3f(AL_POSITION, perso->pos_x * 4, 0, -perso->pos_y * 4);
    v_perso->setPosition(vec2::fromValues(-perso->pos_x * 4, -perso->pos_y * 4));
}

ALuint Scene::initSound(std::string soundpathname, int right, int up, int back)
{
    // ouverture du flux audio à placer dans le buffer
    ALuint buffer = alutCreateBufferFromFile(soundpathname.c_str());
    //ALuint buffer = alutCreateBufferHelloWorld();
    if (buffer == AL_NONE)
    {
        ALenum chose = alGetError();
        string truc = alutGetErrorString(alutGetError());
        std::cerr << "unable to open file " << soundpathname << truc << chose << std::endl;
        throw std::runtime_error("file not found or not readable");
    }
    ALuint source;
    // lien buffer -> source
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    // propriétés de la source à l'origine
    alSource3f(source, AL_POSITION, right, up, back); // on positionne la source à (0,0,0) par défaut
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    // dans un cone d'angle [-inner/2,inner/2] il n'y a pas d'attenuation
    alSourcef(source, AL_CONE_INNER_ANGLE, 20);
    alSourcef(source, AL_MAX_DISTANCE, 5);
    alSourcei(source, AL_DISTANCE_MODEL, AL_INVERSE_DISTANCE_CLAMPED);
    // dans un cone d'angle [-outer/2,outer/2] il y a une attenuation linéaire entre 0 et le gain
    alSourcef(source, AL_CONE_OUTER_GAIN, 0);
    alSourcef(source, AL_CONE_OUTER_ANGLE, 360);
    // à l'extérieur de [-outer/2,outer/2] il y a une attenuation totale
    return source;
}

/**
 * Appelé quand appuie sur touche droite
 */
void Scene::actionDroite()
{
    Case c = lab->getPosition(perso->pos_x, perso->pos_y);
    if (c.East)
    {
        perso->pos_x++;
    }
}
/**
* Appelé quand appuie sur touche gauche
*/
void Scene::actionGauche()
{
    Case c = lab->getPosition(perso->pos_x, perso->pos_y);
    if (c.West)
    {
        perso->pos_x--;
    }
}
/**
* Appelé quand appuie sur touche up
*/
void Scene::actionFace()
{
    Case c = lab->getPosition(perso->pos_x, perso->pos_y);
    if (c.North)
    {
        perso->pos_y--;
    }
}
/**
* Appelé quand appuie sur touche down
*/
void Scene::actionArriere()
{
    Case c = lab->getPosition(perso->pos_x, perso->pos_y);
    if (c.South)
    {
        perso->pos_y++;
    }
}

/**
 * Dessine l'image courante
 */
void Scene::onDrawFrame()
{
    /** préparation des matrices **/

    // positionner la caméra
    mat4::identity(m_MatV);

    // éloignement de la scène
    //mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.0, -m_Distance));
    mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.0, -99));
    mat4::rotateX(m_MatV, m_MatV, Utils::radians(90));

    // rotation demandée par la souris
    //mat4::rotateX(m_MatV, m_MatV, Utils::radians(m_Elevation));
    //mat4::rotateY(m_MatV, m_MatV, Utils::radians(m_Azimut));

    // centre des rotations
    mat4::translate(m_MatV, m_MatV, m_Center);

    /** gestion des lampes **/
    /*
    // calculer la position et la direction de la lampe par rapport à la scène
    m_Light->transform(m_MatV);

    // fournir position et direction en coordonnées caméra aux objets éclairés
    m_Ground->setLight(m_Light);

    /** dessin de l'image **/

    // effacer l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // dessiner le sol
    /*m_Ground->onDraw(m_MatP, m_MatV);

    m_Cube->onRender(m_MatP, m_MatV);*/

    //m_Mur->onRender(m_MatP, m_MatV);

    //m_mur->setPosition(vec2::fromValues(2,2));
    //m_mur->onRender(m_MatP, m_MatV);
    //v_lab->onRender(m_MatP, m_MatV);
    //lab->onRender(m_MatP, m_MatV);
    //VisualCase vc = VisualCase();
    //Case c = lab->getPosition(0,0);
    //vc.set(c,0,0);
    //vc.North.onRender(m_MatP, m_MatV);
    string soundpathname = "data/chouette2.wav";
    string soundMur = "data/white_noise.wav";
    if (premier)
    {
        alSourceStopv(indexSource, sources);
    }
    int size = lab->getSize();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {

            Case c = lab->getPosition(i, j);
            if (premier)
            {
            }
            Mur South = Mur(1, 4);
            South.setDoor(c.South);
            South.setPosition(vec2::fromValues(0 - j * 4, -2 - i * 4));
            Mur East = Mur(4, 1);
            East.setDoor(c.East);
            East.setPosition(vec2::fromValues(-2 - j * 4, 0 - i * 4));

            //cout << i << " " << j << " " << c.North << " " << c.East << " " << c.South << " " << c.West << endl;

            /*Mur North = Mur(1, 4);
            North.setDoor(c.North);
            North.setPosition(vec2::fromValues(0 - j * 4, 2 - i * 4));
            North.onRender(m_MatP, m_MatV);*/

            South.onRender((m_MatP), (m_MatV));

            East.onRender(m_MatP, m_MatV);

            /*Mur West = Mur(4, 1);
            West.setDoor(c.West);
            West.setPosition(vec2::fromValues(2 - j * 4, 0 - i * 4));
            West.onRender(m_MatP, m_MatV);*/
        }
    }
    if (premier)
    {
        premier = false;
        alSourcePlayv(indexSource, sources);
    }

    v_perso->onRender(m_MatP, m_MatV);
}

/** supprime tous les objets de cette scène */
Scene::~Scene()
{
    //delete m_Cube;
    //delete m_Ground;
    delete lab;
    delete v_perso;
    alDeleteSources(256, sources);
    //delete m_Case;
}
