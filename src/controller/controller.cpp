#include "controller.h"

#include <chrono>

namespace s21 {
QImage Controller::getImage() { return render->getImage(); }

void Controller::changeRenderDotSetting(bool enable, Color color, int size,
                                        bool circulDot) {
  RenderSettings settings = render->getSettings();
  settings.renderDot = enable;
  settings.vertexColor = color;
  settings.vertexSize = size;
  settings.cirul = circulDot;

  render->setSettings(settings);
}

void Controller::changeRenderLineSetting(bool enable, Color color,
                                         bool dashed) {
  RenderSettings settings = render->getSettings();
  settings.renderLine = enable;
  settings.lineColor = color;
  settings.lineDash = dashed;

  render->setSettings(settings);
}

void Controller::changeRenderFaceSetting(bool enable, bool texture) {
  RenderSettings settings = render->getSettings();

  settings.renderFace = enable;
  settings.texture = texture;
  render->setSettings(settings);
}

void Controller::updateModel() {
#ifdef LOG_TIME
  auto start = std::chrono::high_resolution_clock::now();
#endif
  render->rendering(*scene);
#ifdef LOG_TIME
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::micro> duration = end - start;

  // Вывод времени выполнения на экран
  std::cout << "Общее выполнения: " << duration.count()
            << " микросекунд.--------" << std::endl;
#endif
}

void Controller::move(float x, float y, float z) {
  scene->getObjects()[0].translate(x, y, z);
}

void Controller::axiosRotate(float angleX, float angleY, float angleZ) {
  scene->getObjects()[0].rotate(Eigen::Vector3f(angleX, angleY, angleZ));
}

void Controller::moveUp(float angle) { move(0, angle, 0); }

void Controller::moveDown(float angle) { move(0, -angle, 0); }

void Controller::moveLeft(float angle) { move(-angle, 0, 0); }

void Controller::moveRight(float angle) { move(angle, 0, 0); }

void Controller::moveForward(float angle) { move(0, 0, -angle); }

void Controller::moveBackward(float angle) { move(0, 0, angle); }

void Controller::axiosRotateXLeft(float angle) {
  axiosRotate(2 * 3.1415 / (-angle), 0, 0);
}

void Controller::axiosRotateYLeft(float angle) {
  axiosRotate(0, 2 * 3.1415 / (-angle), 0);
}

void Controller::axiosRotateZLeft(float angle) {
  axiosRotate(0, 0, 2 * 3.1415 / (-angle));
}

void Controller::axiosRotateYRight(float angle) {
  axiosRotate(0, 2 * 3.1415 / angle, 0);
}

void Controller::axiosRotateZRight(float angle) {
  axiosRotate(0, 0, 2 * 3.1415 / angle);
}

void Controller::axiosRotateXRight(float angle) {
  axiosRotate(2 * 3.1415 / angle, 0, 0);
}

void Controller::axiosScale(float x, float y, float z) {
  scene->getObjects()[0].scale(x, y, z);
}

void Controller::axiosScaleMore(float scaleValue) {
  axiosScale(scaleValue, scaleValue, scaleValue);
}

void Controller::axiosScaleLess(float scaleValue) {
  axiosScale(scaleValue, scaleValue, scaleValue);
}
}  // namespace s21