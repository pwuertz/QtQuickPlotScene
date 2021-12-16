#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGRendererInterface>
#include <QtQuickControls2/QQuickStyle>


int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
  QQuickStyle::setStyle("Material");
  QQmlApplicationEngine engine("qrc:/demo.qml");
  return app.exec();
}
