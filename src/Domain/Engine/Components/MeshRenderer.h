#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang.h"

#include <glm/gtx/string_cast.hpp>

#include "VAO.h"
#include "Mesh.h"
#include "Component.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"
#include "Renderer.h"

#ifdef BANG_EDITOR
#include "InspectorFileSW.h"
#endif


class MeshRenderer : public Renderer
{

private:
    Mesh *m_mesh = nullptr;

protected:

    virtual void RenderWithoutBindingMaterial() const override;

public:
    MeshRenderer();
    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;
    virtual ~MeshRenderer();

    void SetMaterial(Material *m) override;
    virtual void SetMesh(Mesh *m);

    virtual Box GetBoundingBox() const override;

    virtual const Mesh* GetMesh();
    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* OnInspectorInfoNeeded() override;
    virtual void OnInspectorInfoChanged(InspectorWidgetInfo *info) override;
    #endif

    virtual void ReadXMLNode(const XMLNode *xmlNode) override;
    virtual void GetXMLNode(XMLNode *xmlNode) const override;
};

#endif // MESHRENDERER_H
