#include "MeshAssetFile.h"

#include "Mesh.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "MeshAssetFileInspectable.h"
#endif

MeshAssetFile::MeshAssetFile(const QFileSystemModel *model, const QModelIndex &index) : File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    m_meshFilepath = xmlInfo->GetFilepath("MeshFilepath");
    delete xmlInfo;
}

QPixmap MeshAssetFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Assets/Engine/Icons/MeshAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

#ifdef BANG_EDITOR
IInspectable *MeshAssetFile::GetInspectable() const
{
    return new MeshAssetFileInspectable(*this);
}
#endif

const String& MeshAssetFile::GetMeshFilepath() const
{
    return m_meshFilepath;
}
