#pragma once
#include <QtCore/QObject>
#include <QtQuick/QQuickItem>

class Utils : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit Utils(QObject *parent = nullptr);

  /**
   * @brief niceNumPrec Return 'nice' number and precision suggestion from
   * input.
   * @param number Input number.
   * @param logMode Return result suitable for log scale.
   * @return Nice number and suggested precision.
   */
  Q_INVOKABLE QPointF niceNumPrec(qreal number, bool logMode) const noexcept {
    const auto [nice, prec] = calcNiceNum(number, logMode);
    return {nice, prec};
  }

  static std::pair<qreal, qreal> calcNiceNum(qreal number, bool logMode) noexcept;
};
