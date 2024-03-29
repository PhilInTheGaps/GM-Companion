cmake_minimum_required(VERSION 3.19)

# set QML import path
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_BINARY_DIR}/../)
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

# configure markdown lib
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utils/markdownutils.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/utils/markdownutils.h @ONLY)

set(SRC_UTILS
    utils/utils.h
    utils/utils.cpp
    utils/fileutils.h
    utils/fileutils.cpp
    utils/markdownutils.h
    utils/markdownutils.h.in
    utils/markdownutils.cpp
    utils/networkutils.h
    utils/networkutils.cpp
    utils/processinfo.h
    utils/processinfo.cpp
    utils/stringutils.h
    utils/stringutils.cpp)

set(SRC_MODELS
    models/treeitem.h
    models/treeitem.cpp
    models/customobjectlistmodel.h
    models/customobjectlistmodel.cpp
    models/customsharedptrlistmodel.h
    models/baseprojectitem.h
    models/baseprojectitem.cpp)

set(SRC_SETTINGS
    settings/settingsmanager.h
    settings/settingsmanager.cpp
    settings/abstractsettingsmanager.h
    settings/abstractsettingsmanager.cpp
    settings/quicksettingsmanager.h
    settings/quicksettingsmanager.cpp)

set(SRC_UPDATES
    updates/updatemanager.h
    updates/updatemanager.cpp
    updates/version.h.in
    updates/version.h
    updates/version.cpp)

set(SRC_PROJECTS
    projects/abstractprojectupgrader.h
    projects/abstractprojectupgrader.cpp
    projects/abstractprojectupgraderjson.h
    projects/abstractprojectupgraderjson.cpp
    projects/abstractprojectupgraderini.h
    projects/abstractprojectupgraderini.cpp)

set(SOURCES
    ${SRC_UTILS}
    ${SRC_MODELS}
    ${SRC_SETTINGS}
    ${SRC_UPDATES}
    ${SRC_PROJECTS}
    abstracttool.h
    abstracttool.cpp
    qmlsingletonfactory.h
    exceptions/notimplementedexception.h)

qt_add_library(common STATIC)
qt_add_qml_module(common
    URI common
    SOURCES ${SOURCES}
)

target_precompile_headers(common PRIVATE
    <QtGlobal>
    <QObject>
    <QCoreApplication>
    <QString>
    <QStringList>
    <QByteArray>
    <QBuffer>
    <QTextStream>
    <QDir>
    <QFile>
    <QFileInfo>
    <QVariant>
    <QHash>
    <QMap>
    <QQueue>
    <QList>
    <QAbstractListModel>
    <QSettings>
    <QTemporaryFile>
    <QTemporaryDir>
    <QLoggingCategory>
    <QJsonDocument>
    <QJsonObject>
    <QJsonArray>
    <QFuture>
    <QPromise>
    <QPointer>
    <QSharedPointer>
    <QQmlEngine>
    <QQmlContext>
    <QJSEngine>
    <QtQml>
    <QtQml/qqmlregistration.h>
    <QNetworkAccessManager>
    <QNetworkRequest>
    <QNetworkReply>
    <QHttpMultiPart>
    <QUrl>
    <QUrlQuery>
    <QRegularExpression>
    <QPixmap>
    <QImage>
    <QTime>
    <QDateTime>
    <QTimer>
    <QRandomGenerator>
    <QProcess>
    <QThread>
    <utility>
    <memory>
    <vector>
    <iostream>
    <algorithm>
    <stdexcept>
    <chrono>
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/updates
        ${CMAKE_CURRENT_SOURCE_DIR}/settings)

target_link_libraries(common
    PUBLIC
    propertyhelper
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Network
    PRIVATE
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY})
