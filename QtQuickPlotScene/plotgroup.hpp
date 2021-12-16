#pragma once
#include <QtQuick/QQuickItem>

class PlotGroup : public QQuickItem
{
  Q_OBJECT
  QML_ELEMENT

public:
  enum class ViewMode { AutoAspect, PreserveAspectFit, PreserveAspectCrop };
  Q_ENUM(ViewMode)

public:
  Q_PROPERTY(ViewMode viewMode MEMBER m_viewMode WRITE setViewMode NOTIFY viewModeChanged)
  Q_PROPERTY(double aspectRatio MEMBER m_aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
  Q_PROPERTY(QRectF viewRect MEMBER m_viewRect WRITE setViewRect NOTIFY viewRectChanged)
  Q_PROPERTY(bool logY MEMBER m_logY WRITE setLogY NOTIFY logYChanged)
  Q_PROPERTY(QQmlListProperty<QQuickItem> plotItems READ plotItems CONSTANT)

  Q_SIGNAL void viewModeChanged(PlotGroup::ViewMode viewMode);
  Q_SIGNAL void aspectRatioChanged(double aspectRatio);
  Q_SIGNAL void viewRectChanged(const QRectF& viewRect);
  Q_SIGNAL void logYChanged(bool logY);

  Q_INVOKABLE void setViewMode(PlotGroup::ViewMode viewMode);
  Q_INVOKABLE void setAspectRatio(double aspectRatio);
  Q_INVOKABLE void setViewRect(const QRectF& viewRect);
  Q_INVOKABLE void setLogY(bool logY);

public:
  explicit PlotGroup(QQuickItem* parent = nullptr);

  [[nodiscard]] QQmlListProperty<QQuickItem> plotItems();
  [[nodiscard]] QRectF viewRect() const noexcept { return m_viewRect; }
  [[nodiscard]] bool logY() const noexcept { return m_logY; }

protected:
  void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
  QRectF adjustViewRect(const QRectF& viewRect) const;
  void updateViewRects();

private:
  ViewMode m_viewMode = ViewMode::AutoAspect;
  double m_aspectRatio = 1.0;
  QRectF m_viewRect = {0., 0., 1., 1.};
  QRectF m_preferredViewRect = {0., 0., 1., 1.};
  bool m_logY = false;
  QVector<QQuickItem*> m_plotItems;
};
