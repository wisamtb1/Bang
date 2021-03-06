#include "EditorGizmosGameObject.h"

#include "Rect.h"
#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "Texture2D.h"
#include "EditorScene.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"
#include "GraphicPipeline.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"
#include "EditorGizmosGameObject.h"

EditorGizmosGameObject::EditorGizmosGameObject(const String &name) :
    EditorGameObject(name)
{
}

void EditorGizmosGameObject::Init()
{
    if (!m_inited)
    {
        m_boxMesh = MeshFactory::GetCube();
        m_sphereMesh = MeshFactory::GetSphere();
        m_planeMesh = MeshFactory::GetPlane();
        m_material = AssetsManager::Load<Material>("Materials/D2G_Default.bmat", true);
        m_material = new Material(*m_material);

        AddComponent<SingleLineRenderer>();
        AddComponent<CircleRenderer>();
        AddComponent<MeshRenderer>();

        List<Renderer*> renderers = GetComponents<Renderer>();
        for (Renderer *rend : renderers)
        {
            rend->SetIsGizmo(true);
            rend->SetMaterial(m_material);
            rend->SetReceivesLighting(false);
            rend->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);
        }

        m_inited = true;
    }
}

void EditorGizmosGameObject::SetResetAllowed(bool allowed)
{
    m_resetAllowed = allowed;
}

void EditorGizmosGameObject::SetColor(const Color &color)
{
    m_material->SetDiffuseColor(color);
}

void EditorGizmosGameObject::SetPosition(const Vector3 &position)
{
    transform->SetLocalPosition(position);
}

void EditorGizmosGameObject::SetRotation(const Quaternion &rotation)
{
    transform->SetLocalRotation(rotation);
}

void EditorGizmosGameObject::SetScale(const Vector3 &scale)
{
    transform->SetLocalScale(scale);
}

void EditorGizmosGameObject::SetLineWidth(float lineWidth)
{
    List<Renderer*> renderers = GetComponents<Renderer>();
    for (Renderer *rend : renderers)
    {
        rend->SetLineWidth(lineWidth);
    }
}

void EditorGizmosGameObject::SetDrawWireframe(bool wireframe)
{
    List<Renderer*> renderers = GetComponents<Renderer>();
    for (Renderer *rend : renderers)
    {
        rend->SetDrawWireframe(wireframe);
    }
}

void EditorGizmosGameObject::SetReceivesLighting(bool receivesLighting)
{
    List<Renderer*> renderers = GetComponents<Renderer>();
    for (Renderer *rend : renderers)
    {
        rend->SetReceivesLighting(receivesLighting);
    }
}

void EditorGizmosGameObject::SetBillboard()
{
    Scene *scene = SceneManager::GetActiveScene();
    GameObject *cam = scene->GetCamera()->gameObject;
    Vector3 lookDir = (cam->transform->GetPosition() - transform->GetPosition());
    lookDir.Normalize();
    Gizmos::SetRotation( Quaternion::LookDirection(lookDir) );
}

void EditorGizmosGameObject::RenderCustomMesh(Mesh *m)
{
    ASSERT(m);
    MeshRenderer *mr = GetComponent<MeshRenderer>();
    mr->SetEnabled(true);
    mr->SetMesh(m);
    Render(mr);
    Reset();
}


