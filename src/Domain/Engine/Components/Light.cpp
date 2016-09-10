#include "Light.h"

#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"

Light::Light()
{
}

void Light::SetUniformsBeforeApplyingLight() const
{
    ShaderProgram *sp = m_lightMaterial->GetShaderProgram();
    sp->SetUniformFloat("B_light_intensity",      m_intensity,                          false);
    sp->SetUniformColor("B_light_color",          m_color,                              false);
    sp->SetUniformVec3 ("B_light_forward_world",  gameObject->transform->GetForward(),  false);
    sp->SetUniformVec3 ("B_light_position_world", gameObject->transform->GetPosition(), false);
}

void Light::ApplyLight(GBuffer *gbuffer) const
{
    SetUniformsBeforeApplyingLight();
    gbuffer->RenderPassWithMaterial(m_lightMaterial);
}

void Light::ApplyLight(Renderer *rend) const
{
    // GBuffer must be bound
    SetUniformsBeforeApplyingLight();
    rend->RenderWithMaterial(m_lightMaterial);
}

const String Light::ToString() const
{
    return "Light";
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = static_cast<Light*>(clone);
    l->SetIntensity(GetIntensity());
    l->SetColor(GetColor());
}

ICloneable *Light::Clone() const
{
    Light *l = new Light();
    CloneInto(l);
    return l;
}

void Light::SetColor(const Color &color)
{
    m_color = color;
}

void Light::SetIntensity(float intensity)
{
    m_intensity = intensity;
}

Color Light::GetColor() const
{
    return m_color;
}

float Light::GetIntensity() const
{
    return m_intensity;
}

#ifdef BANG_EDITOR
void Light::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void Light::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void Light::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    SetIntensity(xmlInfo->GetFloat("Intensity"));
    SetColor(xmlInfo->GetColor("Color"));
    m_lightMaterial = AssetsManager::LoadAsset<Material>(xmlInfo->GetFilepath("Material"));
}

void Light::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Light");

    xmlInfo->SetFloat("Intensity", GetIntensity());
    xmlInfo->SetColor("Color", GetColor());
    xmlInfo->SetFilepath("Material", m_lightMaterial->GetFilepath(), "bmat",
                        {XMLProperty::Hidden});
}
