#pragma once
#include <QtQuick/QQuickItem>

class LinePlot : public QQuickItem
{
  Q_OBJECT
  QML_ELEMENT

public:
  explicit LinePlot(QQuickItem* parent = nullptr);

public:
  Q_PROPERTY(QRectF viewRect MEMBER m_viewRect WRITE setViewRect NOTIFY viewRectChanged)
  Q_PROPERTY(bool logY MEMBER m_logY WRITE setLogY NOTIFY logYChanged)
  Q_PROPERTY(qreal lineWidth MEMBER m_lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
  Q_PROPERTY(QColor lineColor MEMBER m_lineColor WRITE setLineColor NOTIFY lineColorChanged)
  Q_PROPERTY(bool closed MEMBER m_closed WRITE setClosed NOTIFY closedChanged)
  Q_PROPERTY(QVector<qreal> coordinates MEMBER m_coordinates WRITE setCoordinates NOTIFY coordinatesChanged)

  Q_SIGNAL void viewRectChanged(const QRectF& viewRect);
  Q_SIGNAL void logYChanged(bool logY);
  Q_SIGNAL void lineWidthChanged(qreal lineWidth);
  Q_SIGNAL void lineColorChanged(const QColor& color);
  Q_SIGNAL void coordinatesChanged(const QVector<qreal>& coordinates);
  Q_SIGNAL void closedChanged(bool closed);

  [[nodiscard]] QQmlListProperty<QPointF> points();
  void setViewRect(const QRectF& viewRect);
  void setLogY(bool logY);
  void setLineWidth(double lineWidth);
  void setLineColor(const QColor& lineColor);
  void setCoordinates(const QVector<qreal>& coordinates);
  void setClosed(bool closed);

protected:
  QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;
  QSGNode* createNode() const;

private:
  QRectF m_viewRect = {0, 0, 1, 1};
  bool m_logY = false;
  qreal m_lineWidth = 1.;
  QColor m_lineColor = {0, 0, 0};
  bool m_closed = false;
  QVector<qreal> m_coordinates;
  bool m_coordinatesModified = false;
};
