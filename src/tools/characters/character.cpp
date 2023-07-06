#include "character.h"
#include "filesystem/file.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace AsyncFuture;

Q_LOGGING_CATEGORY(gmCharactersCharacter, "gm.characters.character")

Character::Character(const QString &name, QObject *parent) : QObject(parent), a_name(name)
{
    qCDebug(gmCharactersCharacter()) << "Initializing new character:" << name << "...";
}

int Character::type() const
{
    if (m_files.size() < 1) return -1;

    if (m_files.at(0)->name().endsWith("pdf"_L1)) return 1;

    if (m_files.at(0)->name().endsWith("json"_L1)) return 2;

    return 0;
}

void Character::loadFiles()
{
    if (m_wasLoaded || folder().isEmpty() || !files().isEmpty())
    {
        emit fileListLoaded(files());
        return;
    }

    qCDebug(gmCharactersCharacter()) << "Loading files ...";

    m_wasLoaded = true;
    loadFileList();
}

void Character::loadFileList()
{
    observe(Files::File::listAsync(folder(), true, false)).subscribe([this](Files::FileListResult *result) {
        if (!result) return;

        foreach (const auto &fileName, result->files())
        {
            m_files.append(new CharacterFile(fileName, FileUtils::fileInDir(fileName, folder()), this));
        }

        emit fileListLoaded(m_files);
        result->deleteLater();
    });
}

void Character::loadFileData(int index)
{
    if (!Utils::isInBounds(files(), index)) return;

    if (!m_files[index]->data().isEmpty())
    {
        qCDebug(gmCharactersCharacter()) << "File data already loaded.";
        emit fileDataLoaded(index, m_files.at(index)->data());
        return;
    }

    observe(Files::File::getDataAsync(m_files.at(index)->path()))
        .subscribe([this, index](Files::FileDataResult *result) {
            if (!result) return;

            m_files.at(index)->data(result->data());
            emit fileDataLoaded(index, result->data());

            result->deleteLater();
        });
}
