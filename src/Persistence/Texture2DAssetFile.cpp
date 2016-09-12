#include "Texture2DAssetFile.h"

#include "XMLNode.h"
#include "FileReader.h"

#ifdef BANG_EDITOR
#include "Texture2DAssetFileInspectable.h"
#endif

Texture2DAssetFile::Texture2DAssetFile
    (const QFileSystemModel *model, const QModelIndex &index) : File(model, index)
{
}

String Texture2DAssetFile::GetImageFilepath() const
{
    String contents = FileReader::GetContents(GetAbsolutePath());
    XMLNode *xmlInfo = XMLNode::FromString(contents);
    String result = "";
    if (xmlInfo)
    {
        result = xmlInfo->GetString("ImageFilepath");
        delete xmlInfo;
    }
    return result;
}

QPixmap Texture2DAssetFile::GetIcon() const
{
    String fp = GetImageFilepath();

    // Mini cache
    static Map<std::string, QPixmap> filepath_To_Pixmap;
    if (!filepath_To_Pixmap.ContainsKey(fp))
    {
        QPixmap pm(QString::fromStdString(fp));
        filepath_To_Pixmap[fp] = pm;
    }

    return filepath_To_Pixmap[fp];
}

#ifdef BANG_EDITOR
IInspectable *Texture2DAssetFile::GetInspectable() const
{
    return new Texture2DAssetFileInspectable(*this);
}
#endif
