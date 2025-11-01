#pragma once

#include <qmutex.h>

#include <QImage>
#include <vector>

#include "backend/scene/scene.h"
#include "backend/types.h"
#include "renderSettings.hpp"

namespace s21 {
class Scene;

/**
 * @class IRender
 * @brief Абстрактный класс для рендеринга сцены.
 */
class IRender {
 public:
  /**
   * @brief Конструктор IRender.
   * @param settings Параметры рендеринга.
   * @param width Ширина изображения (по умолчанию 800).
   * @param height Высота изображения (по умолчанию 600).
   */
  IRender(RenderSettings& settings, int width = 800, int height = 600)
      : _frontBuffer(width, height, QImage::Format_ARGB32),
        _backBuffer(width, height, QImage::Format_ARGB32),
        depthBuffer(width, std::vector<float>(height, 1.0f)),
        m_settings(settings) {
    _frontBuffer.fill(m_settings.fon_color);
    _backBuffer.fill(m_settings.fon_color);
  }

 public:
  /**
   * @brief Абстрактный метод рендеринга сцены.
   * @param _scene Сцена для рендеринга.
   */
  virtual void rendering(Scene& _scene) = 0;

  /**
   * @brief Обмен переднего и заднего буферов.
   */
  void swapBuffers() { _frontBuffer.swap(_backBuffer); }

  /**
   * @brief Изменяет размер буферов рендеринга.
   * @param width Новая ширина буфера.
   * @param height Новая высота буфера.
   * @param fon_color Новый цвет фона (по умолчанию Qt::white).
   */
  void resizeBuffers(int width, int height,
                     Qt::GlobalColor fon_color = Qt::white) {
    QMutexLocker backLocker(&_backBufferMutex);
    QMutexLocker frontLocker(&_frontBufferMutex);
    _frontBuffer = _frontBuffer.scaled(width, height, Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation);
    _backBuffer = QImage(width, height, QImage::Format_ARGB32);
    depthBuffer = std::vector<std::vector<float>>(
        width, std::vector<float>(height, 1.0f));
  }

 public:
  /**
   * @brief Получает изображение переднего буфера.
   * @return Изображение QImage.
   */
  QImage getImage() {
    QMutexLocker locker(&_frontBufferMutex);
    return _frontBuffer;
  }

  /**
   * @brief Получает настройки рендеринга.
   * @return Текущие настройки рендеринга.
   */
  RenderSettings getSettings() { return m_settings; }

  /**
   * @brief Устанавливает новые настройки рендеринга.
   * @param settings Новые настройки.
   */
  void setSettings(RenderSettings settings) {
    QMutexLocker backLocker(&_backBufferMutex);
    m_settings = settings;
  }

 protected:
  /**
   * @brief Очищает изображение и буфер глубины.
   */
  void clearImage() {
    _backBuffer.fill(m_settings.fon_color);
    for (auto& row : depthBuffer) {
      std::fill(row.begin(), row.end(), 1.0f);
    }
  }

 protected:
  QImage _frontBuffer;  ///< Передний буфер (то, что видит пользователь)
  QImage _backBuffer;  ///< Задний буфер (для рендеринга)
  std::vector<std::vector<float>> depthBuffer;  ///< Буфер глубины

  RenderSettings& m_settings;  ///< Настройки рендеринга

  QMutex _frontBufferMutex;  ///< Мьютекс для переднего буфера
  QMutex _backBufferMutex;  ///< Мьютекс для заднего буфера
};
}  // namespace s21
