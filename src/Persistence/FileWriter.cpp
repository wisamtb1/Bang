#include "FileWriter.h"
#include "Scene.h"
#include "Asset.h"

FileWriter::FileWriter()
{
}

void FileWriter::WriteScene(std::string filename, Scene *scene)
{
    std::ofstream ofs;
    filename = Persistence::AppendExtension(filename,
                                            Scene::GetFileExtension());
    ofs.open(filename);
    if(!ofs.is_open())
    {
        Logger_Error("There was an error when saving file '" << filename << "'");
        ofs.close();
        return;
    }
    scene->Write(ofs);
    ofs.close();
}

void FileWriter::WriteAsset(std::string filename, Asset *a)
{
    std::ofstream ofs;
    filename = Persistence::AppendExtension(filename,
                                            a->GetFileExtension());
    ofs.open(filename);
    if(!ofs.is_open())
    {
        Logger_Error("There was an error when saving file '" << filename << "'");
        ofs.close();
        return;
    }
    a->Write(ofs);
    ofs.close();
}

void FileWriter::Write(float v, std::ostream &f)
{
    f << v << std::endl;
}

void FileWriter::Write(const glm::vec2 &v, std::ostream &f)
{
    f << v.x << " " << v.y << std::endl;
}

void FileWriter::Write(const Vector3 &v, std::ostream &f)
{
    f << v.x << " " << v.y << " " << v.z << std::endl;
}

void FileWriter::Write(const glm::vec4 &v, std::ostream &f)
{
    f << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
}


void FileWriter::Write(const Quaternion &q, std::ostream &f)
{
    f << q.x << " " << q.y << " " << q.z << " " << q.w << std::endl;
}

void FileWriter::Write(const Rect &r, std::ostream &f)
{
    f << r.minx << " " << r.maxx << " " << r.miny << " " << r.maxy << std::endl;
}

void FileWriter::Write(const std::string &str, std::ostream &f)
{
    if(str != "")
    {
        f << str << std::endl;
    }
    else
    {
        f << "-" << std::endl;
    }
}

void FileWriter::WriteFilepath(const std::string &path, std::ostream &f)
{
    Write(Persistence::ProjectRootAbsoluteToRelative(path), f);
}
