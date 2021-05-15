#ifndef V_PERSO_H
#define V_PERSO_H

#include "Mesh.h"
#include <gl-matrix.h>
#include "AL/al.h"

class VisualPerso : public Mesh
{
private:
    Material *m_Material;
    vec2 m_position;

    float height, width;

public:
    VisualPerso(float height, float width);
    VisualPerso() : VisualPerso(1, 1) {}
    ~VisualPerso();

    /**
     * dessiner le mur
     * @param matP : matrice de projection
     * @param matMV : matrice view*model (caméra * position objet)
     */
    void onRender(const mat4 &matP, const mat4 &matMV);

    /**
     * retourne la position % scèce du cube
     * @return vec2 position
     */
    vec2 &getPosition();

    /**
     * affecte la position % scène du cube
     * @param vec2 pos position
     */
    void setPosition(vec2 pos);
};

#endif