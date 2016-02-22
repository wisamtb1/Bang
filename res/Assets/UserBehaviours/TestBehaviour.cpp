#include "TestBehaviour.h"

#include "ShaderContract.h"
#include "Transform.h"
#include "Material.h"
#include "Logger.h"
#include "Canvas.h"
#include "Stage.h"
#include "Time.h"

TestBehaviour::TestBehaviour()
{
    time = 0.0f;
    randomAxis = glm::vec3( float(rand()%2000-1000)/1000,
                            float(rand()%2000-1000)/1000,
                            float(rand()%2000-1000)/1000);
    randomAxis = glm::normalize(randomAxis);
}

void TestBehaviour::OnStart()
{
    originalScale = GetOwner()->GetPart<Transform>()->GetScale().x;

    if(originalScale > 0.1f)
    {
        Material *m = new Material();

        m->SetShaderProgram(new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv,
                                              ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv));

        m->SetDiffuseColor(glm::vec4( float(rand()%2000-1000)/1000,
                                      float(rand()%2000-1000)/1000,
                                      float(rand()%2000-1000)/1000,
                                      0.2f));

        if(rand() % 2 == 0)
            m->SetTexture( AssetsManager::GetAsset<Texture2D>("res/Assets/woodTexture.asset") );
        else
            m->SetTexture( AssetsManager::GetAsset<Texture2D>("res/Assets/carpetTexture.asset") );

        GetOwner()->GetPart<MeshRenderer>()->SetMaterial(m);
    }

    GetOwner()->GetPart<Transform>()->SetPosition(glm::vec3(float(rand()%2000-1000)/1000 * 6.0f,
                                                            float(rand()%2000-1000)/1000 * 3.0f,
                                                            float(rand()%2000-1000)/1000 * 3.0f));
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetOwner()->GetPart<Transform>();
    t->SetRotation(glm::angleAxis(1.0f * Time::GetDeltaTime(), randomAxis) * t->GetRotation());
    t->SetScale(glm::vec3(originalScale) * (float(sin(time)) * 0.5f + 0.5f + 0.2f));
}

void TestBehaviour::OnDestroy()
{

}

