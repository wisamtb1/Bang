#include "Mesh.h"

#include <GL/glew.h>

#include "VAO.h"
#include "VBO.h"
#include "XMLNode.h"
#include "IFileable.h"
#include "FileReader.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"

Mesh::Mesh()
{
    m_vao = new VAO();
}

Mesh::Mesh(const Mesh &m)
{
    //TODO, do copy of VAO and VBO's
    m_vao = m.GetVAO();
    m_bBox = m.m_bBox;
    m_bSphere = m.m_bSphere;
    m_vertexPositionsVBO = m.m_vertexPositionsVBO;
    m_vertexNormalsVBO = m.m_vertexNormalsVBO;
    m_vertexUvsVBO = m.m_vertexUvsVBO;
    m_vertexCount = m.m_vertexCount;
    m_positions = m.m_positions;
    m_normals = m.m_normals;
    m_uvs = m.m_uvs;
}

Mesh::~Mesh()
{
    if (m_vertexPositionsVBO) delete m_vertexPositionsVBO;
    if (m_vertexNormalsVBO)   delete m_vertexNormalsVBO;
    if (m_vertexNormalsVBO)   delete m_vertexUvsVBO;
}

const String Mesh::GetFileExtensionStatic() { return "bmesh"; }

const String Mesh::GetFileExtension()
{
    return Mesh::GetFileExtensionStatic();
}

void Mesh::LoadFromFile(const String &filepath)
{
    m_positions.Clear();
    m_normals.Clear();
    m_uvs.Clear();
    if ( FileReader::ReadOBJ(filepath,
                             &m_positions, &m_normals, &m_uvs,
                             &m_trisModel))
    {
        LoadPositions(m_positions);
        LoadNormals(m_normals);
        LoadUvs(m_uvs);
    }
    else
    {
        Debug_Error("There was an error when reading mesh file '" << filepath << "'.");
    }
}

void Mesh::LoadPositions(const Array<Vector3>& positions)
{
    if (m_vertexPositionsVBO) delete m_vertexPositionsVBO;

    m_positions = positions;
    if (m_positions.Empty())
    {
        m_positions.PushBack(0);
    }
    m_vertexPositionsVBO = new VBO();
    m_vertexPositionsVBO->Fill((void*)(&m_positions[0]), m_positions.Size() * sizeof(float) * 3);
    m_vertexCount = m_positions.Size();

    m_bBox.FillFromPositions(m_positions);
    m_bSphere.FromBox(m_bBox);
}

void Mesh::LoadNormals(const Array<Vector3> &normals)
{
    if (m_vertexNormalsVBO) delete m_vertexNormalsVBO;

    m_normals = normals;
    if (m_normals.Empty())
    {
        m_normals.PushBack(0);
    }
    m_vertexNormalsVBO = new VBO();
    m_vertexNormalsVBO->Fill((void*)(&m_normals[0]), m_normals.Size() * sizeof(float) * 3);
}

void Mesh::LoadUvs(const Array<Vector2> &uvs)
{
    if (m_vertexUvsVBO) delete m_vertexUvsVBO;

    m_uvs = uvs;
    if (m_uvs.Empty())
    {
        m_uvs.PushBack(0);
    }
    m_vertexUvsVBO = new VBO();
    m_vertexUvsVBO->Fill((void*)(&m_uvs[0]), m_uvs.Size() * sizeof(float) * 2);
}

void Mesh::LoadAll(const Array<Vector3> &positions,
                   const Array<Vector3> &normals,
                   const Array<Vector2> &uvs)
{
    LoadPositions(positions);
    LoadNormals(normals);
    LoadUvs(uvs);
}

void Mesh::BindPositionsToShaderProgram(const String &nameInShader, const ShaderProgram &sp)
{
    if (m_vertexPositionsVBO)
    {
        GLint location = sp.GetAttribLocation(nameInShader);
        m_vao->BindVBO(m_vertexPositionsVBO, location, 3);
    }
}

void Mesh::BindNormalsToShaderProgram(const String &nameInShader, const ShaderProgram &sp)
{
    if (m_vertexNormalsVBO)
    {
        GLint location = sp.GetAttribLocation(nameInShader);
        m_vao->BindVBO(m_vertexNormalsVBO, location, 3);
    }
}

void Mesh::BindUvsToShaderProgram(const String &nameInShader, const ShaderProgram &sp)
{
    if (m_vertexUvsVBO)
    {
        GLint location = sp.GetAttribLocation(nameInShader);
        m_vao->BindVBO(m_vertexUvsVBO, location, 2);
    }
}

void Mesh::BindAllVBOsToShaderProgram(const ShaderProgram &sp)
{
    BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw, sp);
    BindNormalsToShaderProgram(ShaderContract::Attr_Vertex_In_Normal_Raw, sp);
    BindUvsToShaderProgram(ShaderContract::Attr_Vertex_In_Uv_Raw, sp);
}

VAO *Mesh::GetVAO() const
{
    return m_vao;
}

int Mesh::GetVertexCount() const
{
    return m_vertexCount;
}

bool Mesh::IsATrianglesModel() const
{
    return m_trisModel;
}

const AABox &Mesh::GetAABBox() const
{
    return m_bBox;
}

const Sphere &Mesh::GetBoundingSphere() const
{
    return m_bSphere;
}

const Array<Vector3> &Mesh::GetPositions()
{
    return m_positions;
}

const Array<Vector3> &Mesh::GetNormals()
{
    return m_normals;
}

const Array<Vector2> &Mesh::GetUvs()
{
    return m_uvs;
}

void Mesh::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);
    m_filepath = xmlInfo->GetFilepath("MeshFilepath");
    LoadFromFile(m_filepath);
}

void Mesh::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("MeshFilepath");

    xmlInfo->SetFilepath("MeshFilepath", m_filepath, "obj");
}
