#include "SingleLineRenderer.h"

SingleLineRenderer::SingleLineRenderer()
{
    #ifdef BANG_EDITOR
        m_inspectorComponentInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Origin", {0.0f, 0.0f, 0.0f}));
        m_inspectorComponentInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Destiny", {0.0f, 0.0f, 0.0f}));
    #endif

    m_points.resize(2);
    m_points[0] = Vector3(0.0f);
    m_points[1] = Vector3(1.0f);

    this->m_drawLinesMode = RenderMode::Lines;
}

SingleLineRenderer::~SingleLineRenderer()
{

}

const std::string SingleLineRenderer::ToString() const
{
    return "SingleLineRenderer";
}

std::string SingleLineRenderer::GetName() const { return "SingleLineRenderer"; }

ICloneable *SingleLineRenderer::Clone() const
{
    return new SingleLineRenderer();
}

void SingleLineRenderer::SetOrigin(Vector3 o)
{
    m_points[0] = o;
    BindPointsToVAO();
}

void SingleLineRenderer::SetDestiny(Vector3 d)
{
    m_points[1] = d;
    BindPointsToVAO();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* SingleLineRenderer::GetComponentInfo()
{
    LineRenderer::GetComponentInfo();

    InspectorVFloatSWInfo *originInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                m_inspectorComponentInfo.GetSlotInfo(2));
    originInfo->m_value = {m_points[0].x, m_points[0].y, m_points[0].z};

    InspectorVFloatSWInfo *destinyInfo  =
            static_cast<InspectorVFloatSWInfo*>(
                m_inspectorComponentInfo.GetSlotInfo(3));
    destinyInfo->m_value = {m_points[1].x, m_points[1].y, m_points[1].z};

    return &m_inspectorComponentInfo;
}

void SingleLineRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    LineRenderer::OnSlotValueChanged(source);

    std::vector<float> origin = source->GetSWVectorFloatValue("Origin");
    m_points[0] = Vector3(origin[0], origin[1], origin[2]);

    std::vector<float> destiny = source->GetSWVectorFloatValue("Destiny");
    m_points[1] = Vector3(destiny[0], destiny[1], destiny[2]);
}


void SingleLineRenderer::Write(std::ostream &f) const
{
    f << "<SingleLineRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(p_material->GetFilepath(), f);
    FileWriter::Write(Vector3(m_points[0].x, m_points[0].y, m_points[0].z), f);
    FileWriter::Write(Vector3(m_points[1].x, m_points[1].y, m_points[1].z), f);
    FileWriter::Write(GetLineWidth(), f);
    f << "</SingleLineRenderer>" << std::endl;
}

void SingleLineRenderer::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetMaterial( AssetsManager::GetAsset<Material>(FileReader::ReadString(f)));
    SetOrigin(FileReader::ReadVec3(f));
    SetDestiny(FileReader::ReadVec3(f));
    SetLineWidth(FileReader::ReadFloat(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif
