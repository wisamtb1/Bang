#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "FileReader.h"
#include "Hierarchy.h"
#include "WindowEventManager.h"

#include "EditorSelectionGameObject.h"

GameObject::GameObject() : GameObject("")
{
}

GameObject::GameObject(const std::string &name) : m_name(name)
{
}

void GameObject::CloneInto(ICloneable *clone) const
{
    GameObject *go = static_cast<GameObject*>(clone);
    for(GameObject *child : m_children)
    {
        if(child->IsEditorGameObject()) continue;
        go->AddChildWithoutNotifyingHierarchy( static_cast<GameObject*>(child->Clone()) );
    }

    for(Component *comp : m_comps)
    {
        go->AddComponent( static_cast<Component*>(comp->Clone()) );
    }

    go->SetName(m_name);
    go->SetRenderLayer(m_renderLayer);
    go->m_isScene = m_isScene;
    go->p_parent = p_parent;
}

ICloneable* GameObject::Clone() const
{
    GameObject *clone = new GameObject();
    CloneInto(clone);
    return clone;
}

GameObject::~GameObject()
{
    this->_OnDestroy();
    for (auto it = m_children.begin(); it != m_children.end();)
    {
        GameObject *child = *it;
        it = this->RemoveChildWithoutNotifyingHierarchy(it);
        delete child;
    }
}


Scene *GameObject::GetScene()
{
    if (m_isScene) { return (Scene*) this; }
    if (p_parent) return p_parent->GetScene();
    return nullptr;
}

GameObject *GameObject::GetParent() const
{
    return p_parent;
}

const std::string GameObject::GetName() const { return m_name; }

unsigned char GameObject::GetRenderLayer() const
{
    return m_renderLayer;
}

const std::list<Component *> &GameObject::GetComponents() const { return m_comps; }

const std::list<GameObject *> GameObject::GetChildren() const
{
    std::list<GameObject *> cc;
    for (auto c = m_children.begin(); c != m_children.end(); ++c)
    {
        if (!(*c)->IsEditorGameObject()) cc.push_back(*c);
    }
    return cc;
}

Box GameObject::GetObjectBoundingBox() const
{
    Box b;
    MeshRenderer *mr = GetComponent<MeshRenderer>();
    if (CAN_USE_COMPONENT(mr))
    {
        const Mesh *m = mr->GetMesh();
        if (m)
        {
            b = m->GetBoundingBox();
        }
    }

    return b;
}

Box GameObject::GetLocalBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    Transform *t = GetComponent<Transform>();
    if(CAN_USE_COMPONENT(t))
    {
        Matrix4 mat;
        t->GetLocalModelMatrix(mat);
        b = mat * b; //Apply transform to Box
    }
    return b;
}

Box GameObject::GetBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    Transform *t = GetComponent<Transform>();
    if(CAN_USE_COMPONENT(t))
    {
        Matrix4 mat;
        t->GetModelMatrix(mat);
        b = mat * b;
    }
    return b;
}

Sphere GameObject::GetObjectBoundingSphere() const
{
    return Sphere::FromBox(GetObjectBoundingBox());
}

Sphere GameObject::GetLocalBoundingSphere() const
{
    return Sphere::FromBox(GetLocalBoundingBox());
}

Sphere GameObject::GetBoundingSphere() const
{
    return Sphere::FromBox(GetBoundingBox());
}

void GameObject::AddComponent(Component *c)
{
    c->p_gameObject = this;
    m_comps.push_back(c);
    c->_OnStart();
}

void GameObject::MoveComponent(Component *c, int distance)
{
    for(auto comp = m_comps.begin(); comp != m_comps.end(); ++comp)
    {
        if(c == *comp)
        {
            auto comp1 = comp;
            std::advance(comp1, 1);
            m_comps.erase(comp, comp1);
            std::advance(comp1, distance);
            m_comps.insert(comp1, 1, c);
            break;
        }
    }
}

void GameObject::RemoveComponent(Component *c)
{
    for(auto comp = m_comps.begin(); comp != m_comps.end(); ++comp)
    {
        if(c == *comp)
        {
            m_comps.erase(comp);
            break;
        }
    }
}


void GameObject::AddChildWithoutNotifyingHierarchy(GameObject *child)
{
    child->p_parent = this;
    m_children.push_back(child);
}

void GameObject::AddChild(GameObject *child)
{
    AddChildWithoutNotifyingHierarchy(child);

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildAdded(child);
    #endif
}

GameObject *GameObject::GetChild(const std::string &name) const
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        GameObject *child = (*it);
        if(child->m_name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void GameObject::MoveChild(GameObject *child, GameObject *newParent)
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if((*it) == child)
        {
            RemoveChildWithoutNotifyingHierarchy(it);
            newParent->AddChildWithoutNotifyingHierarchy(child);

            #ifdef BANG_EDITOR
            WindowEventManager::NotifyChildChangedParent(child, this);
            #endif

            break;
        }
    }
}

