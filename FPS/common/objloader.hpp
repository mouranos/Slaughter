#ifndef OBJLOADER_H
#define OBJLOADER_H

class loadOBJ{
public:
    loadOBJ(const char* path);
    loadOBJ(const char* path,int d);
    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec2> out_uvs;
    std::vector<glm::vec3> out_normals;
};

//bool loadAssImp(
//	const char * path,
//	std::vector<unsigned short> & indices,
//	std::vector<glm::vec3> & vertices,
//	std::vector<glm::vec2> & uvs,
//	std::vector<glm::vec3> & normals
//);

#endif
