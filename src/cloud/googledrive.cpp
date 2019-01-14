#include "googledrive.h"
#include <QDebug>
#include "o0settingsstore.h"
#include "o2requestor.h"
#include "o0globals.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QUrlQuery>
#include <QNetworkReply>

GoogleDrive::GoogleDrive(QObject *parent) : QObject(parent)
{
    qDebug() << "Starting Google Drive ...";

    drive      = new O2Google;
    m_manager  = new QNetworkAccessManager;
    folderTree = new FolderTree({ sManager.getSetting(Setting::googlePath, "music"),
                                  sManager.getSetting(Setting::googlePath, "sounds"),
                                  sManager.getSetting(Setting::googlePath, "radio"),
                                  sManager.getSetting(Setting::googlePath, "audio"),
                                  sManager.getSetting(Setting::googlePath, "maps"),
                                  sManager.getSetting(Setting::googlePath, "shop"),
                                  sManager.getSetting(Setting::googlePath, "characters"),
                                  sManager.getSetting(Setting::googlePath, "notes"),
                                  sManager.getSetting(Setting::googlePath, "resources")
                                }, "root", this);

    // Auth
    drive->setClientId(sManager.getSetting(Setting::googleID));
    drive->setClientSecret(sManager.getSetting(Setting::googleSecret));
    drive->setScope("https://www.googleapis.com/auth/drive");
    drive->setLocalPort(1966);

    // TODO: Replace with secure keychain or something similar
    O0SettingsStore *settings = new O0SettingsStore("gm-companion");
    drive->setStore(settings);

    // Signals
    connect(drive, &O2Google::linkingFailed, [ = ]() {
        qDebug() << "GOOGLE: LINKING FAILED";
        emit linkStatusChanged(false);
    });

    connect(drive, &O2Google::linkingSucceeded, [ = ]() {
        qDebug() << "GOOGLE: LINKING SUCCEEDED";
        emit linkStatusChanged(true);
    });

    connect(drive, &O2Google::openBrowser, [ = ](QUrl url) {
        emit openWebsite(url);
    });

    if (sManager.getSetting(Setting::googleConnect).toInt())
    {
        drive->link();
    }
}

/**
 * @brief Connect to GoogleDrive
 */
void GoogleDrive::link()
{
    drive->setClientId(m_id);
    drive->setClientSecret(m_secret);
    drive->link();
}

/**
 * @brief Add file paths to the folder tree
 * @param paths List of paths to be added
 * @param root The root folder where the paths should be added to
 * @return List of pointers to the new folders
 */
QList<Folder *>GoogleDrive::addPathsToTree(QStringList paths, DefaultFolder root)
{
    qDebug() << "Adding" << paths.size() << "paths to file tree ...";

    QList<Folder *>  fileFolders;
    QList<FilePath> *filePaths = nullptr;

    switch (root)
    {
    case DefaultFolder::Music:
        filePaths = &m_songPaths;
        break;

    case DefaultFolder::Sounds:
        filePaths = &m_soundPaths;
        break;

    case DefaultFolder::Radio:
        filePaths = &m_filePaths;
        break;

    case DefaultFolder::Resources:
        filePaths = &m_iconPaths;
        break;

    default:
        qDebug() << "Error: Unknown folder root!";
        return {};
    }

    for (int index = 0; index < paths.size(); index++)
    {
        QString p           = paths[index];
        QString path        = p.remove(0, 1);
        QStringList folders = path.split('/');

        FilePath sp;
        sp.fullPath = paths[index];
        sp.fileName = folders[folders.size() - 1];
        folders.removeLast();

        qDebug() << sp.fileName;
        qDebug() << "   " << folders;

        // Add folders to tree
        Folder *f = folderTree->getDefaultFolder(root);

        if (f == nullptr) return {};

        // If file is in root folder
        if (folders.size() == 0)
        {
            qDebug() << "File is located in root";
            qDebug() << "Appending folder ...";
            fileFolders.append(f);
            sp.internalParentID = f->getInternalId();
            qDebug() << "Appending path ...";
            filePaths->append(sp);

            qDebug() << "Done.";
        }
        else
        {
            // If file is in a subfolder of root
            for (int i = 0; i < folders.size(); i++)
            {
                QString folder = folders[i];
                bool    exists = false;

                // Check if folder exists as subfolder of parent dir
                for (Folder *tf : f->children())
                {
                    if (tf->getName() == folder)
                    {
                        exists = true;
                        f      = tf;

                        if (i == folders.size() - 1)
                        {
                            sp.internalParentID = f->getInternalId();
                            filePaths->append(sp);

                            if (!fileFolders.contains(f))
                            {
                                fileFolders.append(f);
                            }
                        }

                        break;
                    }
                }

                // Create folder
                if (!exists)
                {
                    Folder *subFolder = new Folder(folder);
                    f->addChild(subFolder);
                    f = subFolder;
                    f->setInternalId(folderTree->getFolderCount());
                    folderTree->increaseMissingFolderIDs();
                    folderTree->increaseFolderCount();

                    if (i == folders.size() - 1)
                    {
                        fileFolders.append(f);
                        sp.internalParentID = f->getInternalId();
                        filePaths->append(sp);
                    }
                }
            }
        }
    }

    folderTree->print();

    return fileFolders;
}

/**
 * @brief Check if folders have a known Google Drive ID, if not fetch them
 * @param parent Parent folder to check in
 * @param mode Mode is passed on to fetchWebContentURLs() if all folders have
 * ids
 * @param args List of additional arguments, passed on to fetchWebContentURLs()
 */
void GoogleDrive::findMissingFolderIDs(Folder *parent, int mode, QStringList args)
{
    if (!parent) return;

    QStringList folders;

    qDebug() << "Google Drive: Missing" << folderTree->getMissingFolderIDs() << "Folder IDs ...";

    if (folderTree->getMissingFolderIDs() > 0)
    {
        if (parent->children().size() == 0)
        {
            folderTree->printMissingFolders();
        }

        for (Folder *f : parent->children())
        {
            if (f)
            {
                if (!f->hasId())
                {
                    if (!f->isIgnored()) folders.append(f->getName());
                }
                else
                {
                    findMissingFolderIDs(f, mode, args);
                }
            }
        }

        if (!folders.isEmpty()) getFolderIDs(folders, parent->getId(), mode, args);
    }
    else
    {
        emit allFolderIDsFound();

        fetchWebContentURLs(0, mode, args);
    }
}

/**
 * @brief Set the Google Drive ID of a folder
 * @param name Name of the folder
 * @param id Google ID of the folder
 * @param parentID Google ID of the parent folder
 * @param mode Mode is passed on to findMissingFolderIDs()
 * @param args Additional arguments, passed on to findMissingFolderIDs()
 */
void GoogleDrive::setFolderID(QString name, QString id, QString parentID, int mode, QStringList args)
{
    qDebug() << "Setting folder ID for" << name << "...";
    Folder *f       = folderTree->getFolder(parentID);
    bool    success = false;

    if (f != nullptr)
    {
        Folder *c = f->getChild(name);

        if (c != nullptr)
        {
            c->setId(id);
            success = true;

            qDebug() << "Found folder" << name << ", set ID:" << id;
            folderTree->decreaseMissingFolderIDs();
            findMissingFolderIDs(c, mode, args);
        }
    }

    if (!success)
    {
        qDebug() << "Error: Could not find folder" << name << "!";
    }
}

/**
 * @brief Send a request to get Google Drive IDs of folders
 * @param folderNames Names of the folders to get the IDs of
 * @param parentID Google ID of the parent folder
 * @param mode Mode is passed on
 * @param args Args are passed on
 */