std::list<GameObject*>::iterator GameObject::RemoveChildWithoutNotifyingHierarchy(
        std::list<GameObject*>::iterator &it)
{
    GameObject *child = (*it);
    child->p_parent = nullptr;
    return m_children.erase(it);
}


std::list<GameObject*>::iterator GameObject::RemoveChild(
        std::list<GameObject*>::iterator &it)
{
    auto itret = RemoveChildWithoutNotifyingHierarchy(it);
    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildRemoved((*it));
    #endif
    return itret;
}

void GameObject::RemoveChild(const std::string &name)
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        GameObject *child = (*it);
        if(child->m_name == name)
        {
            RemoveChild(it);
            break;
        }
    }
}

void GameObject::RemoveChild(GameObject *child)
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if((*it) == child)
        {
            RemoveChild(it);
            break;
        }
    }
}

void GameObject::SetParent(GameObject *newParent)
{
    GameObject *previousParent = p_parent;
    if(p_parent )
    {
        if(newParent )
        {
            p_parent->MoveChild(this, newParent);
        }
        else
        {
            Scene *st = GetScene();
            if(st )
            {
                p_parent->MoveChild(this, newParent);
            }
        }
    }

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildChangedParent(this, previousParent);
#endif
}

void GameObject::SetRenderLayer(unsigned char layer)
{
    this->m_renderLayer = layer;
}

void GameObject::SetName(const std::string &name)
{
    this->m_name = name;
    Hierarchy::GetInstance()->OnGameObjectNameChanged(this);
}

bool GameObject::IsEditorGameObject() const
{
    return false;
}

bool GameObject::IsScene() const
{
    return m_isScene;
}

#ifdef BANG_EDITOR
void GameObject::OnTreeHierarchyGameObjectsSelected(
        std::list<GameObject*> &selectedEntities )
{
    if(IsEditorGameObject() || IsScene()) return;

    bool selected = false;
    for(auto it : selectedEntities)
    {
        if( it == this )
        {
            selected = true;
            break;
        }
    }

    if(selected)
    {
        if(!m_wasSelectedInHierarchy)
        {
            m_selectionGameObject = new EditorSelectionGameObject(this);
            Canvas::GetCurrentScene()->AddChildWithoutNotifyingHierarchy(m_selectionGameObject);
        }
    }
    else
    {
        if(m_wasSelectedInHierarchy)
        {
            Canvas::GetCurrentScene()->RemoveChild(m_selectionGameObject);
        }
    }

    m_wasSelectedInHierarchy = selected;
}
#endif

void GameObject::Write(std::ostream &f) const
{
    f << "<GameObject>" << std::endl;
    f << ((void*)this) << std::endl;   //internal file id
    f << this->GetName() << std::endl; //Print name

    f << "<children>" << std::endl;
    for(GameObject *e : m_children)
    {
        e->Write(f);
    }
    f << "</children>" << std::endl;

    f << "<components>" << std::endl;
    for(Component *p : m_comps)
    {
        p->Write(f);
    }
    f << "</components>" << std::endl;

    f << "</GameObject>" << std::endl;
}

void GameObject::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this); //Read GameObject id
    SetName( FileReader::ReadString(f) );  //Read GameObject name
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</GameObject>")
    {
        if(line == "<children>")
        {
            FileReader::ReadChildren(f, this);
        }
        else if(line == "<components>")
        {
            FileReader::ReadComponents(f, this);
        }
        else
        {

        }
    }
}

void GameObject::SetEnabled(bool enabled) { this->m_enabled = enabled; }
bool GameObject::IsEnabled()
{
    return m_enabled && (!p_parent ? true : p_parent->IsEnabled());
}


const std::string GameObject::ToString() const
{
    return " [GameObject:'" + m_name + "']";
}

void GameObject::_OnStart()
{
    PROPAGATE_EVENT(_OnStart, m_children);
    OnStart();
}


void GameObject::_OnUpdate()
{
    bool canUpdate =
            !Canvas::GetInstance()->IsPaused() ||
            IsEditorGameObject();

    if(canUpdate)
    {
        PROPAGATE_EVENT(_OnUpdate, m_comps);
    }

    PROPAGATE_EVENT(_OnUpdate, m_children);

    if(canUpdate)
    {
        OnUpdate();
    }
}

void GameObject::_OnPreRender ()
{
    PROPAGATE_EVENT(_OnPreRender, m_children);

    if(this->m_renderLayer == GetScene()->m_currentRenderLayer)
    {
        PROPAGATE_EVENT(_OnPreRender, m_comps);
        OnPreRender();
    }
}

void GameObject::_OnRender ()
{
    PROPAGATE_EVENT(_OnRender, m_children);

    if(this->m_renderLayer == GetScene()->m_currentRenderLayer)
    {
        PROPAGATE_EVENT(_OnRender, m_comps);
        OnRender();
    }
}

void GameObject::_OnDestroy()
{
    PROPAGATE_EVENT(_OnDestroy, m_comps);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
    OnDestroy();
}
