#ifndef SCENE_H
#define SCENE_H

#include "Bang.h"

#include <list>
#include <string>

#include "Logger.h"
#include "GameObject.h"
#include "Camera.h"
#include "Canvas.h"
#include "GBuffer.h"
#include "MeshScreenPlane.h"
#include "Material.h"

class Canvas;
class Scene : public GameObject
{
friend class Canvas;

protected:

    //Framebuffer for positions, normals, uvs and diffuse
    GBuffer *gbuffer = nullptr;

    const Canvas *canvas = nullptr; //set by canvas when added to it
    const GameObject *cameraGameObject = nullptr;

    void _OnResize(int newWidth, int newHeight);

public:
    const static std::string GetFileExtension() { return "bscene"; }

    Scene();
    virtual ~Scene();

    void _OnRender() override;

    void SetCamera(const Camera *cam);

    Camera* GetCamera() const;
    const Canvas* GetCanvas() const;

    #ifdef BANG_EDITOR
    void Write(std::ostream &f) const override;
    #endif
};

#endif // SCENE_H
