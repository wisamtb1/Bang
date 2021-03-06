#include "PrefabAssetFileInspectable.h"

#include "Debug.h"
#include "Prefab.h"
#include "Inspector.h"
#include "GameObject.h"
#include "FileWriter.h"
#include "EditorWindow.h"
#include "InspectorWidget.h"
#include "EditorGameObject.h"

PrefabAssetFileInspectable::PrefabAssetFileInspectable(const File &f) :
    m_file(f)
{
    Prefab *prefab = new Prefab();
    prefab->ReadXMLInfoFromString(f.GetContents());
    m_prefabTempGameObject = prefab->InstantiateWithoutStarting();
    delete prefab;
}

PrefabAssetFileInspectable::~PrefabAssetFileInspectable()
{
    Inspector::GetInstance()->Clear();
    if (m_prefabTempGameObject)
    {
        delete m_prefabTempGameObject;
    }
}

void PrefabAssetFileInspectable::ShowInInspector()
{
    Inspector::GetInstance()->ShowPrefabInspectableInfo(this);
}

void PrefabAssetFileInspectable::OnInspectorSlotChanged(InspectorWidget *inspectorItem)
{
    SaveToFile();
}

GameObject *PrefabAssetFileInspectable::GetPrefabTempGameObject() const
{
    return m_prefabTempGameObject;
}

void PrefabAssetFileInspectable::SaveToFile()
{
    ASSERT(m_prefabTempGameObject);

    FileWriter::WriteToFile(m_file.GetAbsolutePath(),
                            m_prefabTempGameObject->GetXMLInfoString());
}



