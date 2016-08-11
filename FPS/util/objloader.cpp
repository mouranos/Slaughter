#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "objloader.h"
#include "vboindexer.h"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide :
// - Binary files. Reading a model should be just a few memcpy's away, not
// parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

loadOBJ::loadOBJ(const char* path)
{
    printf("Loading OBJ file %s...\n", path);

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    bool hasUv = false, hasNormal = false;

    std::ifstream file(path, std::ios::in);
    if(!file.is_open())
    {
        printf("Impossible to open the file ! Are you in the right path ?\n");
        getchar();
    }

    std::string line;
    while(std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        switch(type[0])
        {
        case 'v':
            if(type.length() == 1)
            {
                glm::vec3 vertex;
                float componentW; // Unused

                if(!(stream >> vertex.x >> vertex.y >> vertex.z))
                    throw std::runtime_error("Invalid number of vertices.");

                if(!(stream >> componentW))
                    componentW = 1.0;
                temp_vertices.push_back(vertex);
            }
            else{
                switch(type[1])
                {
                case 't':
                {
                    glm::vec2 uv;
                    if(!(stream >> uv.x >> uv.y))
                        throw std::runtime_error("Invalid number of uvs.");
                    temp_uvs.push_back(uv);
                    break;
                }
                case 'n':
                {
                    glm::vec3 normal;
                    if(!(stream >> normal.x >> normal.y >> normal.z))
                        throw std::runtime_error("Invalid number of normals.");
                    temp_normals.push_back(normal);
                    break;
                }
                default:
                {
                    throw std::runtime_error(
                        "File can't be read by our simple "
                        "parser :-( Try exporting with other "
                        "options.");
                    break;
                }
                }
            }
            break;
        case 'f':
            std::string buf;
            std::size_t numFaces = 0;
            while(stream >> buf)
            {
                std::string vertex, uv, normal;
                std::stringstream item(buf);
                std::getline(item, vertex, '/');
                std::getline(item, uv, '/');
                vertices_.push_back(temp_vertices[std::stoi(vertex)-1]);
                if(!uv.empty())
                {
                    hasUv = true;
                    uvs_.push_back(temp_uvs[std::stoi(uv)-1]);
                }
                std::getline(item, normal, '/');
                if(!normal.empty())
                {
                    hasNormal = true;
                    normals_.push_back((temp_normals[std::stoi(normal)-1]));
                }
                numFaces++;
            }
            if(numFaces < 3)
            {
                throw std::runtime_error("Invalid number of face elements.");
            }
            else
            {
                switch(numFaces)
                {
                case 3:
                    polygonType = GL_TRIANGLES;
                    break;
                case 4:
                    polygonType = GL_QUADS;
                    break;
                default:
                    polygonType = GL_POLYGON;
                    break;
                }
            }
        }
    }
    if(hasUv & hasNormal)
    {
        indexVBO(vertices_, uvs_, normals_, indices_, indexedVertices_,
                 indexedUvs_, indexedNormals_);
    }
    else
    {
        if(hasUv)
        {
            indexVBO(vertices_, uvs_, indices_, indexedVertices_, indexedUvs_);
        }
        else if(hasNormal)
        {
            indexVBO(vertices_, normals_, indices_, indexedVertices_,
                     indexedNormals_);
        }
    }
}

#ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails
                  // to compile on your machine, at least all the other
                  // tutorials still work)

// Include AssImp
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

    bool loadAssImp(const char* path, std::vector<unsigned short>& indices,
                    std::vector<glm::vec3>& vertices,
                    std::vector<glm::vec2>& uvs,
                    std::vector<glm::vec3>& normals)
    {

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
            path,
            0 /*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
        if(!scene)
        {
            fprintf(stderr, importer.GetErrorString());
            getchar();
            return false;
        }
        const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code
                                                // we always use the 1rst
        // mesh (in OBJ files there is often only one anyway)

        // Fill vertices positions
        vertices.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            aiVector3D pos = mesh->mVertices[i];
            vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
        }

        // Fill vertices texture coordinates
        uvs.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            aiVector3D UVW =
                mesh->mTextureCoords[0][i]; // Assume only 1 set of UV
                                            // coords; AssImp supports
                                            // 8 UV sets.
            uvs.push_back(glm::vec2(UVW.x, UVW.y));
        }

        // Fill vertices normals
        normals.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            aiVector3D n = mesh->mNormals[i];
            normals.push_back(glm::vec3(n.x, n.y, n.z));
        }

        // Fill face indices
        indices.reserve(3 * mesh->mNumFaces);
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            // Assume the model has only triangles.
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[1]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }

        // The "scene" pointer will be deleted automatically by "importer"
    }

#endif
