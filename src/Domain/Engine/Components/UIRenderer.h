#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Color.h"
#include "String.h"
#include "Matrix4.h"
#include "MeshRenderer.h"

class XMLNode;
class Material;
class ICloneable;
class UIRenderer : public MeshRenderer
{
public:
    virtual void RenderCustomPR() const override;
    bool IsACanvasRenderer() const override;

    virtual void Render() const override;

    virtual void SetMatricesUniforms(
            Material *mat,
            const Matrix4 &model      = Matrix4::Identity,
            const Matrix4 &normal     = Matrix4::Identity,
            const Matrix4 &view       = Matrix4::Identity,
            const Matrix4 &projection = Matrix4::Identity,
            const Matrix4 &pvm        = Matrix4::Identity) const override;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    virtual String GetName() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    Material *m_materialPR = nullptr;
    Color m_tint = Color::Zero;
    Color m_strokeColor = Color::White;
    float m_stroke = 0.0f;

    UIRenderer();
    virtual ~UIRenderer();

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    friend class GameObject;
};

#endif // UIRENDERER_H
