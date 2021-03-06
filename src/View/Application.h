#ifndef APPLICATION_H
#define APPLICATION_H

#include <QTimer>
#include <iostream>
#include <QKeyEvent>
#include <QApplication>

#include "Map.h"
#include "Array.h"

class Scene;
class SceneManager;
class AssetsManager;
class ShortcutManager;
class BehaviourManager;
class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);

    AssetsManager *GetAssetsManager() const;
    static Application *GetInstance();

    bool notify(QObject *receiver, QEvent *e) override;


public slots:
    void OnDrawTimerTick();

private:
    SceneManager *m_sceneManager = nullptr;
    AssetsManager *m_assetsManager = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

    unsigned long long m_lastRenderTime = 0;
    QTimer m_drawTimer;

    // If this is too low, Qt stops it when dragging
    // something around the window :(
    const int c_redrawDelay = 16; // 60 FPS cap
    //const int c_redrawDelay = 35; // 30 FPS cap

    // To detect when it's autorepeat (very fast Release after a Press)
    struct LastKeyEventInfo
    {
        unsigned long long time;
        int key;
    };
    LastKeyEventInfo m_lastKeyPressEvInfo;

private slots:
    bool CurrentKeyReleaseIsAutoRepeat(const QKeyEvent *keyReleaseEvent);


    friend class SceneManager;
    friend class ShortcutManager;
    friend class BehaviourManager;
    friend class IShortcutListener;
};

#endif // APPLICATION_H
