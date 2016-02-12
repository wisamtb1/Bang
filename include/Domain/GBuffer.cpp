#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateDepthBufferAttachment();
    CreateTextureAttachment(Attachment::Position);
    CreateTextureAttachment(Attachment::Normal);
    CreateTextureAttachment(Attachment::Uv);
    CreateTextureAttachment(Attachment::Diffuse);

    renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_Render_To_Screen,
                                          ShaderContract::Filepath_Shader_Fragment_Render_To_Screen);
    renderToScreenMaterial->SetShaderProgram(sp);

    renderToScreenPlaneMesh = new MeshScreenPlane();
}

GBuffer::~GBuffer()
{
    if(renderToScreenMaterial != nullptr) delete renderToScreenMaterial;
    if(renderToScreenPlaneMesh != nullptr) delete renderToScreenPlaneMesh;
}

void GBuffer::RenderToScreen() const
{
    TextureRender *positionTex = GetTextureAttachment(GBuffer::Attachment::Position);
    TextureRender *normalTex   = GetTextureAttachment(GBuffer::Attachment::Normal);
    TextureRender *uvTex       = GetTextureAttachment(GBuffer::Attachment::Uv);
    TextureRender *diffuseTex  = GetTextureAttachment(GBuffer::Attachment::Diffuse);

    renderToScreenPlaneMesh->GetVAO()->Bind();

    //Pass the textures to the render to screen shaderProgram, through the Material
    //First set the corresponding texture slots for every texture
    positionTex->SetTextureSlot(GBuffer::Attachment::Position); //0
    normalTex->SetTextureSlot(GBuffer::Attachment::Normal);     //1
    uvTex->SetTextureSlot(GBuffer::Attachment::Uv);             //2
    diffuseTex->SetTextureSlot(GBuffer::Attachment::Diffuse);   //3
    //Now attach to the material, with its corresponding index for the name (BANG_texture_0)
    //which in this case are the same as each respective texture slot
    renderToScreenMaterial->SetTexture(positionTex, GBuffer::Attachment::Position); //0
    renderToScreenMaterial->SetTexture(normalTex,   GBuffer::Attachment::Normal);   //1
    renderToScreenMaterial->SetTexture(uvTex,       GBuffer::Attachment::Uv);       //2
    renderToScreenMaterial->SetTexture(diffuseTex,  GBuffer::Attachment::Diffuse);  //3
    renderToScreenMaterial->Bind();

    //Render the screen plane!
    glDrawArrays(renderToScreenPlaneMesh->GetRenderMode(), 0, renderToScreenPlaneMesh->GetVertexCount());

    renderToScreenMaterial->UnBind();
    renderToScreenPlaneMesh->GetVAO()->UnBind();
}