void GoogleDrive::getFolderIDs(QStringList folderNames, QString parentID, int mode, QStringList args)
{
    O2Requestor *requestor = new O2Requestor(m_manager, drive, this);

    QNetworkRequest request(QUrl("https://www.googleapis.com/drive/v3/files/?q='" + parentID +
                                 "'+in+parents&fields=files(id, name)&pageSize=1000"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    qDebug() << request.url();

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
        qDebug() << "Received folder IDs";
        auto doc   = QJsonDocument::fromJson(data);
        auto root  = doc.object();
        auto files = root.value("files").toArray();

        qDebug() << "RECEIVED IDs:" << files;

        QStringList tempFolderNames = folderNames;

        for (auto a : files)
        {
            QString tempName = a.toObject().value("name").toString();
            QString tempID   = a.toObject().value("id").toString();

            if (folderNames.contains(tempName))
            {
                setFolderID(tempName, tempID, parentID, mode, args);
                tempFolderNames.removeOne(tempName);
            }
        }

        Folder *p = folderTree->getFolder(parentID);
        qDebug() << "The following folders can not be found upstream:";

        for (auto a : tempFolderNames)
        {
            qDebug() << "   " << a;
            folderTree->decreaseMissingFolderIDs(p->getChild(a)->ignore());
        }
    });

    qDebug() << "GET FOLDER IDs: Sending request for folders:" << folderNames;
    requestor->get(request);
}

/**
 * @brief Get the WebContentLink URLs of files in a folder
 * @param parentId Google ID of the parent folder
 * @return Request ID of the GET request
 */
int GoogleDrive::getFileUrls(QString parentId)
{
    qDebug() << "Google Drive: Getting file urls of files in" << parentId << "...";

    O2Requestor *requestor = new O2Requestor(m_manager, drive, this);

    QUrl url("https://www.googleapis.com/drive/v3/files/?q='" + parentId + "'+in+parents&fields=files(name, webContentLink, mimeType, id)");
    QNetworkRequest request(url);
    qDebug() << url;

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int reqId, QNetworkReply::NetworkError error, QByteArray data) {
        qDebug() << "Google Drive: Received files! (getFileUrls(QString))";
        qDebug() << reqId << error;

        const auto folders = QJsonDocument::fromJson(data).object().value("files").toArray();
        QList<GoogleFile>googleFiles;

        for (auto f : folders)
        {
            if (f.toObject().value("mimeType") != "application/vnd.google-apps.folder")
            {
                qDebug() << f.toObject().value("name").toString() << f.toObject().value("webContentLink").toString();
                GoogleFile g;
                g.fileName = f.toObject().value("name").toString();
                g.id       = f.toObject().value("id").toString();
                g.weblink  = f.toObject().value("webContentLink").toString();
                g.parent   = parentId;
                googleFiles.append(g);
            }
        }

        emit receivedFileUrls(reqId, googleFiles);
    });

    return requestor->get(request);
}

/**
 * @brief Get the WebContentLinks of files
 * @param paths List of filepaths to get links of
 * @param mode 0: Music, 1: Sounds, 2: Radio, 3: Resources
 * @param args Additional arguments, used for sounds
 */
void GoogleDrive::getFileUrls(QStringList paths, int mode, QStringList args)
{
    switch (mode)
    {
    case 0: // Music
        m_songPaths.clear();
        m_songGetUrls.clear();

        // Add file paths to tree
        m_songFolders = addPathsToTree(paths, DefaultFolder::Music);
        break;

    case 1: // Sounds
        m_soundPaths.clear();

        // Add file paths to tree
        m_soundFolders = addPathsToTree(paths, DefaultFolder::Sounds);
        break;

    case 2: // Radio
        m_filePaths.clear();
        m_fileFolders = addPathsToTree(paths, DefaultFolder::Radio);
        break;

    case 3: // Resources
        m_iconPaths.clear();
        m_iconFolders = addPathsToTree(paths, DefaultFolder::Resources);
        break;

    default:
        break;
    }

    // Find folder ids of new folders
    qDebug() << "Finding missing folder IDs ...";
    findMissingFolderIDs(folderTree->root(), mode, args);

    qDebug() << "AMOUNT OF FILES REQUIRED:" << paths.size();

    // Get URLs of files after all folder IDs are found
}

/**
 * @brief Get a list of files in a folder
 * @param id Folder id
 * @return Request id. Can be used to identify the request later.
 */
