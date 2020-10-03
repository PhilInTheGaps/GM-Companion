#include "audioelement.h"
#include "logging.h"
#include <QJsonArray>

AudioElement::AudioElement(const QString& name, Type type, const QString& path, QObject *parent)
    : QObject(parent), m_name(name), m_type(type)
{
    m_path = path + "/" + typeToString(type) + "/" + name;
    m_icon = new AudioIcon(path, this);
}

AudioElement::AudioElement(QJsonObject object, Type type, const QString& path, QObject *parent)
    : QObject(parent)
{
    m_name             = object["name"].toString();
    m_relativeIconPath = object["icon"].toString();
    m_mode             = object["mode"].toInt();
    m_path             = path + "/" + typeToString(type) + "/" + m_name;
    m_icon             = new AudioIcon(m_path, this);
    m_type             = type;

    for (auto file : object.value("files").toArray())
    {
        m_files.append(new AudioFile(file.toObject(), this));
    }
}

/**
 * @brief Serialize AudioElement to json
 */
auto AudioElement::toJson() const -> QJsonObject
{
    QJsonObject object;
    object.insert("name", m_name);
    object.insert("icon", m_relativeIconPath);
    object.insert("mode", m_mode);

    // Files
    QJsonArray files;
    for (auto *file : m_files)
    {
        files.append(file->toJson());
    }
    object.insert("files", files);
    return object;
}

auto AudioElement::typeToString(AudioElement::Type type) -> QString
{
    return QVariant::fromValue(type).toString();
}

/**
 * @brief Compare two audio elements, true if e1 comes alphabetically before e2.
 */
auto AudioElement::compare(AudioElement *e1, AudioElement *e2) -> bool
{
    if (e1 && e2) return e1->name() < e2->name();
    return true;
}
