#pragma once

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include "file.h"

namespace Files {

class FileAccessSwitcher : public QObject
{
    Q_OBJECT
public:
    static auto instance() -> FileAccessSwitcher*
    {
        if (!single)
        {
            single = new FileAccessSwitcher(nullptr);
        }
        return single;
    }

    static auto QmlSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject *
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return instance();
    }

    Q_INVOKABLE void updateFileAccess()
    {
        File::updateFileAccess();
    }

private:
    FileAccessSwitcher(QObject *parent) : QObject(parent) {}

    inline static FileAccessSwitcher *single = nullptr;
};

}
