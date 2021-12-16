#include "lineplot.hpp"
#include <utility>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>


LinePlot::LinePlot(QQuickItem* parent) : QQuickItem{parent}
{
  setFlag(QQuickItem::Flag::ItemHasContents);
}

void LinePlot::setViewRect(const QRectF& viewRect)
{
  if (std::exchange(m_viewRect, viewRect) == viewRect) { return; }
  update();
  Q_EMIT viewRectChanged(viewRect);
}

void LinePlot::setLogY(const bool logY)
{
  if (std::exchange(m_logY, logY) == logY) { return; }
  update();
  Q_EMIT logYChanged(logY);
}

void LinePlot::setLineWidth(const double lineWidth)
{
  if (std::exchange(m_lineWidth, lineWidth) == lineWidth) { return; }
  update();
  Q_EMIT lineWidthChanged(lineWidth);
}

void LinePlot::setLineColor(const QColor& lineColor)
{
  if (std::exchange(m_lineColor, lineColor) == lineColor) { return; }
  update();
  Q_EMIT lineColorChanged(lineColor);
}

void LinePlot::setCoordinates(const QVector<qreal>& coordinates)
{
  m_coordinates = coordinates;
  m_coordinatesModified = true;
  update();
  Q_EMIT coordinatesChanged(m_coordinates);
}

void LinePlot::setClosed(const bool closed)
{
  if (std::exchange(m_closed, closed) == closed) { return; }
  update();
  Q_EMIT closedChanged(closed);
}

using NodeType = QSGGeometryNode;
using MaterialType = QSGFlatColorMaterial;
using GeometryType = QSGGeometry;

QSGNode* LinePlot::createNode() const
{
  auto* geometry = new GeometryType(QSGGeometry::defaultAttributes_Point2D(), 0);
  geometry->setDrawingMode(m_closed ? QSGGeometry::DrawLineLoop : QSGGeometry::DrawLineStrip);
  geometry->setLineWidth(m_lineWidth);

  auto* material = new MaterialType;
  material->setColor(m_lineColor);

  auto* node = new NodeType;
  node->setGeometry(geometry);
  node->setFlag(QSGNode::OwnsGeometry);
  node->setMaterial(material);
  node->setFlag(QSGNode::OwnsMaterial);

  return node;
}

QSGNode* LinePlot::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* paintData)
{
  auto* node = static_cast<NodeType*>((oldNode) ? oldNode : createNode());
  auto* material = static_cast<MaterialType*>(node->material());
  auto* geometry = static_cast<GeometryType*>(node->geometry());

  // Apply view rect transformation
  auto transformMat = QMatrix4x4();
  transformMat.translate(0, height());
  transformMat.scale(width() / m_viewRect.width(), -height() / m_viewRect.height());
  transformMat.translate(-m_viewRect.left(), -m_viewRect.top());
  paintData->transformNode->setMatrix(transformMat);

  // Update coordinates if required
  if (m_coordinatesModified) {
    const auto n = m_coordinates.size() / 2;
    node->geometry()->allocate(n);
    auto* points = node->geometry()->vertexDataAsPoint2D();
    if (m_logY) {
      for (qsizetype i = 0; i < n; ++i) {
        points[i].set(m_coordinates[i*2 + 0], std::log10(m_coordinates[i*2 + 1]));
      }
    } else {
      for (qsizetype i = 0; i < n; ++i) {
        points[i].set(m_coordinates[i*2 + 0], m_coordinates[i*2 + 1]);
      }
    }
    node->geometry()->markVertexDataDirty();
    node->markDirty(QSGNode::DirtyStateBit::DirtyGeometry);
    m_coordinatesModified = false;
  }

  // Check for geometry changes
  const auto drawingMode = m_closed ? QSGGeometry::DrawLineLoop : QSGGeometry::DrawLineStrip;
  if (geometry->drawingMode() != drawingMode || geometry->lineWidth() != m_lineWidth) {
    geometry->setDrawingMode(drawingMode);
    geometry->setLineWidth(m_lineWidth);
    node->markDirty(QSGNode::DirtyStateBit::DirtyGeometry);
  }

  // Check for material changes
  if (material->color() != m_lineColor) {
    material->setColor(m_lineColor);
    node->markDirty(QSGNode::DirtyStateBit::DirtyMaterial);
  }

  return node;
}