int GoogleDrive::getFolderContent(QString id)
{
    qDebug() << "Google Drive: Getting folder content of" << id << "...";
    O2Requestor *requestor = new O2Requestor(m_manager, drive, this);

    QNetworkRequest request(QUrl("https://www.googleapis.com/drive/v3/files/?q='" + id +
                                 "'+in+parents&fields=files(id, name, webContentLink, trashed, parents)&pageSize=1000&trashed=false"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int reqId, QNetworkReply::NetworkError error, QByteArray data) {
        qDebug() << "Google Drive: Received files!";
        auto doc   = QJsonDocument::fromJson(data);
        auto root  = doc.object();
        auto files = root.value("files").toArray();

        QList<GoogleFile>list;
        QStringList fileNames;

        for (auto a : files)
        {
            GoogleFile f;
            f.fileName = a.toObject().value("name").toString();
            f.id       = a.toObject().value("id").toString();
            f.parent   = id;
            f.weblink  = a.toObject().value("webContentLink").toString();

            if (!fileNames.contains(f.fileName) && !a.toObject().value("trashed").toBool())
            {
                list.append(f);
                fileNames.append(f.fileName);
                qDebug() << f.weblink;
                qDebug() << a.toObject().value("parents").toArray();
            }
        }

        emit receivedFolderContent(m_requestCount, list);
    });

    requestor->get(request);
    m_requestCount++;

    return m_requestCount;
}

/**
 * @brief Download a file from Google Drive
 * @param id File ID
 * @return Request ID
 */
int GoogleDrive::getFile(QString id)
{
    qDebug() << "Google Drive: Getting file" << id << "...";

    O2Requestor *requestor = new O2Requestor(m_manager, drive, this);

    QUrl url("https://www.googleapis.com/drive/v3/files/" + id);
    QUrlQuery query;
    query.addQueryItem("alt", "media");
    url.setQuery(query.query());
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int reqId, QNetworkReply::NetworkError error, QByteArray data) {
        qDebug() << "Google Drive: Received file!";
        emit receivedFile(m_requestCount, data);
    });

    requestor->get(request);
    m_requestCount++;

    return m_requestCount;
}

/**
 * @brief Get a list of ids of subfolders
 * @param parentId GoogleDrive ID of the parent folder
 * @return Request ID
 */
int GoogleDrive::getSubfolderIDs(QString parentId)
{
    qDebug() << "Google Drive: Getting subfolders of" << parentId << "...";

    O2Requestor *requestor = new O2Requestor(m_manager, drive, this);

    QUrl url("https://www.googleapis.com/drive/v3/files/?q='" + parentId + "'+in+parents&mimeType='application/vnd.google-apps.folder'&fields=files(name, webContentLink, mimeType, id)");
    QNetworkRequest request(url);
    qDebug() << url;

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int reqId, QNetworkReply::NetworkError error, QByteArray data) {
        qDebug() << "Google Drive: Received Subfolder IDs!";

        qDebug() << reqId << error;

        const auto folders = QJsonDocument::fromJson(data).object().value("files").toArray();
        QList<GoogleFile>googleFiles;

        for (auto f : folders)
        {
            if (f.toObject().value("mimeType") == "application/vnd.google-apps.folder")
            {
                qDebug() << f.toObject().value("name").toString() << f.toObject().value("webContentLink").toString();
                GoogleFile g;
                g.fileName = f.toObject().value("name").toString();
                g.id       = f.toObject().value("id").toString();
                g.weblink  = f.toObject().value("webContentLink").toString();
                g.parent   = parentId;
                googleFiles.append(g);
            }
        }

        emit receivedSubfolderIDs(reqId, googleFiles);
    });

    return requestor->get(request);
}

/**
 * @brief Get WebContentURLs of files
 * @param index Starting index in file list
 * @param mode 0: Music, 1: Sounds, 2: Radio, 3: Resources
 * @param args Additional arguments, used for sounds
 */
