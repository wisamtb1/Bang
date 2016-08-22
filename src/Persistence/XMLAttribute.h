#ifndef XMLATTRIBUTE_H
#define XMLATTRIBUTE_H

#include "Bang.h"


#include <vector>
#include <iostream>
#include <functional>

#include "Rect.h"
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "IToString.h"
#include "Quaternion.h"
#include "StringUtils.h"
#include "Persistence.h"
#include "XMLProperty.h"

#include "IAttrWidgetButtonListener.h"

class XMLAttribute : public IToString
{
public:

    NamedEnum (Type,
        Bool = 0,
        Int,
        String,
        Float,
        Vector2,
        Vector3,
        Vector4,
        Color,
        Quaternion,
        Rect,
        File,
        Enum,
        Button
    );

private:
    String m_name = "";
    XMLAttribute::Type m_type = XMLAttribute::Type::Bool;
    String m_value = "";
    std::vector<XMLProperty> m_properties;

public:
    XMLAttribute();
    XMLAttribute(const String &name,
                 const String &value,
                 XMLAttribute::Type type,
                 const std::vector<XMLProperty> &properties = {});

    void Set(const String &name,
             const String &value,
             XMLAttribute::Type type,
             const std::vector<XMLProperty> &properties = {});
    void SetName(const String &name);
    void SetType(const XMLAttribute::Type &type);
    void SetValue(const String &value);
    void SetProperty(const XMLProperty &property);
    void SetProperty(const String &propertyName, const String &propertyValue = "");
    void SetProperties(const std::vector<XMLProperty>& properties);
    const String GetPropertyValue(const String &propertyName) const;
    bool HasProperty(const XMLProperty &property) const;
    bool HasProperty(const String &propertyName) const;
    void RemoveProperty(const String &propertyName);


    void SetPointer(const void *value, const std::vector<XMLProperty>& properties = {});
    void SetBool(bool value, const std::vector<XMLProperty>& properties = {});
    void SetInt(int value, const std::vector<XMLProperty>& properties = {});
    void SetString(const String &value, const std::vector<XMLProperty>& properties = {});
    void SetFloat(float value, const std::vector<XMLProperty>& properties = {});
    void SetVector2(const Vector2 &value, const std::vector<XMLProperty>& properties = {});
    void SetVector3(const Vector3 &value, const std::vector<XMLProperty>& properties = {});
    void SetVector4(const Vector4 &value, const std::vector<XMLProperty>& properties = {});
    void SetColor(const Color &value, const std::vector<XMLProperty>& properties = {});
    void SetQuaternion(const Quaternion &value, const std::vector<XMLProperty>& properties = {});
    void SetRect(const Rect &value, const std::vector<XMLProperty>& properties = {});
    void SetFilepath(const String &filepath, const String &fileExtension = "",
                     const std::vector<XMLProperty>& properties = {});
    void SetEnum(const std::vector<String>& enumNames,
                 const String& selectedEnumName,
                 const std::vector<XMLProperty>& properties = {});
    void SetEnum(const std::vector<String>& enumNames,
                 int selectedEnumIndex, const std::vector<XMLProperty>& properties = {});
    void SetButton(const String buttonText,
                   IAttrWidgetButtonListener *listener,
                   const std::vector<XMLProperty>& properties = {});


    /**
     * @brief HasVectorLikeType
     * @return Whether the type is either Float, Vector*, or Quaternion.
     */
    bool HasVectoredType() const;

    /**
     * @brief GetNumberOfFields
     * @return The number of fields of the type. For example: TFloat=1, TVector2=2, TQuaternion=4, etc.
     */
    int GetNumberOfFieldsOfType() const;

    const String ToString() const override;

    const String& GetName() const;
    const XMLAttribute::Type& GetType() const;

    const String& GetValue() const;
    bool GetBool() const;
    int GetInt() const;
    float GetFloat() const;
    String GetFilepath() const;
    String GetString() const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;
    Color GetColor() const;
    Quaternion GetQuaternion() const;
    Rect GetRect() const;
    IAttrWidgetButtonListener* GetButtonListener() const;
    String GetEnumSelectedName() const;
    int GetEnumSelectedIndex() const;
    std::vector<String> GetEnumNames() const;

    String GetTypeName() const;
    const std::vector<XMLProperty>& GetProperties() const;

    static XMLAttribute FromString(const String &string);
};

#endif // XMLATTRIBUTE_H
