#include "renderRasterize.h"

#include <omp.h>
#include <qdebug.h>

#include <cstdio>

namespace s21 {
RenderRasterize::RenderRasterize(RenderSettings& settings, int width, int hight)
    : IRender(settings, width, hight) {}

void RenderRasterize::rendering(Scene& scene) {
  auto frameStart = std::chrono::steady_clock::now();

  QMutexLocker locker(&_backBufferMutex);
  clearImage();

  for (int i = 0; i < scene.getObjects().size(); i++) {
    // Трансформируем один рабочий меш in-place. Мировые вершины держим отдельно:
    // они ещё нужны для освещения, а work к тому моменту уже в clip space.
    Object work = scene.getObjects()[i];

    transformToWorldCoordinates(work, work);

    std::vector<Vertex> worldVertex = work.getMesh().vertices_;

    Camera& camera = *scene.getCurrentCamera();
    Vector3F viewDir = camera.target - camera.position;
    work.getMesh().faces_ = performBackfaceCullingParallel(
        work.getMesh().faces_, work.getMesh().vertices_, viewDir);

    transformToCameraCoordinates(camera, work, work);  // world -> camera
    projectToCamera(camera, work, work);               // camera -> clip space
    clipedObject(camera, work);                        // отсечение граней

    std::vector<Vertex> screenVertex(work.getMesh().vertices_.size());
    projectToScreen(work, screenVertex);

    if (m_settings.renderFace) {
      rasterizeMesh(work.getMesh(), screenVertex, worldVertex, scene);
    }
    if (m_settings.renderDot || m_settings.renderLine) {
      rasterizeMesh2(work.getMesh(), screenVertex, scene);
    }
  }

  QMutexLocker frontLocker(&_frontBufferMutex);
  swapBuffers();

  auto frameEnd = std::chrono::steady_clock::now();
  accountFrame(
      std::chrono::duration<double, std::milli>(frameEnd - frameStart).count());
}

void RenderRasterize::accountFrame(double frameMs) {
  auto now = std::chrono::steady_clock::now();
  if (!fpsInited_) {
    fpsWindowStart_ = now;
    fpsInited_ = true;
    fpsMsMin_ = fpsMsMax_ = frameMs;
  }

  fpsFrameCount_++;
  fpsMsAccum_ += frameMs;
  fpsMsMin_ = std::min(fpsMsMin_, frameMs);
  fpsMsMax_ = std::max(fpsMsMax_, frameMs);

  double windowMs =
      std::chrono::duration<double, std::milli>(now - fpsWindowStart_).count();
  if (windowMs >= 1000.0) {
    double avgMs = fpsMsAccum_ / fpsFrameCount_;
    // realFps упирается в кэп таймера (~60), потолок 1000/avg от кэпа не зависит.
    double realFps = fpsFrameCount_ * 1000.0 / windowMs;
    std::fprintf(stderr,
                 "[render] %.1f fps | frame avg %.2f ms (min %.2f, max %.2f) | "
                 "потолок ~%.0f fps\n",
                 realFps, avgMs, fpsMsMin_, fpsMsMax_,
                 avgMs > 0.0 ? 1000.0 / avgMs : 0.0);

    fpsWindowStart_ = now;
    fpsFrameCount_ = 0;
    fpsMsAccum_ = 0.0;
    fpsMsMin_ = fpsMsMax_ = frameMs;
  }
}

void RenderRasterize::transformToWorldCoordinates(const Object& objInput,
                                                  Object& objOutput) {
  const Transform& transform = objInput.getTransform();
  const std::vector<Vertex>& localVertexes = objInput.getMesh().vertices_;
  const std::vector<Normal>& localNormals = objInput.getMesh().normals_;

  std::vector<Vertex>& globalVertexes = objOutput.getMesh().vertices_;
  std::vector<Normal>& globalNormals = objOutput.getMesh().normals_;

#pragma omp parallel for
  for (int i = 0; i < globalVertexes.size(); i++) {
    globalVertexes[i] = transform.apply(localVertexes[i]);
  }

#pragma omp parallel for
  for (int i = 0; i < globalNormals.size(); i++) {
    globalNormals[i] = transform.applyToNormal(localNormals[i]);
  }
}

std::vector<Face> RenderRasterize::performBackfaceCullingParallel(
    const std::vector<Face>& faces, const std::vector<Vertex>& vertices,
    const Vector3F& viewDir) {
  size_t num_faces = faces.size();
  std::vector<Face> culledFaces;
  culledFaces.reserve(num_faces);

#pragma omp parallel
  {
    std::vector<Face> localFaces;
    localFaces.reserve(num_faces / omp_get_num_threads());

#pragma omp for nowait
    for (size_t i = 0; i < num_faces; ++i) {
      const auto& v0_4f = vertices[faces[i].vertexIndex[0]];
      const auto& v1_4f = vertices[faces[i].vertexIndex[1]];
      const auto& v2_4f = vertices[faces[i].vertexIndex[2]];

      Vector3F v0(v0_4f.x() / v0_4f.w(), v0_4f.y() / v0_4f.w(),
                  v0_4f.z() / v0_4f.w());
      Vector3F v1(v1_4f.x() / v1_4f.w(), v1_4f.y() / v1_4f.w(),
                  v1_4f.z() / v1_4f.w());
      Vector3F v2(v2_4f.x() / v2_4f.w(), v2_4f.y() / v2_4f.w(),
                  v2_4f.z() / v2_4f.w());

      const auto faceNormal = (v1 - v0).cross(v2 - v0).normalized();

      if (faceNormal.dot(viewDir) < -0.001) {
        localFaces.push_back(faces[i]);
      }
    }

#pragma omp critical
    culledFaces.insert(culledFaces.end(), localFaces.begin(), localFaces.end());
  }

  return culledFaces;
}

void RenderRasterize::transformToCameraCoordinates(const Camera& camera,
                                                   const Object& objInput,
                                                   Object& objOutput) {
  const std::vector<Vertex>& localVertexes = objInput.getMesh().vertices_;
  const std::vector<Normal>& localNormals = objInput.getMesh().normals_;

  std::vector<Vertex>& globalVertexes = objOutput.getMesh().vertices_;
  std::vector<Normal>& globalNormals = objOutput.getMesh().normals_;

  Matrix4x4 matrixVertex = camera.view_matrix;
  Eigen::Matrix3f matrixNormal =
      matrixVertex.block<3, 3>(0, 0).inverse().transpose();

#pragma omp parallel for
  for (int i = 0; i < globalVertexes.size(); i++) {
    globalVertexes[i] = matrixVertex * localVertexes[i];
  }

#pragma omp parallel for
  for (int i = 0; i < globalNormals.size(); i++) {
    globalNormals[i] = (matrixNormal * localNormals[i]).normalized();
  }
}

void RenderRasterize::projectToCamera(const Camera& camera,
                                      const Object& objInput,
                                      Object& objOutput) {
  const std::vector<Vertex>& localVertexes = objInput.getMesh().vertices_;
  std::vector<Vertex>& globalVertexes = objOutput.getMesh().vertices_;
  Matrix4x4 matrixVertex = camera.projection_matrix;

#pragma omp parallel for
  for (int i = 0; i < globalVertexes.size(); i++) {
    globalVertexes[i] = matrixVertex * localVertexes[i];
  }
}

void RenderRasterize::projectToScreen(const Object& objInput,
                                      std::vector<Vertex>& screenVertex) {
  const std::vector<Vertex>& cameraVertexes = objInput.getMesh().vertices_;
  for (int i = 0; i < cameraVertexes.size(); i++) {
    float w = cameraVertexes[i].w();
    screenVertex[i].x() =
        (cameraVertexes[i].x() / w + 1) * 0.5f * _backBuffer.width();
    screenVertex[i].y() =
        (1.0f - cameraVertexes[i].y() / w) * 0.5f * _backBuffer.height();
    screenVertex[i].z() = cameraVertexes[i].z() / w;
    screenVertex[i].w() = w;
  }
}

void RenderRasterize::rasterizeMesh2(const Mesh& mesh,
                                     std::vector<Vertex>& screenVertex,
                                     const Scene& scene) {
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < static_cast<int>(mesh.faces_.size()); i++) {
    const Face& face = mesh.faces_[i];
    const Vertex& v0 = screenVertex[face.vertexIndex[0]];
    const Vertex& v1 = screenVertex[face.vertexIndex[1]];
    const Vertex& v2 = screenVertex[face.vertexIndex[2]];

    if (m_settings.renderLine) {
      drawLine(v0, v1);
      drawLine(v1, v2);
      drawLine(v2, v0);
    };
    if (m_settings.renderDot) {
      if (m_settings.cirul) {
        drawPointAsCircle(v0);
        drawPointAsCircle(v1);
        drawPointAsCircle(v2);
      } else {
        drawPointAsSquar(v0);
        drawPointAsSquar(v1);
        drawPointAsSquar(v2);
      }
    }
  }
}

