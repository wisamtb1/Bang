#include "XMLProperty.h"

#include "StringUtils.h"

const XMLProperty XMLProperty::Hidden("Hidden");
const XMLProperty XMLProperty::Inline("Inline");
const XMLProperty XMLProperty::BigText("BigText");
const XMLProperty XMLProperty::Readonly("Readonly");
const XMLProperty XMLProperty::Disabled("Disabled");
const XMLProperty XMLProperty::IsEngineFile("IsEngineFile");
const XMLProperty XMLProperty::FileExtension("FileExtension");

XMLProperty::XMLProperty()
{
}

XMLProperty::XMLProperty(const String &name, const String &value) :
    m_name(name), m_value(value)
{

}

void XMLProperty::SetName(const String &name)
{
    m_name = name;
}

void XMLProperty::SetValue(const String &value)
{
    m_value = value;
}

const String &XMLProperty::GetName() const
{
    return m_name;
}

const String &XMLProperty::GetValue() const
{
    return m_value;
}

XMLProperty XMLProperty::FromString(const String &string)
{
    String propString = string;

    StringUtils::Trim(&propString);
    Array<String> parts = StringUtils::Split(propString, '=');

    String name = parts[0];

    String value = "";
    if (parts.Size() > 1)
    {
        value = parts[1];
        value = value.substr(1, value.Length() - 2); // Strip the "
    }

    return XMLProperty(name, value);
}

bool XMLProperty::operator==(const XMLProperty &rhs) const
{
    return m_name == rhs.m_name && m_value == rhs.m_value;
}
