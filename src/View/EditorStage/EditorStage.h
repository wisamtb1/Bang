#ifndef EDITORSTAGE_H
#define EDITORSTAGE_H

#include "Stage.h"
#include "EditorCamera.h"

class EditorStage : public Stage
{
private:
    EditorCamera *cameraEntity = nullptr;

public:
    EditorStage();
    virtual ~EditorStage();
};

#endif // EDITORSTAGE_H