void EditorGizmosGameObject::RenderSimpleBox(const AABox &b)
{
    const Quaternion &r = transform->GetLocalRotation();
    const Vector3& bMin = b.GetMin();
    const Vector3& bMax = b.GetMax();

    SetResetAllowed(false);
    RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMin.z));
    RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMin.z));
    RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMin.y, bMax.z));

    RenderLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    RenderLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMax.z));

    RenderLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    RenderLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    RenderLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMin.y, bMax.z));
    RenderLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    RenderLine(r * Vector3(bMin.x, bMax.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    RenderLine(r * Vector3(bMax.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    RenderLine(r * Vector3(bMax.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    SetResetAllowed(true);

    Reset();
}

void EditorGizmosGameObject::RenderBox(const AABox &b)
{

    MeshRenderer *mr = GetComponent<MeshRenderer>();
    mr->SetEnabled(true);
    mr->SetMesh(m_boxMesh);
    transform->SetPosition(b.GetCenter());
    transform->SetScale(transform->GetScale() * b.GetDimensions());
    Render(mr);
    Reset();
}

void EditorGizmosGameObject::RenderRect(const Rect &r)
{
    SetResetAllowed(false);
    RenderScreenLine( Vector2(r.m_minx, r.m_miny), Vector2(r.m_maxx, r.m_miny) );
    RenderScreenLine( Vector2(r.m_maxx, r.m_miny), Vector2(r.m_maxx, r.m_maxy) );
    RenderScreenLine( Vector2(r.m_maxx, r.m_maxy), Vector2(r.m_minx, r.m_maxy) );
    RenderScreenLine( Vector2(r.m_minx, r.m_maxy), Vector2(r.m_minx, r.m_miny) );
    SetResetAllowed(true);
    Reset();
}

void EditorGizmosGameObject::RenderCircle(float radius)
{
    CircleRenderer *cr = GetComponent<CircleRenderer>();
    cr->SetRadius(radius);
    cr->Render();
    Reset();
}

void EditorGizmosGameObject::RenderIcon(const Texture2D *texture,
                                        bool billboard)
{
    MeshRenderer *mr = GetComponent<MeshRenderer>();
    ASSERT(m_planeMesh);
    mr->SetMesh(m_planeMesh);

    SetDrawWireframe(false);
    SetReceivesLighting(false);
    if (billboard)
    {
        Camera *cam = SceneManager::GetActiveScene()->GetCamera();
        Vector3 camPos = cam->transform->GetPosition();

        float distScale = 1.0f;
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
           Vector3 pos = transform->GetPosition();
           distScale = Vector3::Distance(camPos, pos);
        }

        Vector3 scale = transform->GetScale();
        transform->SetScale(distScale * scale);
        transform->LookInDirection(cam->transform->GetForward(),
                                   cam->transform->GetUp());
    }
    mr->GetMaterial()->SetTexture(texture);
    Render(mr);
    Reset();
}

void EditorGizmosGameObject::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
    SingleLineRenderer *slr = GetComponent<SingleLineRenderer>();
    slr->SetOrigin(origin);
    slr->SetDestiny(destiny);

    transform->SetPosition(Vector3::Zero);
    transform->SetScale(Vector3::One);

    Render(slr);
    Reset();
}

void EditorGizmosGameObject::RenderScreenLine(const Vector2 &origin, const Vector2 &destiny)
{
    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); ASSERT(cam);
    const float z = cam->GetZNear() + 0.01f;
    Vector3 worldPosOrigin  = cam->ScreenNDCPointToWorld(origin,  z);
    Vector3 worldPosDestiny = cam->ScreenNDCPointToWorld(destiny, z);
    bool resetAllowedBefore = m_resetAllowed;
    SetResetAllowed(false);
    RenderLine(worldPosOrigin, worldPosDestiny);
    SetResetAllowed(resetAllowedBefore);
    Reset();
}

void EditorGizmosGameObject::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
    RenderLine(origin, origin + rayDir);
    Reset();
}

void EditorGizmosGameObject::RenderSphere(const Vector3 &origin, float radius)
{
    MeshRenderer *mr = GetComponent<MeshRenderer>();
    mr->SetEnabled(true);
    mr->SetMesh(m_sphereMesh);
    transform->SetPosition(origin);
    transform->SetScale(radius);
    Render(mr);
    Reset();
}

void EditorGizmosGameObject::RenderFrustum(
       const Vector3 &forward, const Vector3 &up,
       const Vector3 &origin,
       float zNear, float zFar,
       float fovDegrees, float aspectRatio)
{
    const Vector3 &c = origin;
    const Vector3 right = Vector3::Cross(forward, up).Normalized();

    const float fovH = glm::radians(fovDegrees) / 2.0f;

    Vector3 nearPlaneCenter = c + forward * zNear;
    Vector3 farPlaneCenter  = c + forward * zFar;

    float nearHeight2 = zNear * glm::tan(fovH);
    float nearWidth2 = nearHeight2 * aspectRatio;
    float farHeight2  = zFar  * glm::tan(fovH);
    float farWidth2  = farHeight2 * aspectRatio;

    Vector3 nearUpLeft    = nearPlaneCenter - right * nearWidth2 + up * nearHeight2;
    Vector3 nearUpRight   = nearPlaneCenter + right * nearWidth2 + up * nearHeight2;
    Vector3 nearDownRight = nearPlaneCenter + right * nearWidth2 - up * nearHeight2;
    Vector3 nearDownLeft  = nearPlaneCenter - right * nearWidth2 - up * nearHeight2;

    Vector3 farUpLeft     = farPlaneCenter - right * farWidth2 + up * farHeight2;
    Vector3 farUpRight    = farPlaneCenter + right * farWidth2 + up * farHeight2;
    Vector3 farDownRight  = farPlaneCenter + right * farWidth2 - up * farHeight2;
    Vector3 farDownLeft   = farPlaneCenter - right * farWidth2 - up * farHeight2;

    SetResetAllowed(false);

    // Near plane
    RenderLine(nearUpLeft   , nearUpRight);
    RenderLine(nearUpRight  , nearDownRight);
    RenderLine(nearDownRight, nearDownLeft);
    RenderLine(nearDownLeft , nearUpLeft);

    // Far plane
    RenderLine(farUpLeft   , farUpRight);
    RenderLine(farUpRight  , farDownRight);
    RenderLine(farDownRight, farDownLeft);
    RenderLine(farDownLeft , farUpLeft);

    // Projection lines
    RenderLine(nearUpLeft   , farUpLeft);
    RenderLine(nearUpRight  , farUpRight);
    RenderLine(nearDownRight, farDownRight);
    RenderLine(nearDownLeft , farDownLeft);

    SetResetAllowed(true);
    Reset();
}

void EditorGizmosGameObject::RenderSimpleSphere(const Vector3 &origin, float radius)
{
    CircleRenderer *cr = GetComponent<CircleRenderer>();
    cr->SetEnabled(true);
    cr->SetRadius(radius);

    transform->SetPosition(origin);

    transform->SetLocalEuler(0, 0, 0);
    Render(cr);
    transform->SetLocalEuler(0, 90, 0);
    Render(cr);

    transform->SetLocalEuler(0, 0, 0);
    Render(cr);
    transform->SetLocalEuler(90, 0, 0);

    Render(cr);
    Reset();
}

void EditorGizmosGameObject::Reset()
{
    Init();
    if (!m_resetAllowed) { return; }

    SetPosition(Vector3::Zero);
    SetRotation(Quaternion::Identity);
    SetScale(Vector3::One);
    SetColor(Color::Green);
    SetLineWidth(1.0f);
    SetReceivesLighting(false);
    SetDrawWireframe(false);

    List<Renderer*> renderers = GetComponents<Renderer>();
    for (Renderer *rend : renderers)
    {
        rend->SetEnabled(false);
    }

    MeshRenderer *mr = GetComponent<MeshRenderer>();
    mr->GetMaterial()->SetTexture(nullptr);
}

void EditorGizmosGameObject::Render(Renderer *rend)
{
    List<Renderer*> renderers = GetComponents<Renderer>();
    for (Renderer *r : renderers)
    {
        r->SetEnabled(r == rend); // Enable only rend
    }

    GraphicPipeline *gp = GraphicPipeline::GetActive(); ASSERT(gp);
    gp->RenderRenderer(rend);
}

EditorGizmosGameObject *EditorGizmosGameObject::GetInstance()
{
    Scene *scene = SceneManager::GetActiveScene();
    ASSERT(scene, "", return nullptr);
    EditorScene *edScene = static_cast<EditorScene*>(scene);
    ASSERT(edScene, "", return nullptr);

    EditorGizmosGameObject *gizmosGo =
            edScene->GetGizmosGameObject();
    gizmosGo->Init(); // Just in case
    return gizmosGo;
}