void RenderRasterize::drawLine(const Vertex& p1, const Vertex& p2) {
  Color color = m_settings.lineColor;
  int x1 = p1.x(), y1 = p1.y();
  int x2 = p2.x(), y2 = p2.y();

  int dx = abs(x2 - x1), dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  int step = 0;
  bool drawPixel = true;

  while (true) {
    if (drawPixel) {
      _backBuffer.setPixelColor(x1, y1,
                                QColor(color.x(), color.y(), color.z()));
    }

    if (x1 == x2 && y1 == y2) break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }

    if (m_settings.lineDash) {
      step++;
      if (step >= 15) {
        drawPixel = !drawPixel;
        step = 0;
      }
    }
  }
}
void RenderRasterize::drawPointAsCircle(const Vertex& center) {
  int radius = m_settings.vertexSize;
  Color color = m_settings.vertexColor;

  if (radius <= 0) {
    return;
  }

  for (int y = -radius; y <= radius; ++y) {
    for (int x = -radius; x <= radius; ++x) {
      if (x * x + y * y <= radius * radius) {
        int pixelX = center.x() + x;
        int pixelY = center.y() + y;
        _backBuffer.setPixelColor(pixelX, pixelY,
                                  QColor(color.x(), color.y(), color.z()));
      }
    }
  }
}

