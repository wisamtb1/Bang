#include "Array.h"

#include <iostream>

#include <QTextStream>

#include "glm/glm.hpp"

#include "Bang.h"

#include "Input.h"
#include "Timer.h"
#include "Debug.h"
#include "Camera.h"
#include "Screen.h"
#include "Shader.h"
#include "XMLNode.h"
#include "Behaviour.h"
#include "Texture2D.h"
#include "XMLParser.h"
#include "GameObject.h"
#include "FileReader.h"
#include "Persistence.h"
#include "Application.h"
#include "Framebuffer.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "SingletonManager.h"

#ifdef BANG_EDITOR

#include "EditorScene.h"
#include "EditorWindow.h"
#include "ShortcutManager.h"
#include "EditorWindowAutoGenerated.h"

#else

#include "GameWindow.h"

#endif

#include "Array.h"
#include "String.h"

int main(int argc, char *argv[])
{
    // Init some paths:
    Persistence::c_ProjectRootAbsolute = "/home/sephirot47/Bang";
    Persistence::c_ProjectAssetsRootAbsolute = Persistence::c_ProjectRootAbsolute + "/Assets";
    Persistence::c_EngineAssetsRootAbsolute = "/home/sephirot47/Bang/EngineAssets";
    //

    SingletonManager::InitSingletonManagerFromMainBinary();
    #ifdef BANG_EDITOR
    ShortcutManager::InitFromMainBinary(); // It must go before Application init
    #endif

    Application app(argc, argv);
    QMainWindow *window = new QMainWindow();

    #ifdef BANG_EDITOR

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(8);
    app.setFont(font);

    EditorWindow::InitFromMainBinary(window, &app);

    #else

    GameWindow::InitFromMainBinary(window, &app);

    #endif

    Screen::InitFromMainBinary();
    Time::InitFromMainBinary();
    Input::InitFromMainBinary();

    #ifdef BANG_EDITOR

    // EDITOR /////////////////////////////////////
    // Load dark Window theme
    QFile f((Persistence::c_EngineAssetsRootAbsolute + "/qdarkstyle/style.qss").ToQString()
            );
    if (!f.exists())
    {
        Debug_Error("Unable to set dark stylesheet, stylesheet not found.");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }
    //

    Scene *scene = new EditorScene();
    String filename = "./Scenes/test.bscene";
    filename = Persistence::ToAbsolute(filename, false);
    FileReader::ReadScene(filename, scene);
    if (scene)
    {
        SceneManager::AddScene(scene);
        SceneManager::SetActiveScene(scene);
        Persistence::SetActiveSceneFilepath(filename);
    }

    #else

    // GAME /////////////////////////////////////
    Scene *scene = new Scene();
    String filename = "./Scenes/test.bscene";
    filename = Persistence::ToAbsolute(filename, false);
    FileReader::ReadScene(filename, scene);
    if (scene)
    {
        SceneManager::AddScene(scene);
        SceneManager::SetActiveScene(scene);
        Persistence::SetActiveSceneFilepath(filename);
    }
    #endif

    return app.exec();
}
