#include "MainWidget.hpp"

#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>

namespace s21 {
MainWidget::MainWidget(IController* controller, QWidget* parent)
    : QWidget(parent),
      viewer(nullptr),
      loadBtn(nullptr),
      moveControl(nullptr),
      rotateControl(nullptr),
      scaleControl(nullptr),
      pointSettingWidget(nullptr),
      lineSettingWidget(nullptr),
      faceSettingWidget(nullptr),
      m_controller(controller) {
  setupUi();
  setupConnections();
}

void MainWidget::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    qDebug() << "ESC pressed in MainWidget!";
    close();
  }
}

void MainWidget::setupUi() {
  QHBoxLayout* mainLayout = new QHBoxLayout(this);

  viewer = new ViewerWidget(m_controller, 800, 600, this);
  mainLayout->addWidget(viewer);

  QScrollArea* scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setFixedWidth(240);

  QWidget* settingsPanel = new QWidget();
  QVBoxLayout* settingsLayout = new QVBoxLayout(settingsPanel);
  settingsPanel->setMinimumWidth(220);

  loadBtn = new FileLoadButton(settingsPanel);
  settingsLayout->addWidget(loadBtn);

  QGroupBox* transformGroup = new QGroupBox("Transform", settingsPanel);
  QVBoxLayout* transformLayout = new QVBoxLayout(transformGroup);
  moveControl = new ControlGroupWidget(
      QString("Move"), QStringList() << "x" << "y" << "z", transformGroup);

  rotateControl = new ControlGroupWidget(
      QString("Rotate"), QStringList() << "angle_x" << "angle_y" << "angle_z",
      transformGroup);

  scaleControl = new ControlGroupWidget(
      QString("Scale"), QStringList() << "x" << "y" << "z", transformGroup);

  transformLayout->addWidget(moveControl);
  transformLayout->addWidget(rotateControl);
  transformLayout->addWidget(scaleControl);
  settingsLayout->addWidget(transformGroup);

  QGroupBox* pointGroup = new QGroupBox("Points", settingsPanel);
  pointSettingWidget = new PointSettingsWidget(pointGroup);
  QVBoxLayout* pointLayout = new QVBoxLayout(pointGroup);
  pointLayout->addWidget(pointSettingWidget);
  settingsLayout->addWidget(pointGroup);

  QGroupBox* lineGroup = new QGroupBox("Lines", settingsPanel);
  lineSettingWidget = new LineSettingsWidget(lineGroup);
  QVBoxLayout* lineLayout = new QVBoxLayout(lineGroup);
  lineLayout->addWidget(lineSettingWidget);
  settingsLayout->addWidget(lineGroup);

  QGroupBox* faceGroup = new QGroupBox("Faces", settingsPanel);
  faceSettingWidget = new FaceSettingsWidget(faceGroup);
  QVBoxLayout* faceLayout = new QVBoxLayout(faceGroup);
  faceLayout->addWidget(faceSettingWidget);
  settingsLayout->addWidget(faceGroup);

  settingsLayout->addStretch();
  scrollArea->setWidget(settingsPanel);
  mainLayout->addWidget(scrollArea);
}

void MainWidget::setupConnections() {
  connect(moveControl, &ControlGroupWidget::controlValues, this,
          [this](const QVector<double>& values) {
            m_controller->move(values[0], values[1], values[2]);
          });
  connect(rotateControl, &ControlGroupWidget::controlValues, this,
          [this](const QVector<double>& values) {
            m_controller->axiosRotate(values[0], values[1], values[2]);
          });
  connect(scaleControl, &ControlGroupWidget::controlValues, this,
          [this](const QVector<double>& values) {
            m_controller->axiosScale(values[0], values[1], values[2]);
          });
  connect(loadBtn, &FileLoadButton::fileSelected, this,
          [this](const QString& filePath) {
            m_controller->loadObject(filePath.toStdString());
          });

  connect(pointSettingWidget, &PointSettingsWidget::pointSettingsChanged, this,
          [this](bool enabled, const QColor& color, int size, bool circulDot) {
            m_controller->changeRenderDotSetting(
                enabled, Color(color.red(), color.green(), color.blue()), size,
                circulDot);
          });

  connect(lineSettingWidget, &LineSettingsWidget::lineSettingsChanged, this,
          [this](bool enabled, const QColor& color, bool isDashed) {
            m_controller->changeRenderLineSetting(
                enabled, Color(color.red(), color.green(), color.blue()),
                isDashed);
          });

  connect(faceSettingWidget, &FaceSettingsWidget::faceSettingsChanged, this,
          [this](bool enable, bool texture) {
            m_controller->changeRenderFaceSetting(enable, texture);
          });
}
}  // namespace s21