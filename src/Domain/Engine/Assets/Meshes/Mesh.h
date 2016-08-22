#ifndef MESH_H
#define MESH_H

#include "Bang.h"

#include <GL/glew.h>
#include <vector>

#include "glm/glm.hpp"

#include "ShaderContract.h"
#include "IFileable.h"
#include "FileReader.h"
#include "Sphere.h"
#include "Asset.h"
#include "VAO.h"
#include "VBO.h"
#include "Box.h"

class Mesh : public Asset
{
friend class MeshRenderer;

public:
    const static String GetFileExtensionStatic() { return "bmesh"; }
    const virtual String GetFileExtension()
    {
        return Mesh::GetFileExtensionStatic();
    }

private:
    Box m_bBox;
    Sphere m_bSphere;

    std::vector<Vector3> m_positions;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_uvs;

    VAO *m_vao = nullptr;
    VBO *m_vertexPositionsVBO = nullptr;
    VBO *m_vertexNormalsVBO = nullptr;
    VBO *m_vertexUvsVBO = nullptr;
    int m_vertexCount = 0;

    bool m_trisModel = false;

public:
    Mesh();
    Mesh(const Mesh& m);
    virtual ~Mesh();

    void LoadFromFile(const String &m_filepath);
    void LoadPositions(const std::vector<Vector3>& positions);
    void LoadNormals(const std::vector<Vector3>& normals);
    void LoadUvs(const std::vector<Vector2>& uvs);
    void LoadAll(const std::vector<Vector3>& positions,
                 const std::vector<Vector3>& normals,
                 const std::vector<Vector2>& uvs);

    void BindPositionsToShaderProgram(const String &nameInShader, const ShaderProgram &sp);
    void BindNormalsToShaderProgram(const String &nameInShader, const ShaderProgram &sp);
    void BindUvsToShaderProgram(const String &nameInShader, const ShaderProgram &sp);

    void BindAllVBOsToShaderProgram(const ShaderProgram &sp);

    VAO *GetVAO() const;
    int GetVertexCount() const;
    bool IsATrianglesModel() const;
    const Box& GetBoundingBox() const;
    const Sphere& GetBoundingSphere() const;

    const std::vector<Vector3>& GetPositions();
    const std::vector<Vector3>& GetNormals();
    const std::vector<Vector2>& GetUvs();

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // MESH_H
