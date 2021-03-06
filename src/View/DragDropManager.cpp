#include "DragDropManager.h"

#include "Time.h"
#include "Debug.h"
#include "EditorWindow.h"
#include "DragDropAgent.h"
#include "IDragDropListener.h"

DragDropManager *DragDropManager::s_ddManager = nullptr;

DragDropManager::DragDropManager()
{
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(Update()));
    m_updateTimer.start(c_UpdateTime * 1000);

    InstallEventFilters();
}

void DragDropManager::InstallEventFilters()
{
    // InstallEventFilter of all the widgets of the window :)
    // Do this every certain time, just in case new Widgets appear hehe
    QTimer::singleShot(1000, this, SLOT(InstallEventFilters()));

    EditorWindow *wm = EditorWindow::GetInstance();
    if (wm && wm->GetMainWindow())
    {
        QMainWindow *mw = wm->GetMainWindow();
        QList<QWidget*> widgets = mw->findChildren<QWidget*>();
        foreach(QWidget *w, widgets)
        {
            w->setAcceptDrops(true);
            w->setMouseTracking(true);
            w->installEventFilter(DragDropManager::s_ddManager);
            w->installEventFilter(mw);
        }
    }
}

bool DragDropManager::eventFilter(QObject *obj, QEvent *e)
{
    return QObject::eventFilter(obj, e);
}

DragDropAgent* DragDropManager::GetDragDropAgentBelowMouse()
{
    // Go up in the hierarchy (parent->parent->...->nullptr), until
    // we find a widget that can be cast to DragDropAgent* (if any)
    QObject *objBelowMouse = IWindow::GetWidgetBelowMouse();
    while (objBelowMouse != nullptr)
    {
        DragDropAgent *dda = dynamic_cast<DragDropAgent*>(objBelowMouse);
        if (dda) return dda;
        objBelowMouse = objBelowMouse->parent();
    }
    return nullptr;
}

DragDropInfo* DragDropManager::GetDragDropInfo()
{
    return &(DragDropManager::s_ddManager->m_ddInfo);
}

void DragDropManager::RegisterDragDropAgent(IDragDropListener *dragDropListener)
{
    if (!DragDropManager::s_ddManager)
    {
        DragDropManager::s_ddManager = new DragDropManager();
    }
    DragDropManager::s_ddManager->m_dragDropListeners.PushBack(dragDropListener);
}

void DragDropManager::UnregisterDragDropAgent(IDragDropListener *dragDropListener)
{
    DragDropManager::s_ddManager->m_dragDropListeners.Remove(dragDropListener);
}

void DragDropManager::HandleGlobalMousePress(QObject *obj, QEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager; ASSERT(m);
    m->m_mouseDown = true;
    m->m_latestUpdateDragging = m->m_dragging = false;
    m->m_timeSinceLastMouseDown = 0.0f;

    DragDropAgent *currentDDAgentBelowMouse = DragDropManager::GetDragDropAgentBelowMouse();
    m->m_ddInfo.sourceObject = dynamic_cast<QObject*>(currentDDAgentBelowMouse);
}

void DragDropManager::HandleGlobalMouseRelease(QObject *obj, QEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager; ASSERT(m);
    m->m_mouseDown = false;
}

QObject *DragDropManager::GetDragSource()
{
    DragDropManager *m = DragDropManager::s_ddManager;
    return m->m_ddInfo.sourceObject;
}

bool DragDropManager::IsDragGoingOn()
{
    DragDropManager *m = DragDropManager::s_ddManager;
    return m->m_mouseDown;
}

void DragDropManager::Update()
{
    m_dragging = ( m_mouseDown &&
                     (m_timeSinceLastMouseDown >= c_TimeToStartDrag)
                    );

    DragDropAgent *currentDDAgentBelowMouse = DragDropManager::GetDragDropAgentBelowMouse();

    m_ddInfo.previousObject = m_ddInfo.currentObject;
    m_ddInfo.currentObject = dynamic_cast<QObject*>(currentDDAgentBelowMouse);

    bool changingOfDragDropAgent = currentDDAgentBelowMouse != m_latestDDAgentBelowMouse;

    // Drag Starts in currentDDAgentBelowMouse
    bool isDragStart = !m_latestUpdateDragging && m_dragging;
    if (isDragStart)
    {
        for (IDragDropListener *d : m_dragDropListeners)
        {
            d->OnDragStart(m_ddInfo);
        }
    }

    if (m_dragging)
    {
        // Drag Enters into currentDDAgentBelowMouse
        bool isDragEnter = currentDDAgentBelowMouse  && changingOfDragDropAgent;
        if (isDragEnter)
        {
            for (IDragDropListener *d : m_dragDropListeners)
            {
                d->OnDragEnter(m_ddInfo);
            }
        }

        // Drag Moves in currentDDAgentBelowMouse
        bool isDragMove = currentDDAgentBelowMouse && !changingOfDragDropAgent;
        if (isDragMove)
        {
            for (IDragDropListener *d : m_dragDropListeners)
            {
                d->OnDragMove(m_ddInfo);
            }
        }

        // Drag Leaves m_latestDDAgentBelowMouse
        bool isDragLeave = m_latestDDAgentBelowMouse && changingOfDragDropAgent;
        if (isDragLeave)
        {
            for (IDragDropListener *d : m_dragDropListeners)
            {
                d->OnDragLeave(m_ddInfo);
            }
        }
    }

    m_latestDDAgentBelowMouse = currentDDAgentBelowMouse;

    if (!m_dragging)
    {
        bool isDrop = m_latestUpdateDragging && !m_dragging;
        if (isDrop)
        {
            for (IDragDropListener *d : m_dragDropListeners)
            {
                d->OnDrop(m_ddInfo);
            }
        }
        m_latestDDAgentBelowMouse = nullptr;
    }
    m_latestUpdateDragging  = m_dragging;

    if (m_mouseDown)
    {
        m_timeSinceLastMouseDown += c_UpdateTime;
    }
    else
    {
        m_timeSinceLastMouseDown = 0.0f;
    }
}

