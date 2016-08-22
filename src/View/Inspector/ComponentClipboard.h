#ifndef COMPONENTCLIPBOARD_H
#define COMPONENTCLIPBOARD_H

#include "Component.h"

class ComponentClipboard
{
private:
    ComponentClipboard() {}

    static Component* s_copiedComponent;

public:
    static bool IsEmpty();
    static bool IsCopiedTransform();
    static String GetCopiedComponentName();
    static void CopyComponent(Component *componentToCopy);
    static void PasteComponentInto(GameObject *go);
    static void PasteComponentValuesInto(Component *comp);
};

#endif // COMPONENTCLIPBOARD_H
