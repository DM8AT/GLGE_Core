/**
 * @file MeshAsset.cpp
 * @author DM8AT
 * @brief implement the mesh asset
 * @version 0.1
 * @date 2025-10-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the mesh asset
#include "MeshAsset.h"
//add the C++ file system
#include <filesystem>
#include <fstream>

//add the GLGE debugging stuff
#include "../../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//add the vertices
#include "Vertex.h"

//add assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

//store the magic number for a mesh asset
static constexpr const char MESH_ASSET_MAGIC[] = "GLGE_MESH";

/**
 * @brief a helper function to write a mesh buffer to a file
 * 
 * @param verts the vertices of the mesh
 * @param indices the indices of the mesh
 * @param path the path of the mesh file to write to
 * @return true : success
 * @return false : failure
 */
static bool __safeMeshAsset(const std::vector<SimpleVertex>& verts, const std::vector<index_t> indices, const String& path, const String& comment = "") noexcept
{
    //create the file to write to
    std::ofstream f(path, std::ofstream::binary);
    //sanity check
    if (!f.is_open()) {return false;}

    //store the magic number
    f.write(MESH_ASSET_MAGIC, strlen(MESH_ASSET_MAGIC));
    
    //store a user-defined, NULL terminated comment
    if (comment.size() > UINT16_MAX) {GLGE_MESSAGE("[WARNING] The comment is too long: it has more than 65535 characters.");}
    uint16_t commentLen = (uint16_t)comment.size();
    f.write((const char*)&commentLen, sizeof(commentLen));
    //only write a comment with a size > 0
    if (commentLen) {f.write(comment.data(), comment.size());}

    //then store a number to identify the type of vertex used. Default is 0. 
    uint32_t vertType = 0;
    f.write((const char*)&vertType, sizeof(vertType));
    //first, store the number of vertices
    uint64_t vertCount = verts.size();
    f.write((const char*)&vertCount, sizeof(vertCount));
    //then, store the actual vertex data
    f.write((const char*)verts.data(), verts.size()*sizeof(SimpleVertex));

    //then, store the number of indices
    uint64_t indCount = indices.size();
    f.write((const char*)&indCount, sizeof(indCount));
    f.write((const char*)indices.data(), indices.size()*sizeof(index_t));

    //success
    return true;
}

/**
 * @brief load a GLGE mesh asset
 * 
 * @param verts the vector to fill with the vertices
 * @param indices the vector to fill with the indices
 * @param path the path to the file to load
 * @return true : successfully loaded the file
 * @return false : failed to load the file / parsing error
 */
static bool __loadMeshAsset(std::vector<SimpleVertex>& verts, std::vector<index_t>& indices, const String& path) noexcept {
    //check if the file exists
    if (!std::filesystem::is_regular_file(path)) {return false;}

    //open the file
    std::ifstream f(path);
    if (!f.is_open()) {return false;}

    //get the first few bytes of the file
    char magicBuff[sizeof(MESH_ASSET_MAGIC)]{0};
    f.read(magicBuff, sizeof(magicBuff)-1);
    //sanity check the header
    if (strcmp(magicBuff, MESH_ASSET_MAGIC)) {
        //debug message
        GLGE_DEBUG_MESSAGE("Failed to load the mesh asset: unexpected magic value");
        return false;
    }

    //read the comment
    //first, get the length
    uint16_t commentLen = 0;
    f.read((char*)&commentLen, sizeof(commentLen));
    //only load the comment if it exists
    if (commentLen != 0) {
        //read the actual comment
        String comment;
        comment.resize(commentLen);
        f.read(comment.data(), commentLen*sizeof(char));
        //just print the comment
        std::cout << "[COMMENT] " << comment << "\n";
    }

    //get the type of vertex
    uint32_t vertType = 0;
    f.read((char*)&vertType, sizeof(vertType));

    //read in the vertex data
    uint64_t vertLen = 0;
    f.read((char*)&vertLen, sizeof(vertLen));
    verts.resize(vertLen);
    f.read((char*)verts.data(), verts.size()*sizeof(SimpleVertex));

    //read the index data
    uint64_t indLen = 0;
    f.read((char*)&indLen, sizeof(indLen));
    indices.resize(indLen);
    f.read((char*)indices.data(), indices.size()*sizeof(index_t));

    //success
    return true;
}

