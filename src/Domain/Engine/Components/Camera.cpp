#include "Camera.h"
#include "Canvas.h"
#include "FileReader.h"

Camera::Camera() : m_orthoRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f)),
                   m_fovDegrees(60.0f),
                   m_zNear(0.1f),
                   m_zFar(100.0f),
                   m_aspectRatio(1.0f),
                   m_projMode(ProjectionMode::Perspective),
                   m_autoUpdateAspectRatio(true)
{
    #ifdef BANG_EDITOR
    m_inspectorInfo.AddSlotInfos(
    {
        new InspectorVFloatSWInfo( "FOV", 1),
        new InspectorVFloatSWInfo( "Z Near", 1),
        new InspectorVFloatSWInfo( "Z Far", 1),
        new InspectorVFloatSWInfo( "Aspect Ratio", 1),
        new InspectorEnumSWInfo( "Projection Mode", {"Orthographic", "Perspective"} )
    });
    #endif
}

void Camera::GetViewMatrix(Matrix4 *view) const
{
    Transform *t = gameObject->transform;
    if (CAN_USE_COMPONENT(t))
    {
        t->GetModelMatrix(view);
        *view = view->Inversed();
    }
    else
    {
        Logger_Verbose(gameObject << " has a Camera but does not have a transform. " <<
                       "View matrix will be the idgameObject matrix.");

        *view = Matrix4(1.0f);
    }
}

void Camera::GetProjectionMatrix(Matrix4 *proj) const
{
    if (m_projMode == ProjectionMode::Perspective)
    {
        if (m_autoUpdateAspectRatio)
        {
            m_aspectRatio = Canvas::GetAspectRatio();
        }

        *proj = Matrix4::Perspective(glm::radians(m_fovDegrees), m_aspectRatio, m_zNear, m_zFar);
    }
    else //Ortho
    {
        *proj = Matrix4::Ortho(m_orthoRect.m_minx, m_orthoRect.m_maxx,
                               m_orthoRect.m_miny, m_orthoRect.m_maxy,
                               m_zNear, m_zFar);
    }
}

void Camera::SetOrthoRect(const Rect &rect)
{
    m_orthoRect = rect;
}







void Camera::SetFovDegrees(float fovDegrees)
{
    this->m_fovDegrees = fovDegrees;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->m_aspectRatio = aspectRatio;
}

void Camera::SetZNear(float zNear)
{
    this->m_zNear = zNear;
}

void Camera::SetZFar(float zFar)
{
    this->m_zFar = zFar;
}

void Camera::SetProjectionMode(Camera::ProjectionMode projMode)
{
    this->m_projMode = projMode;
}

void Camera::SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio)
{
    this->m_autoUpdateAspectRatio = autoUpdateAspectRatio;
}






float Camera::GetFovDegrees() const
{
    return m_fovDegrees;
}

float Camera::GetAspectRatio() const
{
    return m_aspectRatio;
}

float Camera::GetZNear() const
{
    return m_zNear;
}

float Camera::GetZFar() const
{
    return m_zFar;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
    return m_projMode;
}

bool Camera::GetAutoUpdateAspectRatio() const
{
    return m_autoUpdateAspectRatio;
}

glm::vec2 Camera::WorldToScreenNDCPoint(const Vector3 &position)
{
    Matrix4 p, v;
    GetProjectionMatrix(&p);
    GetViewMatrix(&v);
    glm::vec4 v_4 = p * v * glm::vec4(position, 1);
    v_4 /= v_4.w;

    return v_4.xy();
}

const std::string Camera::ToString() const
{
    return "Camera";
}

void Camera::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Camera *cam = static_cast<Camera*>(clone);
    cam->SetAspectRatio(GetAspectRatio());
    cam->SetAutoUpdateAspectRatio(GetAutoUpdateAspectRatio());

}

ICloneable *Camera::Clone() const
{
    Camera *cam = new Camera();
    CloneInto(cam);
    return cam;
}


#ifdef BANG_EDITOR
InspectorWidgetInfo* Camera::OnInspectorInfoNeeded()
{
    m_inspectorInfo.GetSlotInfo("FOV")->SetFloat(m_fovDegrees);
    m_inspectorInfo.GetSlotInfo("Z Near")->SetFloat(m_zNear);
    m_inspectorInfo.GetSlotInfo("Z Far")->SetFloat(m_zFar);
    m_inspectorInfo.GetSlotInfo("Aspect Ratio")->SetFloat(m_aspectRatio);
    m_inspectorInfo.GetSlotInfo("Projection Mode")->SetInt(m_projMode);

    return &m_inspectorInfo;
}

void Camera::OnInspectorInfoChanged(InspectorWidgetInfo *info)
{
    m_fovDegrees = info->GetFloat("FOV");
    m_zNear = info->GetFloat("Z Near");
    m_zFar = info->GetFloat("Z Far");
    m_aspectRatio = info->GetFloat("Aspect Ratio");
    m_projMode =  static_cast<Camera::ProjectionMode>(info->GetEnumSelectedIndex("Projection Mode"));
}

void Camera::ReadXMLNode(const XMLNode *xmlNode)
{
    Component::ReadXMLNode(xmlNode);

    SetFovDegrees(xmlNode->GetFloat("fovDegrees"));
    SetZNear(xmlNode->GetFloat("zNear"));
    SetZFar(xmlNode->GetFloat("zFar"));
    SetProjectionMode(xmlNode->GetString("projMode")  == "Perspective" ?
                          Camera::ProjectionMode::Perspective :
                          Camera::ProjectionMode::Orthographic);
    SetOrthoRect( xmlNode->GetRect("orthoRect") );
}

void Camera::GetXMLNode(XMLNode *xmlNode) const
{
    Component::GetXMLNode(xmlNode);
    xmlNode->SetTagName("Camera");

    xmlNode->AddAttribute("id", this);
    xmlNode->AddAttribute("fovDegrees", m_fovDegrees);
    xmlNode->AddAttribute("zNear", m_zNear);
    xmlNode->AddAttribute("zFar", m_zFar);
    xmlNode->AddAttribute("projMode", (m_projMode == ProjectionMode::Perspective ? "Perspective" : "Orthographic"));
    xmlNode->AddAttribute("orthoRect", m_orthoRect);
}
#endif
