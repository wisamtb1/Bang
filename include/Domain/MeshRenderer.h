#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "VAO.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"

class MeshRenderer
{
public:
    enum DrawingMode
    {
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };

private:

    const Mesh *mesh;
    const Material *material;

    int meshVerticesPosVBOIndex;
    VAO *vao;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void Render(const Transform *t,
                MeshRenderer::DrawingMode drawingMode = MeshRenderer::DrawingMode::Triangles) const;

    void SetMesh(const Mesh *m);
    const Mesh* GetMesh() { return mesh; }

    void SetMaterial(const Material *m);
    const Material* GetMaterial() { return material; }
};

#endif // MESHRENDERER_H
