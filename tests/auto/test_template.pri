TEMPLATE = app
CONFIG += qmltestcase

QMLPLOTTING_IMPORTPATH = $$shadowed($$dirname(_QMAKE_CONF_)/qml)
DEFINES += QMLPLOTTING_IMPORTPATH=\"\\\"$$QMLPLOTTING_IMPORTPATH\\\"\"
IMPORTPATH += $$QMLPLOTTING_IMPORTPATH
