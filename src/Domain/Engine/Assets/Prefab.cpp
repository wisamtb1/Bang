#include "Prefab.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const Prefab &p)
{
    m_gameObjectXMLInfoContent = p.m_gameObjectXMLInfoContent;
}

Prefab::Prefab(GameObject *go)
{
    if (go)
    {
        m_gameObjectXMLInfoContent = go->GetXMLInfoString();
    }
}

Prefab::Prefab(const String &gameObjectXMLInfoContent)
{
    m_gameObjectXMLInfoContent = gameObjectXMLInfoContent;
}

GameObject *Prefab::Instantiate() const
{
    GameObject *go = InstantiateWithoutStarting();
    if (go)
    {
        go->_OnStart();
    }
    return go;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if (m_gameObjectXMLInfoContent != "")
    {
        GameObject *go = new GameObject();
        XMLNode *xmlInfo = XMLParser::FromString(m_gameObjectXMLInfoContent);
        go->ReadXMLInfo(xmlInfo);
        return go;
    }
    return nullptr;
}

void Prefab::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);
    m_gameObjectXMLInfoContent = xmlInfo->ToString();
}

void Prefab::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);

    XMLNode *goInfo = XMLParser::FromString(m_gameObjectXMLInfoContent);
    goInfo->CloneInto(xmlInfo);
    xmlInfo->SetTagName("Prefab");
}
