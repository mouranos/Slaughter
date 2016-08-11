#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class loadOBJ{
public:
    loadOBJ(const char* path);
    GLenum getPolygonType(){return polygonType;}
    std::vector<unsigned short> getIndices(){return indices_;}
    std::vector<glm::vec3> getIndexedVertices(){return indexedVertices_;}
    std::vector<glm::vec2> getIndexedUvs(){return indexedUvs_;}
    std::vector<glm::vec3> getIndexedNormals(){return indexedNormals_;}
private:
    GLenum polygonType;
    bool loadMTL(const char* path);
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec2> uvs_;
    std::vector<glm::vec3> normals_;
    std::vector<unsigned short> indices_;
    std::vector<glm::vec3> indexedVertices_;
    std::vector<glm::vec2> indexedUvs_;
    std::vector<glm::vec3> indexedNormals_;
};

//bool loadAssImp(
//	const char * path,
//	std::vector<unsigned short> & indices,
//	std::vector<glm::vec3> & vertices,
//	std::vector<glm::vec2> & uvs,
//	std::vector<glm::vec3> & normals
//);

#endif