void RenderRasterize::drawPointAsSquar(const Vertex& center) {
  int radius = m_settings.vertexSize;
  Color color = m_settings.vertexColor;

  if (radius <= 0) {
    return;
  }

  for (int y = -radius; y <= radius; ++y) {
    for (int x = -radius; x <= radius; ++x) {
      int pixelX = center.x() + x;
      int pixelY = center.y() + y;
      _backBuffer.setPixelColor(pixelX, pixelY,
                                QColor(color.x(), color.y(), color.z()));
    }
  }
}

void RenderRasterize::rasterizeMesh(const Mesh& mesh,
                                    std::vector<Vertex>& screenVertex,
                                    const std::vector<Vertex>& globalVertex,
                                    const Scene& scene) {
  const int W = _backBuffer.width();
  const int H = _backBuffer.height();
  // Один detach в начале — дальше потоки пишут в сырые байты напрямую.
  unsigned char* bits = _backBuffer.bits();
  const qsizetype bpl = _backBuffer.bytesPerLine();
  const Light& light = scene.getLight(0);

  // Делим экран на горизонтальные полосы строк и раздаём потокам. Полоса владеет
  // своими строками, так что записи в цвет/глубину не пересекаются и блокировки
  // не нужны. Полос берём больше, чем потоков: треугольники по кадру распределены
  // неравномерно, и schedule(dynamic) сам выравнивает нагрузку.
  const int bands = std::min(H, std::max(1, omp_get_max_threads() * 4));

#pragma omp parallel for schedule(dynamic)
  for (int b = 0; b < bands; ++b) {
    const int yLo = static_cast<int>(static_cast<long long>(b) * H / bands);
    const int yHi = static_cast<int>(static_cast<long long>(b + 1) * H / bands);

    for (size_t i = 0; i < mesh.faces_.size(); ++i) {
      const Face& face = mesh.faces_[i];
      const Vertex& v0 = screenVertex[face.vertexIndex[0]];
      const Vertex& v1 = screenVertex[face.vertexIndex[1]];
      const Vertex& v2 = screenVertex[face.vertexIndex[2]];

      // Быстрый отбор: треугольник не задевает строки этой полосы.
      int triMinY = static_cast<int>(std::min({v0.y(), v1.y(), v2.y()}));
      int triMaxY = static_cast<int>(std::max({v0.y(), v1.y(), v2.y()}));
      if (triMaxY < yLo || triMinY >= yHi) continue;

      drawTriangle(v0, v1, v2, mesh.uvCoordinates_[face.uvCoordinateIndex[0]],
                   mesh.uvCoordinates_[face.uvCoordinateIndex[1]],
                   mesh.uvCoordinates_[face.uvCoordinateIndex[2]],
                   mesh.normals_[face.normalIndex[0]],
                   mesh.normals_[face.normalIndex[1]],
                   mesh.normals_[face.normalIndex[2]],
                   globalVertex[face.vertexIndex[0]],
                   globalVertex[face.vertexIndex[1]],
                   globalVertex[face.vertexIndex[2]], light,
                   scene.getMaterial(face.materialIndex), yLo, yHi, bits, bpl, W,
                   H);
    }
  }
}