void GoogleDrive::fetchWebContentURLs(int index, int mode, QStringList args)
{
    QString query;
    QList<FilePath> *filePaths = nullptr;
    QList<QUrl>     *fileURLs  = nullptr;

    switch (mode)
    {
    case 0: // Music
        qDebug() << "Fetching songs!";
        filePaths = &m_songPaths;
        fileURLs  = &m_songGetUrls;
        break;

    case 1: // Sounds
        qDebug() << "Fetching sounds!";
        filePaths = &m_soundPaths;
        fileURLs  = new QList<QUrl>;
        qDebug() << "AMOUNT OF SOUNDS:" << m_soundPaths.size();
        break;

    case 2: // Radio
        qDebug() << "Fetching radio!";
        filePaths = &m_filePaths;
        fileURLs  = new QList<QUrl>;
        break;

    case 3: // Resources
        qDebug() << "Fetching icons!";
        filePaths = &m_iconPaths;
        fileURLs  = new QList<QUrl>;
        break;

    default:
        break;
    }

    // Make a query for MAX_FILE_QUERY_COUNT amount of files
    // The file count cap is required because Google does not accept queries
    // that are too long, but they don't publish the maximum size, the defined
    // value seems to be a good number
    qDebug() << "Index:" << index << "New Index:" << index + MAX_FILE_QUERY_COUNT;

    for (int i = index; i < filePaths->size() && i < index + MAX_FILE_QUERY_COUNT; i++)
    {
        FilePath sp       = filePaths->at(i);
        QString  fileName = sp.fileName;
        fileName = fileName.replace('\'', "\\'").replace('&', "%26").replace('?', "%3F");

        query.append("name+=+'" + fileName + "'+or+");
    }

    query = query.remove(query.lastIndexOf("+or+"), 4);

    qDebug() << "Query Size:" << query.size();

    O2Requestor *requestor = new O2Requestor(m_manager, drive, this);

    QUrl url("https://www.googleapis.com/drive/v3/files/?q=" + query + "&fields=files(name, webContentLink, parents, mimeType)");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    qDebug() << "Sending the following request:" << request.url() << request.rawHeaderList() << "\n";

    if (!url.isValid())
    {
        qDebug() << "   URL is not valid:" << url.errorString();
    }

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int id, QNetworkReply::NetworkError error, QByteArray data)
    {
        if (error > 0)
        {
            qDebug() << "GOOGLE DRIVE ERROR:" << error;
            qDebug() << data;
        }
        else
        {
            auto doc   = QJsonDocument::fromJson(data);
            auto root  = doc.object();
            auto files = root.value("files").toArray();

            qDebug() << "Google Drive: Received files." << files.size();

            // Get all files in folders
            QList<FilePath>cloudFilePaths;

            for (auto a : files)
            {
                if (a.toObject().value("mimeType").toString() != "application/vnd.google-apps.folder")
                {
                    FilePath fp;
                    fp.fileName      = a.toObject().value("name").toString();
                    fp.webConentLink = a.toObject().value("webContentLink").toString();
                    fp.parentID      = a.toObject().value("parents").toArray()[0].toString();
                    cloudFilePaths.append(fp);
                }
            }

            // Get webContentLinks from cloudSongPaths
            for (int i = index; i < filePaths->size() && i < index + MAX_FILE_QUERY_COUNT; i++)
            {
                FilePath sp = filePaths->at(i);
                Folder *f   = folderTree->getFolder(sp.internalParentID);
                Folder *tf;

                bool found = false;

                for (FilePath cfp : cloudFilePaths)
                {
                    tf = folderTree->getFolder(cfp.parentID);

                    if ((sp.fileName == cfp.fileName) && (tf != nullptr) && (tf->getInternalId() == f->getInternalId()))
                    {
                        fileURLs->append(QUrl(cfp.webConentLink));
                        sp.webConentLink = cfp.webConentLink;
                        filePaths->replace(i, sp);
                        found = true;
                        break;
                    }
                }

                if (!found) qDebug() << "FILE NOT FOUND:" << sp.fileName;
            }

            qDebug() << "Done finding files!";
            qDebug() << "USEFUL FILES RECEIVED:" << fileURLs->size();
            emit receivedFileUrls(*fileURLs, mode, args);
            emit receivedFileUrls(filePaths, mode);

            if (index + MAX_FILE_QUERY_COUNT < filePaths->size())
            {
                qDebug() << "File list is still incomplete, sending another request ...";
                fetchWebContentURLs(index + MAX_FILE_QUERY_COUNT, mode, args);
            }
        }
    });

    requestor->get(request);
}
