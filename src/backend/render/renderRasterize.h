#ifndef RENDER_RASTERIZE
#define RENDER_RASTERIZE

#include <mutex>
#include <vector>

#include "backend/render/irender.h"

namespace s21 {
/**
 * @class RenderRasterize
 * @brief Класс для растеризационного рендеринга сцены.
 */
class RenderRasterize : public IRender {
 public:
  /**
   * @brief Конструктор класса RenderRasterize.
   * @param settings Параметры рендеринга.
   * @param width Ширина буфера рендеринга (по умолчанию 800).
   * @param height Высота буфера рендеринга (по умолчанию 600).
   */
  RenderRasterize(RenderSettings& settings, int width = 800, int height = 600);

  /**
   * @brief Выполняет рендеринг сцены.
   * @param scene Сцена для рендеринга.
   */
  void rendering(Scene& scene) override;

 private:
  /**
   * @brief Преобразует объект в мировые координаты.
   */
  void transformToWorldCoordinates(const Object& objInput, Object& objOutput);

  /**
   * @brief Выполняет отсечение невидимых граней методом backface culling.
   */
  std::vector<Face> performBackfaceCullingParallel(
      const std::vector<Face>& faces, const std::vector<Vertex>& vertices,
      const Vector3F& viewDir);

  /**
   * @brief Преобразует объект в координаты камеры.
   */
  void transformToCameraCoordinates(const Camera& camera,
                                    const Object& objInput, Object& objOutput);

  /**
   * @brief Проецирует объект в координаты камеры.
   */
  void projectToCamera(const Camera& camera, const Object& objInput,
                       Object& objOutput);

  /**
   * @brief Проецирует объект на экранные координаты.
   */
  void projectToScreen(const Object& objInput,
                       std::vector<Vertex>& screenVertex);

  /**
   * @brief Растеризует меш с использованием второго метода.
   */
  void rasterizeMesh2(const Mesh& mesh, std::vector<Vertex>& screenVertex,
                      const Scene& scene);

  /**
   * @brief Отрисовывает точку в виде круга.
   */
  void drawPointAsCircle(const Vertex& center);

  /**
   * @brief Растеризует меш, используя переданные вершины.
   */
  void rasterizeMesh(const Mesh& mesh, std::vector<Vertex>& screenVertex,
                     const std::vector<Vertex>& globalVertex,
                     const Scene& scene);

  /**
   * @brief Рисует линию между двумя точками.
   */
  void drawLine(const Vertex& p1, const Vertex& p2);

  /**
   * @brief Рисует треугольник с учетом освещения и текстурирования.
   */
  void drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2,
                    const UVCoordinate texture_coord_0,
                    const UVCoordinate texture_coord_1,
                    const UVCoordinate texture_coord_2, const Normal normal0,
                    const Normal normal1, const Normal normal3,
                    const Vertex& vg1, const Vertex& vg2, const Vertex& vg3,
                    const Light& light, const Material& material);

  /**
   * @brief Вычисляет освещение по модели Фонга для вершины.
   */
  Eigen::Vector3f calculatePhongIlluminationForVertex(
      const Vertex& v, const Normal& n, const Material& material,
      const Light& light, const Eigen::Vector3f& viewPos);

  /**
   * @brief Выполняет отсечение объекта.
   */
  void clipedObject(const Camera& camera, Object& cameraObj);

  /**
   * @brief Отрисовывает точку в виде квадрата.
   */
  void drawPointAsSquar(const Vertex& center);

  /**
   * @brief Проецирует объект в координаты камеры и выполняет отсечение.
   */
  Object projectToCameraCoordinatesAndClip(const Object& globalObject,
                                           Camera& camera);

  /**
   * @brief Проецирует вершины в координаты камеры.
   */
  void projectToCameraCoordinates(const std::vector<Vertex>& worldVertices,
                                  std::vector<Vertex>& screenVertices,
                                  const Camera& camera);

  /**
   * @brief Проецирует вершины в экранные координаты.
   */
  void projectToScreenCoordinates(const std::vector<Vertex>& screenVertices,
                                  std::vector<Vertex>& finalScreenVertices,
                                  const Camera& camera);

 private:
  /**
   * @brief Вычисляет амбиентное освещение.
   */
  Eigen::Vector3f calculateAmbient(const Material& material,
                                   const Light& light);

  /**
   * @brief Вычисляет диффузное освещение.
   */
  Eigen::Vector3f calculateDiffuse(const Material& material, const Light& light,
                                   const Normal& normal,
                                   const Eigen::Vector3f& lightDir);

  /**
   * @brief Вычисляет площадь треугольника.
   */
  float triangleArea(const Eigen::Vector2i& p1, const Eigen::Vector2i& p2,
                     const Eigen::Vector2i& p3);

  /**
   * @brief Вычисляет барицентрические координаты.
   */
  Eigen::Vector3f barycentricCoords(const Eigen::Vector2i& p,
                                    const Eigen::Vector2i& p0,
                                    const Eigen::Vector2i& p1,
                                    const Eigen::Vector2i& p2, float area);

  /**
   * @brief Интерполирует значение с использованием барицентрических координат.
   */
  template <typename T>
  T interpolate(const T& v0, const T& v1, const T& v2,
                const Eigen::Vector3f& baryCoords);
};

/**
 * @brief Шаблонная функция интерполяции значений.
 */
template <typename T>
T RenderRasterize::interpolate(const T& v0, const T& v1, const T& v2,
                               const Eigen::Vector3f& baryCoords) {
  return baryCoords[0] * v0 + baryCoords[1] * v1 + baryCoords[2] * v2;
}
}  // namespace s21
#endif