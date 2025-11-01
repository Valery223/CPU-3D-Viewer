#include "backend/loaders/objectLoader/ObjectLoader.h"

#include <filesystem>

#include "backend/loaders/materialLoader/MaterialLoader.h"

namespace s21 {
void ObjectLoader::loadObj(const std::string& filepath, Mesh& mesh,
                           MaterialManager& materialManager) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Could not open file " << filepath << "\n";
    throw std::runtime_error("File not found");
  }

  std::string line;
  std::string currentMaterialName;
  uint32_t currentMaterialIndex = -1;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "v") {
      mesh.addVertex(parseVertex(iss));
    } else if (prefix == "vn") {
      mesh.addNormal(parseNormal(iss));
    } else if (prefix == "vt") {
      mesh.addUVCoordinate(parseUVcoordiantes(iss));
    } else if (prefix == "mtllib") {
      std::string mtlFileName;
      iss >> mtlFileName;

      std::filesystem::path objPath(filepath);
      std::filesystem::path dirPath = objPath.parent_path();

      std::filesystem::path materialPath = dirPath / mtlFileName;

      MtlFileLoader::loadMtl(materialPath, materialManager);

    } else if (prefix == "usemtl") {
      iss >> currentMaterialName;
      currentMaterialIndex = materialManager.getMaterialId(currentMaterialName);
    } else if (prefix == "f") {
      mesh.addFace(parseFace(iss, currentMaterialIndex));
    }
  }

  file.close();
}

Vertex ObjectLoader::parseVertex(std::istringstream& iss) {
  Vertex vertex;
  if (iss >> vertex[0] >> vertex[1] >> vertex[2]) {
    vertex[3] = 1.0f;
  } else {
    std::cerr << "Invalid vertex data\n";
  }
  return vertex;
}

Normal ObjectLoader::parseNormal(std::istringstream& iss) {
  Normal normal;
  if (iss >> normal[0] >> normal[1] >> normal[2]) {
  } else {
    std::cerr << "Invalid normal data\n";
  }
  return normal;
}

UVCoordinate ObjectLoader::parseUVcoordiantes(std::istringstream& iss) {
  UVCoordinate uv;
  if (iss >> uv[0] >> uv[1]) {
  } else {
    std::cerr << "Invalid uv coordinate\n";
  }
  return uv;
}

Face ObjectLoader::parseFace(std::istringstream& iss, uint32_t materialIndex) {
  Face face;
  char slash;
  unsigned int v1, v2, v3;
  unsigned int n1, n2, n3;
  unsigned int t1, t2, t3;

  if (iss >> v1 >> slash >> t1 >> slash >> n1 >> v2 >> slash >> t2 >> slash >>
      n2 >> v3 >> slash >> t3 >> slash >> n3) {
    face.vertexIndex[0] = v1 - 1;
    face.vertexIndex[1] = v2 - 1;
    face.vertexIndex[2] = v3 - 1;
    face.normalIndex[0] = n1 - 1;
    face.normalIndex[1] = n2 - 1;
    face.normalIndex[2] = n3 - 1;
    face.uvCoordinateIndex[0] = t1 - 1;
    face.uvCoordinateIndex[1] = t2 - 1;
    face.uvCoordinateIndex[2] = t3 - 1;
    face.materialIndex = materialIndex;
  } else {
    std::cerr << "Invalid face data\n";
  }
  return face;
}
}  // namespace s21