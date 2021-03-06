#ifndef BANGFAB_H
#define BANGFAB_H

#include "Asset.h"
#include "String.h"

//Represents an GameObject that can be saved as an asset, and
//can be instantiated.
//In fact its asset file will be of the form
//<GameObject>
//  ...
//</GameObject>
class XMLNode;
class GameObject;
class Prefab : public Asset
{
public:
    Prefab();
    Prefab(const Prefab &p);
    Prefab(GameObject *go);
    Prefab(const String &gameObjectXMLInfoContent);

    const static String GetFileExtensionStatic();
    const virtual String GetFileExtension();

    GameObject* Instantiate() const;
    GameObject* InstantiateWithoutStarting() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

private:
    String m_gameObjectXMLInfoContent = "";
};

#endif // BANGFAB_H
