#include "AttrWidgetVectorFloat.h"

AttrWidgetVectorFloat::AttrWidgetVectorFloat(const XMLAttribute &xmlAttribute,
                                             InspectorWidget *inspectorWidget) :
    AttributeWidget(xmlAttribute, inspectorWidget, false, true, true)
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    m_layout->addLayout(hLayout, 1);
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    int numberOfFields = xmlAttribute.GetNumberOfFieldsOfType();
    for (unsigned int i = 0; i < numberOfFields; ++i)
    {
        AttrWidgetFloat *s = new AttrWidgetFloat(xmlAttribute, inspectorWidget, true);
        m_floatSlots.push_back(s);
        hLayout->addWidget(s, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    setMinimumHeight(40);
    AfterConstructor();
}

void AttrWidgetVectorFloat::SetValue(const std::vector<float> &v)
{
    for (unsigned int i = 0; i < m_floatSlots.size(); ++i)
    {
        m_floatSlots[i]->SetValue(v[i]);
    }
}

std::vector<float>  AttrWidgetVectorFloat::GetValue()
{
    std::vector<float> result;
    for (unsigned int i = 0; i < m_floatSlots.size(); ++i)
    {
        float f = m_floatSlots[i]->GetValue();
        result.push_back(f);
    }
    return result;
}

float AttrWidgetVectorFloat::GetFloat() const
{
    return m_floatSlots[0]->GetValue();
}

Vector2 AttrWidgetVectorFloat::GetVector2() const
{
    return Vector2(m_floatSlots[0]->GetValue(),
                     m_floatSlots[1]->GetValue());
}

Vector3 AttrWidgetVectorFloat::GetVector3() const
{
    return Vector3(m_floatSlots[0]->GetValue(),
                   m_floatSlots[1]->GetValue(),
                   m_floatSlots[2]->GetValue());
}

Vector4 AttrWidgetVectorFloat::GetVector4() const
{
    return Vector4(m_floatSlots[0]->GetValue(),
                   m_floatSlots[1]->GetValue(),
                   m_floatSlots[2]->GetValue(),
                   m_floatSlots[3]->GetValue());
}

void AttrWidgetVectorFloat::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    XMLAttribute::Type attrType = attribute.GetType();

    std::vector<float> vf;
    if (attrType == XMLAttribute::Type::Float)
    {
        float v = attribute.GetFloat();
        vf = {v};
    }
    else if (attrType == XMLAttribute::Type::Vector2)
    {
        Vector2 v = attribute.GetVector2();
        vf = {v.x, v.y};
    }
    else if (attrType == XMLAttribute::Type::Vector3)
    {
        Vector3 v = attribute.GetVector3();
        vf = {v.x, v.y, v.z};
    }
    else if (attrType == XMLAttribute::Type::Vector4 ||
             attrType == XMLAttribute::Type::Quaternion ||
             attrType == XMLAttribute::Type::Rect)
    {
        Vector4 v = attribute.GetVector4();
        vf = {v.x, v.y, v.z, v.w};
    }

    SetValue(vf);
}