void RenderRasterize::drawTriangle(
    const Vertex& v0, const Vertex& v1, const Vertex& v2,
    const UVCoordinate texture_coord_0, const UVCoordinate texture_coord_1,
    const UVCoordinate texture_coord_2, const Normal normal0,
    const Normal normal1, const Normal normal3, const Vertex& vg1,
    const Vertex& vg2, const Vertex& vg3, const Light& light,
    const Material& material, int yLo, int yHi, unsigned char* bits,
    qsizetype bpl, int W, int H) {
  Eigen::Vector2i p0(v0.x(), v0.y());
  Eigen::Vector2i p1(v1.x(), v1.y());
  Eigen::Vector2i p2(v2.x(), v2.y());

  int minX = std::max(0, std::min({p0.x(), p1.x(), p2.x()}));
  int maxX = std::min(W - 1, std::max({p0.x(), p1.x(), p2.x()}));
  // Y зажимаем и буфером, и границами текущей полосы [yLo, yHi).
  int minY = std::max(yLo, std::min({p0.y(), p1.y(), p2.y()}));
  int maxY = std::min(yHi - 1, std::max({p0.y(), p1.y(), p2.y()}));
  if (minX > maxX || minY > maxY) return;

  float area = triangleArea(p0, p1, p2);
  if (area <= 0.0f) return;  // вырожденный треугольник

  const bool useTexture =
      m_settings.texture && !material.texture.colors_.empty();

  for (int x = minX; x <= maxX; ++x) {
    std::vector<float>& depthCol = depthBuffer[x];  // непрерывно по y
    for (int y = minY; y <= maxY; ++y) {
      Eigen::Vector2i p(x, y);
      Eigen::Vector3f baryCoords = barycentricCoords(p, p0, p1, p2, area);
      if (baryCoords[0] < 0 || baryCoords[1] < 0 || baryCoords[2] < 0 ||
          std::abs(baryCoords.sum() - 1.0f) >= 1e-6f)
        continue;

      float depth = interpolate(v0.z(), v1.z(), v2.z(), baryCoords);
      if (depth >= depthCol[y]) continue;
      depthCol[y] = depth;

      Normal interpolateNormal =
          interpolate(normal0, normal1, normal3, baryCoords);
      Vertex interpolateGlobalVertex = interpolate(vg1, vg2, vg3, baryCoords);
      Color lightColor = calculatePhongIlluminationForVertex(
          interpolateGlobalVertex, interpolateNormal, material, light,
          Vector3F{0.0f, 0.0f, 150.0f});

      Color finalColor = lightColor;
      if (useTexture) {
        UVCoordinate w0 = texture_coord_0 / v0.w();
        UVCoordinate w1 = texture_coord_1 / v1.w();
        UVCoordinate w2 = texture_coord_2 / v2.w();
        UVCoordinate uv = interpolate(w0, w1, w2, baryCoords);
        uv *= (1.0f / interpolate(1.0f / v0.w(), 1.0f / v1.w(), 1.0f / v2.w(),
                                  baryCoords));
        int x1 = std::clamp(
            static_cast<int>(uv.x() * (material.texture.width_ - 1)), 0,
            material.texture.width_ - 1);
        int y1 = std::clamp(
            static_cast<int>(uv.y() * (material.texture.height_ - 1)), 0,
            material.texture.height_ - 1);
        const Color& texture_color =
            material.texture.colors_[y1 * material.texture.width_ + x1];
        finalColor = (lightColor / 255).cwiseProduct(texture_color / 255) * 255;
      }

      quint32* px = reinterpret_cast<quint32*>(bits + static_cast<qsizetype>(y) *
                                                          bpl);
      px[x] = 0xFF000000u |
              (static_cast<quint32>(std::clamp(int(finalColor[0]), 0, 255))
               << 16) |
              (static_cast<quint32>(std::clamp(int(finalColor[1]), 0, 255))
               << 8) |
              static_cast<quint32>(std::clamp(int(finalColor[2]), 0, 255));
    }
  }
}

