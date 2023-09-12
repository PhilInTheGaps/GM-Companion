#pragma once

#include <QJSEngine>
#include <QQmlEngine>

#define QML_SINGLETON_FACTORY(TYPE)                                                                                    \
public:                                                                                                                \
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)->TYPE *                                             \
    {                                                                                                                  \
        Q_UNUSED(jsEngine)                                                                                             \
        if (qmlEngine->thread() != instance()->thread())                                                               \
        {                                                                                                              \
            instance()->moveToThread(qmlEngine->thread());                                                             \
        }                                                                                                              \
        QJSEngine::setObjectOwnership(instance(), QJSEngine::CppOwnership);                                            \
        return instance();                                                                                             \
    }

#define QML_ONLY_SINGLETON_FACTORY(TYPE)                                                                               \
public:                                                                                                                \
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)->TYPE *                                             \
    {                                                                                                                  \
        Q_UNUSED(jsEngine)                                                                                             \
        auto *instance = qmlInstance(qmlEngine);                                                                       \
        if (qmlEngine->thread() != instance->thread())                                                                 \
        {                                                                                                              \
            instance->moveToThread(qmlEngine->thread());                                                               \
        }                                                                                                              \
        QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);                                              \
        return instance;                                                                                               \
    }
