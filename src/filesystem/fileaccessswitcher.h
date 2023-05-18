#pragma once

#include "file.h"
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

namespace Files
{

class FileAccessSwitcher : public QObject
{
    Q_OBJECT
public:
    static auto instance() -> FileAccessSwitcher *
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

    Q_INVOKABLE static void updateFileAccess()
    {
        File::updateFileAccess();
    }

private:
    using QObject::QObject;

    inline static FileAccessSwitcher *single = nullptr;
};

} // namespace Files