Eigen::Vector3f RenderRasterize::calculatePhongIlluminationForVertex(
    const Vertex& v, const Normal& n, const Material& material,
    const Light& light, const Eigen::Vector3f& viewPos) {
  Eigen::Vector3f vertexPos = v.head<3>();
  Eigen::Vector3f lightDir = (light.position - vertexPos).normalized();

  Eigen::Vector3f ambient = calculateAmbient(material, light);
  Eigen::Vector3f diffuse = calculateDiffuse(material, light, n, lightDir);
  Eigen::Vector3f specular = Eigen::Vector3f::Zero();

  Eigen::Vector3f color = ambient + diffuse + specular;
  color = color.cwiseMin(255.0f).cwiseMax(0.0f);

  return color;
}

Eigen::Vector3f RenderRasterize::calculateAmbient(const Material& material,
                                                  const Light& light) {
  return material.ambient.cwiseProduct(light.color);
}

Eigen::Vector3f RenderRasterize::calculateDiffuse(
    const Material& material, const Light& light, const Normal& normal,
    const Eigen::Vector3f& lightDir) {
  float diff = std::max(normal.dot(lightDir), 0.0f);
  return material.diffuse.cwiseProduct(light.color) * diff;
}

void RenderRasterize::clipedObject(const Camera& camera, Object& cameraObj) {
  const Mesh& mesh = cameraObj.getMesh();
  float xmin = -1.0f, xmax = 1.0f;
  float ymin = -1.0f, ymax = 1.0f;
  float zmin = -1.0f, zmax = 1.0f;

  std::vector<Face> faces;

#pragma omp parallel
  {
    std::vector<Face> localFaces;

#pragma omp for nowait
    for (int i = 0; i < mesh.faces_.size(); i++) {
      const Face& face = mesh.faces_[i];
      Vertex v0 = mesh.vertices_[face.vertexIndex[0]];
      Vertex v1 = mesh.vertices_[face.vertexIndex[1]];
      Vertex v2 = mesh.vertices_[face.vertexIndex[2]];
      v0 /= v0.w();
      v1 /= v1.w();
      v2 /= v2.w();

      if (v0.x() < xmin || v0.x() > xmax || v0.y() < ymin || v0.y() > ymax ||
          v0.z() < zmin || v0.z() > zmax ||

          v1.x() < xmin || v1.x() > xmax || v1.y() < ymin || v1.y() > ymax ||
          v1.z() < zmin || v1.z() > zmax ||

          v2.x() < xmin || v2.x() > xmax || v2.y() < ymin || v2.y() > ymax ||
          v2.z() < zmin || v2.z() > zmax) {
        continue;
      }

      localFaces.push_back(face);
    }

#pragma omp critical
    faces.insert(faces.end(), localFaces.begin(), localFaces.end());
  }

  cameraObj.getMesh().faces_ = faces;
}

float RenderRasterize::triangleArea(const Eigen::Vector2i& p1,
                                    const Eigen::Vector2i& p2,
                                    const Eigen::Vector2i& p3) {
  return std::abs((p1.x() * (p2.y() - p3.y()) + p2.x() * (p3.y() - p1.y()) +
                   p3.x() * (p1.y() - p2.y())) /
                  2.0f);
}

Eigen::Vector3f RenderRasterize::barycentricCoords(const Eigen::Vector2i& p,
                                                   const Eigen::Vector2i& p0,
                                                   const Eigen::Vector2i& p1,
                                                   const Eigen::Vector2i& p2,
                                                   float area) {
  float alpha = triangleArea(p, p1, p2) / area;
  float beta = triangleArea(p0, p, p2) / area;
  float gamma = triangleArea(p0, p1, p) / area;
  return Eigen::Vector3f(alpha, beta, gamma);
}
}  // namespace s21