String MeshAsset::import(const String& path, const String& suffix) noexcept
{
    //store the string without the suffix
    String path_raw = path;
    //extract the file suffix
    size_t pos = path_raw.find_last_of('.');
    //sanity check (the file may not have a suffix)
    if (pos != String::npos) {
        //cut off the suffix
        path_raw = path_raw.substr(0, pos);
    }
    //store the path of the asset that will be created
    String assPath = path_raw + "." + suffix;
    //check if the asset exists. If it does, just stop. 
    if (std::filesystem::is_regular_file(assPath))
    {return assPath;}

    //load the actual asset
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    //sanity check
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        //print an error
        std::cerr << "[ERROR] Failed to import the file \"" << path << "\" because an error occurred: " << importer.GetErrorString() << "\n";
        return "";
    }

    //if there are no meshes, stop
    if (scene->mNumMeshes == 0) {return "";}

    GLGE_DEBUG_MESSAGE("Successfully loaded \"" << path << "\" for importing");
    //check if there are more than one meshes
    //this is a mesh asset. There should be only one. 
    #if GLGE_BG_DEBUG
    if (scene->mNumMeshes != 1) {
        std::cout << "[WARNING] Importing a mesh asset with " << scene->mNumMeshes << ". Only loading mesh 1.\n";
    }
    #endif

    //extract the mesh
    aiMesh* mesh = scene->mMeshes[0];

    //make a list of all vectors to store
    std::vector<SimpleVertex> verts;
    verts.reserve(mesh->mNumVertices);
    //iterate and load all vertices in the correct format
    for (size_t i = 0; i < verts.capacity(); ++i) {
        //format the vertex correctly
        SimpleVertex vert;
        //store the position. This must always exist. 
        vert.pos = *((vec3*)&mesh->mVertices[i]);
        //if normals exist, add them
        if (mesh->HasNormals())
        {vert.normal = *((vec3*)&mesh->mNormals[i]);}
        //only use the texture coordinate at index 0
        if (mesh->HasTextureCoords(i))
        {vert.tex = *((vec2*)&mesh->mTextureCoords[i][0]);}
        //store the new vertex
        verts.push_back(vert);
    }

    //make a list of all indices to store
    std::vector<index_t> indices;
    indices.reserve(mesh->mNumFaces*3);
    //iterate and encode all faces (all triangles since the models were triangulated)
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        //get the face
        aiFace* face = &(mesh->mFaces[i]);
        //only for debugging check if the face is actually a triangle
        GLGE_DEBUG_ASSERT("Found a triangle during mesh import that has " << face->mNumIndices << " corners. What?", face->mNumIndices != 3);
        //store all indices of the face
        indices.push_back(face->mIndices[0]);
        indices.push_back(face->mIndices[1]);
        indices.push_back(face->mIndices[2]);
    }

    //store the mesh data
    if (!__safeMeshAsset(verts, indices, assPath)) {return "";}

    //success
    return assPath;
}

MeshAsset::MeshAsset(const String& path)
{
    //set the path to load from
    m_path = path;
}

void MeshAsset::load() noexcept
{
    //mark the asset as loading
    updateLoadState(ASSET_STATE_LOADING);
    //load the data
    std::vector<SimpleVertex> verts;
    std::vector<index_t> indices;
    bool success = __loadMeshAsset(verts, indices, m_path);
    //store the actual mesh
    m_ptr = new (m_mesh) Mesh(verts.data(), verts.size(), GLGE_VERTEX_LAYOUT_SIMPLE_VERTEX, indices);
    //depending on the success set the next load state
    updateLoadState(success ? ASSET_STATE_LOADED : ASSET_STATE_FAILED);
}