#ifndef SHADERCONTRACT_H
#define SHADERCONTRACT_H

#include "Bang.h"

//TODO: Add check of all variables beginning with ShaderNames_Prefix, to see
//      if all of them have an existing name. Do this when loading shaders.

#include <string>

class ShaderContract
{
private:
    ShaderContract() {}

public:
    const static std::string \
        ShaderNames_Prefix,

        Vertex_In_Position_Raw,
        Vertex_In_Normal_Raw,
        Vertex_In_Uv_Raw,

        Vertex_Out_Fragment_In_Vertex_Position_Raw,
        Vertex_Out_Fragment_In_Vertex_Position_Model,
        Vertex_Out_Fragment_In_Vertex_Position_View,
        Vertex_Out_Fragment_In_Vertex_Position_Projection,
        Vertex_Out_Fragment_In_Vertex_Normal_Raw,
        Vertex_Out_Fragment_In_Vertex_Uv_Raw,

        Fragment_Out_GBuffer_In_Position,
        Fragment_Out_GBuffer_In_Normal,
        Fragment_Out_GBuffer_In_Uv,
        Fragment_Out_GBuffer_In_Diffuse,

        Uniform_Position_Camera,

        Uniform_Matrix_Model,
        Uniform_Matrix_Model_Inverse,
        Uniform_Matrix_View,
        Uniform_Matrix_View_Inverse,
        Uniform_Matrix_Projection,
        Uniform_Matrix_Projection_Inverse,
        Uniform_Matrix_PVM,

        Uniform_Texture_Prefix,

        // When rendering to screen after Deferred Rendering passes.
        // This texture will hold the definitive color values of the rendered scene
        Uniform_Texture_Final_Color,
        Uniform_Material_Diffuse_Color,

        Macro_Draw_To_GBuffer_VS_Declare,
        Macro_Draw_To_GBuffer_VS_Init_Main,
        Macro_Draw_To_GBuffer_FS_Declare,
        Macro_Draw_To_GBuffer_FS_Init_Main,
        Macro_Post_Render_VS_Declare,
        Macro_Post_Render_VS_Init_Main,
        Macro_Post_Render_FS_Declare,
        Macro_Post_Render_FS_Init_Main,

        Macro_Draw_To_GBuffer_VS_Declare_Content,
        Macro_Draw_To_GBuffer_VS_Init_Main_Content,
        Macro_Draw_To_GBuffer_FS_Declare_Content,
        Macro_Draw_To_GBuffer_FS_Init_Main_Content,
        Macro_Post_Render_VS_Declare_Content,
        Macro_Post_Render_VS_Init_Main_Content,
        Macro_Post_Render_FS_Declare_Content,
        Macro_Post_Render_FS_Init_Main_Content,

        Filepath_Shader_Fragment_Selection,

        Filepath_Shader_Vertex_Render_To_Screen,
        Filepath_Shader_Fragment_Render_To_Screen,

        Filepath_Shader_Vertex_Pass_Position,
        Filepath_Shader_Vertex_Pass_Position_Normal,
        Filepath_Shader_Vertex_Model_Position,
        Filepath_Shader_Vertex_Model_Position_Normal,
        Filepath_Shader_Vertex_PVM_Position,
        Filepath_Shader_Vertex_PVM_Position_Normal,
        Filepath_Shader_Vertex_PVM_Position_Normal_Uv,

        Filepath_Shader_Fragment_Pass_Position,
        Filepath_Shader_Fragment_Pass_Position_Normal,
        Filepath_Shader_Fragment_Pass_Position_Normal_Uv
    ;
};

#endif // SHADERCONTRACT_H
