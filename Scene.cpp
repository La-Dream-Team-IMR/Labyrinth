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
#include "VisualPerso.h"

using namespace std;

/** constructeur */
Scene::Scene()
{
    premier = true;
    lab = new Labyrinthe(3);
    v_perso = new VisualPerso(1, 1);

    const auto size = lab->getSize();

    indexSource = 0;
    string soundpathname = "data/chouette2.wav";

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

            if (c.South)
            {
                sources[indexSource] = initSound(soundpathname, i * 50, 0, 25 + j * 50);
                indexSource++;
            }
            if (c.East)
            {
                sources[indexSource] = initSound(soundpathname, 25 + i * 50, 0, j * 50);
                indexSource++;
            }
        }

        std::cout << std::endl;
    }

    perso = new Perso();

    alListener3f(AL_POSITION, perso->pos_x * 30, 0, perso->pos_y * 30);
    ALfloat orientation[] = {0, 0, -1, 0, 1, 0};
    alListenerfv(AL_DIRECTION, orientation);
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

    m_Center = vec3::create();
    m_Clicked = false;

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
}

/**
 * appelée quand on appuie sur une touche du clavier
 * @param code : touche enfoncée
 */
void Scene::onKeyDown(int code)
{
    // construire la matrice inverse de l'orientation de la vue à la souris
    mat4::identity(m_MatTMP);

    // vecteur indiquant le décalage à appliquer au pivot de la rotation
    vec3 offset = vec3::create();
    switch (code)
    {
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
        spectateur = !spectateur;
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
    cout << c.North << " " << c.East << " " << c.South << " " << c.West << " " << endl
         << perso->pos_x << " " << perso->pos_y << endl;
    int x = perso->pos_x;
    int y = perso->pos_y;

    alListener3f(AL_POSITION, x * 50, 0, y * 50);
    v_perso->setPosition(vec2::fromValues(-x * 4, -y * 4));
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
    alSource3f(source, AL_POSITION, right, up, back);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcef(source, AL_MAX_GAIN, 1);
    alSourcef(source, AL_MIN_GAIN, 0);
    alSourcef(source, AL_MAX_DISTANCE, 12);
    alSourcef(source, AL_REFERENCE_DISTANCE, 0);
    alSourcef(source, AL_ROLLOFF_FACTOR, 1000);
    alSourcei(source, AL_DISTANCE_MODEL, AL_EXPONENT_DISTANCE_CLAMPED);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (spectateur)
    {
        /** préparation des matrices **/

        // positionner la caméra
        mat4::identity(m_MatV);

        // éloignement de la scène
        //mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.0, -m_Distance));
        mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.0, -99));
        mat4::rotateX(m_MatV, m_MatV, Utils::radians(90));

        // centre
        mat4::translate(m_MatV, m_MatV, m_Center);

        int size = lab->getSize();
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {

                Case c = lab->getPosition(i, j);
                Mur South = Mur(0.25f, 4);
                South.setDoor(c.South);
                South.setPosition(vec2::fromValues(0 - i * 4, -2 - j * 4));

                Mur East = Mur(4, 0.25f);
                East.setDoor(c.East);
                East.setPosition(vec2::fromValues(-2 - i * 4, 0 - j * 4));

                //cout << i << " " << j << " " << c.North << " " << c.East << " " << c.South << " " << c.West << endl;

                Mur North = Mur(0.25f, 4);
                North.setDoor(c.North);
                North.setPosition(vec2::fromValues(0 - i * 4, 2 - j * 4));

                Mur West = Mur(4, 0.25f);
                West.setDoor(c.West);
                West.setPosition(vec2::fromValues(2 - i * 4, 0 - j * 4));

                West.onRender(m_MatP, m_MatV);
                North.onRender(m_MatP, m_MatV);
                South.onRender((m_MatP), (m_MatV));
                East.onRender(m_MatP, m_MatV);
            }
        }

        v_perso->onRender(m_MatP, m_MatV);
    }
    if (premier)
    {
        premier = false;
        alSourcePlayv(indexSource, sources);
    }
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
