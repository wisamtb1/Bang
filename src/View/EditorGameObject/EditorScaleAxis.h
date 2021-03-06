#ifndef EDITORSCALEAXIS_H
#define EDITORSCALEAXIS_H


#include "EditorAxis.h"

class Renderer;
class GameObject;
class EditorGameObject;
class SingleLineRenderer;
class EditorScaleAxis : public EditorAxis
{
public:
    EditorScaleAxis(EditorAxis::EditorAxisDirection dir, GameObject *attachedGameObject);
    virtual ~EditorScaleAxis();

    void OnUpdate() override;
    void OnDrawGizmosOverlay() override;

protected:
    virtual Renderer *GetAxisRenderer() const override;

private:
    EditorGameObject *m_axisCap = nullptr;
    SingleLineRenderer *m_line = nullptr;

     // If its the central axis, we must do little tweaks
    void ConfigureCentralAxis();
};


#endif // EDITORSCALEAXIS_H
