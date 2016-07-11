#include "Material.h"

Material::Material() : Asset(),
                       m_diffuseColor(glm::vec4(0.0f)),
                       p_shaderProgram(nullptr)
{
}

Material::Material(const Material &m)
{
    SetDiffuseColor(m.GetDiffuseColor());

    //This is a copy of the pointers,
    // not the textures themselves
    m_textures = m.m_textures;

    //TODO: copy shaderProgram
    SetShaderProgram(m.GetShaderProgram());
}

Material::~Material()
{

}

void Material::Bind() const
{
    if(p_shaderProgram )
    {
        p_shaderProgram->Bind();

        for(unsigned int i = 0; i < m_textures.size(); ++i)
        {
            const Texture *t = m_textures[i];
            if(t )
            {
                std::string texName = ShaderContract::Uniform_Texture_Prefix + std::to_string(i);
                p_shaderProgram->SetUniformTexture(texName, t, false); //Set the uniform with the texture slot
                t->Bind(); //Leave it bound
            }
        }

        p_shaderProgram->SetUniformVec4(ShaderContract::Uniform_Material_Diffuse_Color,
                                      m_diffuseColor, false);
    }
}

void Material::UnBind() const
{
    if(p_shaderProgram )
    {
        p_shaderProgram->UnBind();
        for(unsigned int i = 0; i < m_textures.size(); ++i)
        {
            if(m_textures[i] )
                m_textures[i]->UnBind();
        }
    }
}

#ifdef BANG_EDITOR
void Material::Write(std::ostream &f) const
{
    FileWriter::Write("<Material>", f);
    std::string vsFile =  "", fsFile = "";
    if(this->p_shaderProgram)
    {
        if(this->p_shaderProgram->GetVertexShader())
        {
            vsFile = this->p_shaderProgram->GetVertexShader()->GetFilepath();
        }

        if(this->p_shaderProgram->GetFragmentShader())
        {
            fsFile = this->p_shaderProgram->GetFragmentShader()->GetFilepath();
        }
    }
    FileWriter::Write(vsFile, f);
    FileWriter::Write(fsFile, f);
    FileWriter::Write(m_diffuseColor, f);
    FileWriter::Write("</Material>", f);
}

void Material::Read(std::istream &f)
{
    std::string vshaderFilepath = FileReader::ReadString(f);
    std::string fshaderFilepath = FileReader::ReadString(f);
    std::string texFilepath = FileReader::ReadString(f);
    glm::vec4 diffColor = FileReader::ReadVec4(f);

    SetShaderProgram(new ShaderProgram(vshaderFilepath, fshaderFilepath));
    Texture2D *tex = AssetsManager::GetAsset<Texture2D>( texFilepath );
    if(tex )
    {
        tex->SetTextureSlot(0);
        SetTexture(tex);
    }

    SetDiffuseColor(diffColor);
}
#endif

void Material::SetShaderProgram(const ShaderProgram *program)
{
    p_shaderProgram = program;
}

void Material::SetTexture(const Texture *texture, unsigned int index)
{
    while(m_textures.size() <= index) m_textures.push_back(nullptr);
    m_textures[index] = texture;
}

void Material::SetDiffuseColor(const glm::vec4 &diffuseColor)
{
    this->m_diffuseColor = diffuseColor;
}

const ShaderProgram *Material::GetShaderProgram() const
{
    return p_shaderProgram;
}

const Texture *Material::GetTexture(unsigned int index) const
{
    if(index >= m_textures.size()) return nullptr;
    return m_textures[index];
}

glm::vec4 Material::GetDiffuseColor() const
{
    return m_diffuseColor;
}
