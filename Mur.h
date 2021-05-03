#ifndef MUR_H
#define MUR_H

#include "Mesh.h"
#include <gl-matrix.h>
#include "AL/al.h"

class Mur : public Mesh
{
private:
    Material* m_Material;

    ALuint buffer, source;
    
    vec2 m_position;

    bool m_hasDoor;

    float height, width;

public:
    Mur(float height, float width);
    Mur() : Mur(1, 1){}
    ~Mur();

    /**
     * dessiner le mur
     * @param matP : matrice de projection
     * @param matMV : matrice view*model (caméra * position objet)
     */
    void onRender(const mat4& matP, const mat4& matMV);

    /**
     * retourne la position % scèce du cube
     * @return vec2 position
     */
    vec2& getPosition();

    /**
     * affecte la position % scène du cube
     * @param vec2 pos position
     */
    void setPosition(vec2 pos);

    /**
     * retourne l'ouverture ou non du mur
     * @return bool state
     */
    bool hasDoor() {
        return m_hasDoor;
    }

    /**
     * affecte l'ouverture ou non du mur
     * @param bool state hasDoor
     */
    void setDoor(bool state)
    {
        m_hasDoor = state;
    }
};


#endif