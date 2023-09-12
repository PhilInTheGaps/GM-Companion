#pragma once

#include "file.h"
#include "qmlsingletonfactory.h"
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

namespace Files
{

class FileAccessSwitcher : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_SINGLETON_FACTORY(FileAccessSwitcher)

public:
    FileAccessSwitcher() = delete;
    static auto instance() -> FileAccessSwitcher *
    {
        if (!single)
        {
            single = new FileAccessSwitcher(nullptr);
        }
        return single;
    }

    Q_INVOKABLE static void updateFileAccess()
    {
        File::updateFileAccess();
    }

private:
    using QObject::QObject;

    inline static FileAccessSwitcher *single = nullptr;
};

} // namespace Files
