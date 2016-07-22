#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : public Light
{
protected:
    float m_range = 1.0f;

    virtual void SetUniformsBeforeApplyingLight() const override;

public:
    PointLight();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;
    #endif

    virtual void WriteInternal(std::ostream &f) const;
    virtual void ReadInternal(std::istream &f);
};

#endif // POINTLIGHT_H