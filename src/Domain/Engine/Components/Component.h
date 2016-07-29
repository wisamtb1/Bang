#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang.h"

#include "Logger.h"
#include "IFileable.h"
#include "IToString.h"
#include "ICloneable.h"
#include "FileWriter.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "InspectorWidget.h"
#include "IWindowEventManagerListener.h"
#endif

#define CAN_USE_COMPONENT(comp)  ( comp  && comp->IsEnabled() )

class GameObject;

class Component :
              public ISceneEventListener
             ,public IToString
             ,public IFileable
             ,public ICloneable
            #ifdef BANG_EDITOR
             ,public IWindowEventManagerListener
             ,public IInspectable
            #endif
{
friend class GameObject;
protected:
    bool m_enabled = true;

    Component();
    virtual ~Component();

public:

    GameObject *m_gameObject = nullptr;
    GameObject* const& gameObject = m_gameObject;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override = 0;

    virtual const std::string ToString() const override;

    virtual std::string GetName() const;

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // COMPONENT_H
