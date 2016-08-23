#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <set>
#include <vector>

#include "Input.h"
#include "IShortcutListener.h"

class ShortcutManager
{
friend class Application;
friend class IShortcutListener;

private:
    std::list<IShortcutListener*> m_shortcutsListeners;
    std::set<Input::Key> m_pressedKeys;

    ShortcutManager();

    void OnKeyPressed(Input::Key key);
    void OnKeyReleased(Input::Key key);

    void RegisterListener(IShortcutListener *list);
    void UnregisterListener(IShortcutListener *list);

public:

    static void InitFromMainBinary();
    static ShortcutManager *GetInstance();

    static bool IsPressed(Input::Key key);
    static bool IsPressed(const std::vector<Input::Key> keys);

    void Clear();
};

#endif // SHORTCUTMANAGER_H