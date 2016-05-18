#include "LineRenderer.h"

LineRenderer::LineRenderer()
{
    #ifdef BANG_EDITOR
        inspectorComponentInfo.SetSlotsInfos(
        {
            new InspectorFileSWInfo("Material",
                        Material::GetFileExtensionStatic()),
            new InspectorVFloatSWInfo("Origin", {0.0f, 0.0f, 0.0f}),
            new InspectorVFloatSWInfo("Destiny", {999.9f, 999.9f, 999.9f}),
            new InspectorVFloatSWInfo("Line Width", {0.0f})
        });
    #endif

    points.resize(2);
    points[1] = Vector3(999.9f);

    vbo = new VBO();
    vbo->Fill(points.data(), points.size() * sizeof(Vector3));

    vao = new VAO();
    vao->BindVBO(vbo, 0, 3 * points.size(), GL_FLOAT);

    SetLineWidth(2.0f);

    Material *m = AssetsManager::GetAsset<Material>(
                "./res/Materials/lines.bmat" );
    SetMaterial(m);
}

LineRenderer::~LineRenderer()
{

}

const std::string LineRenderer::ToString() const
{
    return "LineRenderer";
}

std::string LineRenderer::GetName() const { return "LineRenderer"; }


void LineRenderer::BindPointsToVAO() const
{
    if(material != nullptr && material->GetShaderProgram() != nullptr)
    {
        vbo->Fill(points.data(), points.size() * sizeof(Vector3));
        GLint verticesShaderLocation = material->GetShaderProgram()->
                GetLocation(ShaderContract::Vertex_In_Position_Raw);
        vao->UnBindVBO(verticesShaderLocation);
        vao->BindVBO(vbo, verticesShaderLocation, 3, GL_FLOAT);
    }
}

void LineRenderer::ActivateStatesBeforeRendering() const
{
    Renderer::ActivateStatesBeforeRendering();
}

void LineRenderer::OnRender()
{
    Scene *scene = GetOwner()->GetScene();
    Camera *cam = scene->GetCamera();
    if(!CAN_USE_COMPONENT(cam))
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
                       << scene << " does not have a set Camera.");
    }

    if(material == nullptr)
    {
        Logger_Verbose(owner << " could not be rendered because it does " <<
                       "not have a Material (or it's disabled')");
        return;
    }
    else if(material->GetShaderProgram() == nullptr)
    {
        Logger_Error(owner << " has a Material with no ShaderProgram." <<
                     "Can't render.");
        return;
    }
    Render();
}

void LineRenderer::Render() const
{
    ActivateStatesBeforeRendering();

    glm::mat4 model, view, projection, pvm;
    GetMatrices(model, view, projection, pvm);
    SetMatricesUniforms(model, view, projection, pvm);

    vao->Bind();
    material->Bind();
    glDrawArrays(Renderer::RenderMode::Lines, 0, points.size());
    material->UnBind();
    vao->UnBind();
}

void LineRenderer::SetMaterial(Material *m)
{
    material = m;
    BindPointsToVAO();
}

void LineRenderer::SetOrigin(Vector3 o)
{
    points[0] = o;
    BindPointsToVAO();
}

void LineRenderer::SetDestiny(Vector3 d)
{
    points[1] = d;
    BindPointsToVAO();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* LineRenderer::GetComponentInfo()
{
    InspectorFileSWInfo* matInfo =
            static_cast<InspectorFileSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(0));

    if (material != nullptr)
    {
        if(material->GetFilepath() != "")
        {
            matInfo->filepath = material->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(matInfo->filepath, (void*)material);
        }
    }
    else
    {
        matInfo->filepath = "-";
    }

    InspectorVFloatSWInfo *originInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(1));
    originInfo->value = {points[0].x, points[0].y, points[0].z};

    InspectorVFloatSWInfo *destinyInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(2));
    destinyInfo->value = {points[1].x, points[1].y, points[1].z};

    InspectorVFloatSWInfo *widthInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                inspectorComponentInfo.GetSlotInfo(3));
    widthInfo->value = {GetLineWidth()};

    return &inspectorComponentInfo;
}

void LineRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    if(materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    std::vector<float> origin = source->GetSWVectorFloatValue("Origin");
    points[0] = Vector3(origin[0], origin[1], origin[2]);

    std::vector<float> destiny = source->GetSWVectorFloatValue("Destiny");
    points[1] = Vector3(destiny[0], destiny[1], destiny[2]);

    std::vector<float> widthInfo = source->GetSWVectorFloatValue("Line Width");
    SetLineWidth(widthInfo[0]);
}


void LineRenderer::Write(std::ostream &f) const
{
    f << "<LineRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(material->GetFilepath(), f);
    FileWriter::Write(Vector3(points[0].x, points[0].y, points[0].z), f);
    FileWriter::Write(Vector3(points[1].x, points[1].y, points[1].z), f);
    FileWriter::Write(GetLineWidth(), f);
    f << "</LineRenderer>" << std::endl;
}

void LineRenderer::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetMaterial( AssetsManager::GetAsset<Material>(FileReader::ReadString(f)));
    SetOrigin(FileReader::ReadVec3(f));
    SetDestiny(FileReader::ReadVec3(f));
    SetLineWidth(FileReader::ReadFloat(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif
