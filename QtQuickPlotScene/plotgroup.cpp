#include "plotgroup.hpp"
#include <QtCore/QDebug>

namespace {

inline void addToGroup(PlotGroup *group, QQuickItem *plotItem) {
  // Reparent and anchor to plot group size
  plotItem->setParentItem(group);
  plotItem->setPosition({0., 0.});
  plotItem->setSize({group->width(), group->height()});
  // Initialize properties from plot group
  plotItem->setProperty("viewRect", group->viewRect());
  plotItem->setProperty("logY", group->logY());
}

} // namespace

PlotGroup::PlotGroup(QQuickItem *parent) : QQuickItem(parent) {}

QQmlListProperty<QQuickItem> PlotGroup::plotItems()
{
  using ListProperty = QQmlListProperty<QQuickItem>;
  const auto append = [](ListProperty *list, QQuickItem *plotItem) {
    auto *self = reinterpret_cast<PlotGroup *>(list->data);
    self->m_plotItems.append(plotItem);
    addToGroup(self, plotItem);
  };
  const auto count = [](ListProperty *list) -> qsizetype {
    auto *self = reinterpret_cast<PlotGroup *>(list->data);
    return self->m_plotItems.count();
  };
  const auto at = [](ListProperty *list, const qsizetype index) -> QQuickItem * {
    auto *self = reinterpret_cast<PlotGroup *>(list->data);
    return self->m_plotItems.at(index);
  };
  const auto clear = [](ListProperty *list) {
    auto *self = reinterpret_cast<PlotGroup *>(list->data);
    self->m_plotItems.clear();
  };
  const auto replace = [](ListProperty *list, const qsizetype index, QQuickItem *plotItem) {
    auto *self = reinterpret_cast<PlotGroup *>(list->data);
    self->m_plotItems[index] = plotItem;
    addToGroup(self, plotItem);
  };
  const auto removeLast = [](ListProperty *list) {
    auto *self = reinterpret_cast<PlotGroup *>(list->data);
    self->m_plotItems.removeLast();
  };
  return {this, this, append, count, at, clear, replace, removeLast};
}

void PlotGroup::setViewMode(const ViewMode viewMode)
{
  if (std::exchange(m_viewMode, viewMode) == viewMode) {
    return;
  }
  updateViewRects();
  Q_EMIT viewModeChanged(viewMode);
}

void PlotGroup::setAspectRatio(double aspectRatio)
{
  aspectRatio = std::max(aspectRatio, 0.);
  if (std::exchange(m_aspectRatio, aspectRatio) == aspectRatio) {
    return;
  }
  if (m_viewMode != ViewMode::AutoAspect) {
    updateViewRects();
  }
  Q_EMIT aspectRatioChanged(aspectRatio);
}

void PlotGroup::setViewRect(const QRectF &viewRect)
{
  // Set preferred view rect
  if (std::exchange(m_preferredViewRect, viewRect) == viewRect) {
    return;
  }
  // Trigger internal view rect update
  updateViewRects();
}

void PlotGroup::updateViewRects()
{
  // Set actual view rect from preferred, possibly adjusted view rect
  const QRectF adjustedViewRect = adjustViewRect(m_preferredViewRect);
  if (std::exchange(m_viewRect, adjustedViewRect) == adjustedViewRect) {
    return;
  }
  // Update all plot items
  for (auto &item : m_plotItems) {
    item->setProperty("viewRect", m_viewRect);
  }
  Q_EMIT viewRectChanged(m_viewRect);
}

void PlotGroup::setLogY(const bool logY)
{
  if (std::exchange(m_logY, logY) == logY) {
    return;
  }
  // Forward logY change to plot items
  for (auto &item : m_plotItems) {
    item->setProperty("logY", logY);
  }
  Q_EMIT logYChanged(logY);
}

void PlotGroup::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
  QQuickItem::geometryChange(newGeometry, oldGeometry);
  // Resize plot items to new item size
  for (auto &item : m_plotItems) {
    item->setSize({width(), height()});
  }
  // Update view rects if size change triggers aspect adjustment
  if (m_viewMode != ViewMode::AutoAspect) {
    updateViewRects();
  }
}

QRectF PlotGroup::adjustViewRect(const QRectF &viewRect) const
{
  // TODO: Support aspect != 1
  // Aspect correction requires valid item size
  if (!(width() > 0 && height() > 0)) {
    return viewRect;
  }
  // Return unchanged rect in auto mode
  if (m_viewMode == ViewMode::AutoAspect) {
    return viewRect;
  }

  const double sourceAspect = viewRect.width() / viewRect.height();
  const double targetAspect = width() / height();

  const auto fitX = [&]() -> QRectF {
    const double targetHeight = viewRect.width() / targetAspect;
    const double delta = targetHeight - viewRect.height();
    return {viewRect.x(), viewRect.y() - 0.5 * delta, viewRect.width(), targetHeight};
  };

  const auto fitY = [&]() -> QRectF {
    const double targetWidth = viewRect.height() * targetAspect;
    const double delta = targetWidth - viewRect.width();
    return {viewRect.x() - 0.5 * delta, viewRect.y(), targetWidth, viewRect.height()};
  };

  switch (m_viewMode) {
  case ViewMode::PreserveAspectFit:
    return (targetAspect >= sourceAspect) ? fitY() : fitX();
  case ViewMode::PreserveAspectCrop:
    return (targetAspect >= sourceAspect) ? fitX() : fitY();
  default:
    return viewRect;
  }
}
