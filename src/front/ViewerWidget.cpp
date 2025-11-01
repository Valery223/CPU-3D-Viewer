#include "ViewerWidget.hpp"

#include <QPainter>
#include <QResizeEvent>
#include <QTimer>

namespace s21 {
ViewerWidget::ViewerWidget(IController* controller, int width, int height,
                           QWidget* parent)
    : QWidget(parent), m_controller(controller) {
  setFocusPolicy(Qt::StrongFocus);
  setMinimumSize(400, 400);
  timer = new QTimer(this);

  connect(timer, &QTimer::timeout, [&]() {
    m_controller->updateModel();
    emit modelUpdated();
  });

  timer->start(1000 / 60);
  connect(this, &ViewerWidget::modelUpdated, this, &ViewerWidget::updateScene);

  m_image = QImage(width, height, QImage::Format_ARGB32);
  m_image.fill(Qt::black);
}

void ViewerWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawImage(0, 0, m_image);
}

void ViewerWidget::resizeEvent(QResizeEvent* event) {
  QSize newSize = event->size();
  m_controller->resizeBuffers(newSize.width(), newSize.height());
  QWidget::resizeEvent(event);
}

void ViewerWidget::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_R:
      m_controller->moveUp();
      break;
    case Qt::Key_F:
      m_controller->moveDown();
      break;
    case Qt::Key_A:
      m_controller->moveLeft();
      break;
    case Qt::Key_D:
      m_controller->moveRight();
      break;
    case Qt::Key_Q:
      m_controller->axiosRotateYLeft();
      break;
    case Qt::Key_E:
      m_controller->axiosRotateYRight();
      break;
    case Qt::Key_Z:
      m_controller->axiosRotateXLeft();
      break;
    case Qt::Key_X:
      m_controller->axiosRotateXRight();
      break;
    case Qt::Key_C:
      m_controller->axiosRotateZLeft();
      break;
    case Qt::Key_V:
      m_controller->axiosRotateZRight();
      break;
    case Qt::Key_W:
      m_controller->moveForward();
      break;
    case Qt::Key_S:
      m_controller->moveBackward();
      break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }
}

void ViewerWidget::updateScene() {
  m_image = m_controller->getImage();
  update();
}
}  // namespace s21