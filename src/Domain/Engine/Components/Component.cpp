#include "Component.h"
#include "GameObject.h"


Component::Component()
{
}

Component::~Component()
{
    for(int i = 0; i < inspectorComponentInfo.GetSlotInfos().size(); ++i)
        delete inspectorComponentInfo.GetSlotInfo(i);
}

void Component::CloneInto(ICloneable *clone) const
{
    Component *c = static_cast<Component*>(clone);
    c->SetEnabled(enabled);
    c->owner = nullptr;
}

GameObject *Component::GetOwner() const
{
    return owner;
}

const std::string Component::ToString() const
{
    std::ostringstream msg;
    msg << "Component";
    return msg.str();
}
