#include <QApplication>

#include "backend/render/renderRasterize.h"
#include "backend/render/renderSettings.hpp"
#include "backend/scene/scene.h"
#include "controller/IController.hpp"
#include "controller/controller.h"
#include "front/MainWidget.hpp"

using namespace s21;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  RenderSettings renderSettings;
  renderSettings.loadFromFile("test.txt");
  RenderRasterize render(renderSettings);

  Scene scene;

  IController *controller = new Controller(&scene, &render);

  MainWidget mainWidget(controller);
  mainWidget.show();

  app.exec();

  renderSettings.saveToFile("test.txt");
  delete controller;

  return 0;
}