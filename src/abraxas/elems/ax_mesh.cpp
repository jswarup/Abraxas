// mesh.cpp ------------------------------------------------------------------------------------------------------------------

#include  "utils/ax_includes.h"
#include  "abraxas/elems/ax_mesh.h"
#include  "abraxas/render/ax_glbuffers.h"
#include  "abraxas/render/ax_shader.h"
#include  <assimp/Importer.hpp>
#include  <assimp/postprocess.h>
#include  <assimp/scene.h>

 
//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Mesh::Init()
{   
    m_VertexIndexBuf.CreateBuffers( m_Vertices, m_VertexIndices);
}

//---------------------------------------------------------------------------------------------------------------------------------

Ax_Mesh::~Ax_Mesh()
{
    m_VertexIndexBuf.DeleteBuffers();
}

//---------------------------------------------------------------------------------------------------------------------------------

bool Ax_Mesh::Load(const std::string& filepath)
{
    const uint32_t cMeshImportFlags = aiProcess_CalcTangentSpace |
                                      aiProcess_Triangulate |
                                      aiProcess_SortByPType |
                                      aiProcess_GenNormals |
                                      aiProcess_GenUVCoords |
                                      aiProcess_OptimizeMeshes |
                                      aiProcess_ValidateDataStructure;

    Assimp::Importer  Importer;

    const aiScene     *pScene = Importer.ReadFile( filepath.c_str(), cMeshImportFlags);

    if ( pScene && pScene->HasMeshes())
    {
        m_VertexIndices.clear();
        m_Vertices.clear();

        auto    *mesh = pScene->mMeshes[ 0];

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Ax_Axial vh( { mesh->mVertices[i].x, mesh->mVertices[i].y ,mesh->mVertices[i].z },
                              { mesh->mNormals[i].x, mesh->mNormals[i].y ,mesh->mNormals[i].z });

            AddVertex( vh);
        }

        for ( size_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace  face = mesh->mFaces[i];
            for ( size_t j = 0; j < face.mNumIndices; j++)
                AddVertexIndex( face.mIndices[j]);
        }

        Init();
        return true;
    }
    return false;
}
 
 
//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Mesh::Render()
{
    m_VertexIndexBuf.Draw( (int) m_VertexIndices.size());
}

//---------------------------------------------------------------------------------------------------------------------------------

void    Ax_Mesh::Update( Ax_Shader *shader)  
{
    // pbr color
    shader->set_vec3( m_Color, "albedo"); 
    shader->set_f1( m_Roughness, "roughness");
    shader->set_f1( m_Metallic, "metallic");
    shader->set_f1( 1.0f, "ao");
}

//---------------------------------------------------------------------------------------------------------------------------------
 