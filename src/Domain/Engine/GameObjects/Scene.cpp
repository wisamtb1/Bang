#include "Scene.h"

#ifdef BANG_EDITOR
#include "EditorScene.h"
#endif

#include "Debug.h"
#include "Screen.h"
#include "Camera.h"
#include "Material.h"
#include "XMLParser.h"
#include "Transform.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "SceneManager.h"
#include "BehaviourHolder.h"
#include "GraphicPipeline.h"
#include "BehaviourManager.h"
#include "DirectionalLight.h"

Scene::Scene() : GameObject("Scene")
{
}

void Scene::_OnStart()
{
    GameObject::_OnStart();
}

void Scene::_OnUpdate()
{
    GameObject::_OnUpdate();

    Camera *cam = m_cameraGameObject->GetComponent<Camera>();
    if (cam  && cam->GetAutoUpdateAspectRatio())
    {
        cam->SetAspectRatio( Screen::GetInstance()->GetAspectRatio() );
    }
}

void Scene::_OnResize(int newWidth, int newHeight)
{
}

const String Scene::GetFileExtensionStatic()
{
    return "bscene";
}

Scene::~Scene()
{
    _OnDestroy();
    delete m_defaultCamera;
}

void Scene::CloneInto(ICloneable *clone) const
{
    GameObject::CloneInto(clone);
}

ICloneable *Scene::Clone() const
{
    Scene *scene = new Scene();
    CloneInto(scene);
    return scene;
}

void Scene::SetCamera(const Camera *cam)
{
    if (!cam)
    {
        m_cameraGameObject = nullptr;
        SetCamera(m_defaultCamera->GetComponent<Camera>());
    }
    else
    {
        m_cameraGameObject = cam->gameObject;
    }
}

void Scene::SetFirstFoundCameraOrDefaultOne()
{
    List<Camera*> cameras = GetComponentsInChildren<Camera>();
    bool cameraFound = false;
    for (Camera *cam : cameras)
    {
        if (!cam->gameObject->IsEditorGameObject())
        {
            SetCamera(cam);
            cameraFound = true;
            break;
        }
    }

    if (!cameraFound) // Create default camera
    {
        Debug_Warn("No camera was found. Creating default camera...");
        GameObject *m_defaultCamera = new GameObject("DefaultCamera");
        m_defaultCamera->transform->SetPosition(Vector3(90));
        m_defaultCamera->transform->LookAt(Vector3::Zero);
        m_defaultCamera->SetParent(this);

        Camera *cam = m_defaultCamera->AddComponent<Camera>();
        cam->SetFovDegrees(60.0f); cam->SetZNear(0.1f);
        cam->SetZFar(99999.0f);
        SetCamera(cam);
    }
}

Scene *Scene::GetActiveScene()
{
    return SceneManager::GetActiveScene();
}

Camera *Scene::GetCamera() const
{
    if (!m_cameraGameObject)
    {
        return nullptr;
    }
    return m_cameraGameObject->GetComponent<Camera>();
}

Scene *Scene::GetDefaultScene()
{
    #ifdef BANG_EDITOR
    Scene *scene = new EditorScene();
    #else
    Scene *scene = new Scene();
    #endif

    GameObject *cube = MeshFactory::GetCubeGameObject();
    cube->SetParent(scene);

    const float c_dist = 5.0f;
    GameObject *dirLight = new GameObject("Directional Light");
    dirLight->AddComponent<DirectionalLight>();
    dirLight->transform->SetPosition(Vector3(-1, 1, -1) * c_dist);
    dirLight->transform->LookAt(cube);
    dirLight->SetParent(scene);

    GameObject *camera = new GameObject("Camera");
    Camera *camComp = camera->AddComponent<Camera>();
    camera->transform->SetPosition(Vector3(1, 1, -1) * c_dist);
    camera->transform->LookAt(cube);
    camComp->SetClearColor(Color::LightBlue);
    camera->SetParent(scene);

    return scene;
}

bool Scene::IsScene() const
{
    return true;
}

void Scene::ReadXMLInfo(const XMLNode *xmlInfo)
{
    GameObject::ReadXMLInfo(xmlInfo);
}

void Scene::FillXMLInfo(XMLNode *xmlInfo) const
{
    GameObject::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Scene");

    Camera *cam = GetCamera();
    xmlInfo->SetPointer("id", cam, {XMLProperty::Hidden});
}

void Scene::PostReadXMLInfo(const XMLNode *xmlInfo)
{
    GameObject::PostReadXMLInfo(xmlInfo);

    if (!IsEditorGameObject()) // In EditorScene we'll use the EditorCamera
    {
        String camId = xmlInfo->GetString("Camera");
        if (!camId.Empty())
        {
            const Camera *cam = dynamic_cast<const Camera*>(XMLParser::GetPointerFromId(camId));
            if (cam)
            {
                SetCamera(cam);
            }
        }
    }
}

