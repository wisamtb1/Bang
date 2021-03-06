#ifndef EDITORROTATEAXISGROUP_H
#define EDITORROTATEAXISGROUP_H

#include "EditorGameObject.h"

class Toolbar;
class MeshRenderer;
class EditorRotateAxis;
class EditorRotateAxisGroup : public EditorGameObject
{
private:
    EditorRotateAxis *m_axisX = nullptr;
    EditorRotateAxis *m_axisY = nullptr;
    EditorRotateAxis *m_axisZ = nullptr;
    CircleRenderer *m_circle = nullptr;

public:
    EditorRotateAxisGroup(GameObject *attachedGameObject);
    virtual ~EditorRotateAxisGroup();

    void OnUpdate() override;
    void OnDrawGizmosOverlay() override;
};

#endif // EDITORROTATEAXISGROUP_H
