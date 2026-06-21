#include "backend/loaders/objectLoader/ObjectLoader.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

#include "backend/loaders/materialLoader/MaterialLoader.h"
#include "backend/loaders/pathUtil.h"

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
  uint32_t currentMaterialIndex = 0;  // 0 — дефолтный материал

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

      std::string materialPath = resolveAssetPath(dirPath, mtlFileName);

      MtlFileLoader::loadMtl(materialPath, materialManager);

    } else if (prefix == "usemtl") {
      iss >> currentMaterialName;
      currentMaterialIndex = materialManager.getMaterialId(currentMaterialName);
    } else if (prefix == "f") {
      std::vector<FaceVertex> poly;
      std::string token;
      while (iss >> token) poly.push_back(parseFaceVertex(token, mesh));
      addPolygon(mesh, poly, currentMaterialIndex);
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

ObjectLoader::FaceVertex ObjectLoader::parseFaceVertex(const std::string& token,
                                                       const Mesh& mesh) {
  // Токен может быть: "v", "v/vt", "v//vn", "v/vt/vn".
  int raw[3] = {0, 0, 0};
  bool present[3] = {false, false, false};

  size_t start = 0;
  for (int part = 0; part < 3; ++part) {
    size_t slash = token.find('/', start);
    size_t len =
        (slash == std::string::npos) ? std::string::npos : slash - start;
    std::string piece = token.substr(start, len);
    if (!piece.empty()) {
      try {
        raw[part] = std::stoi(piece);
        present[part] = true;
      } catch (const std::exception&) {
        present[part] = false;
      }
    }
    if (slash == std::string::npos) break;
    start = slash + 1;
  }

  // OBJ допускает отрицательные (относительные) индексы: -1 — последний элемент.
  auto resolve = [](int idx, size_t count) -> int {
    if (idx > 0) return idx - 1;                          // 1-based -> 0-based
    if (idx < 0) return static_cast<int>(count) + idx;    // относительный
    return -1;
  };

  FaceVertex fv{-1, -1, -1};
  if (present[0]) fv.v = resolve(raw[0], mesh.vertices_.size());
  if (present[1]) fv.vt = resolve(raw[1], mesh.uvCoordinates_.size());
  if (present[2]) fv.vn = resolve(raw[2], mesh.normals_.size());
  return fv;
}

void ObjectLoader::addPolygon(Mesh& mesh, const std::vector<FaceVertex>& poly,
                              uint32_t materialIndex) {
  if (poly.size() < 3) {
    std::cerr << "Invalid face data (вершин < 3)\n";
    return;
  }

  // Веерная триангуляция: (0, i, i+1).
  for (size_t i = 1; i + 1 < poly.size(); ++i) {
    const FaceVertex tri[3] = {poly[0], poly[i], poly[i + 1]};

    // Пропускаем грань, если хоть один индекс вершины битый.
    if (tri[0].v < 0 || tri[1].v < 0 || tri[2].v < 0) {
      std::cerr << "Invalid face data (индекс вершины)\n";
      continue;
    }

    Face face;
    face.materialIndex = materialIndex;

    // Если vn есть у всех углов, берём из файла; иначе считаем нормаль грани.
    bool haveNormals = tri[0].vn >= 0 && tri[1].vn >= 0 && tri[2].vn >= 0;
    uint32_t genNormalIdx = 0;
    if (!haveNormals) {
      const Position3F p0 = mesh.vertices_[tri[0].v].head<3>();
      const Position3F p1 = mesh.vertices_[tri[1].v].head<3>();
      const Position3F p2 = mesh.vertices_[tri[2].v].head<3>();
      Normal n = (p1 - p0).cross(p2 - p0);
      float len = n.norm();
      n = (len > 1e-8f) ? Normal(n / len) : Normal(0.0f, 0.0f, 1.0f);
      mesh.addNormal(n);
      genNormalIdx = static_cast<uint32_t>(mesh.normals_.size() - 1);
    }

    // UV-фолбэк: один общий (0,0) на треугольник, если у угла нет vt.
    uint32_t defaultUvIdx = 0;
    bool defaultUvCreated = false;

    for (int k = 0; k < 3; ++k) {
      face.vertexIndex[k] = static_cast<uint32_t>(tri[k].v);
      face.normalIndex[k] =
          haveNormals ? static_cast<uint32_t>(tri[k].vn) : genNormalIdx;

      if (tri[k].vt >= 0) {
        face.uvCoordinateIndex[k] = static_cast<uint32_t>(tri[k].vt);
      } else {
        if (!defaultUvCreated) {
          mesh.addUVCoordinate(UVCoordinate(0.0f, 0.0f));
          defaultUvIdx = static_cast<uint32_t>(mesh.uvCoordinates_.size() - 1);
          defaultUvCreated = true;
        }
        face.uvCoordinateIndex[k] = defaultUvIdx;
      }
    }

    mesh.addFace(face);
  }
}
}  // namespace s21