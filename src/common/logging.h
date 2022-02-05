#ifndef LOGGING_H
#define LOGGING_H

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(gmMain)

Q_DECLARE_LOGGING_CATEGORY(gmAudioTool)
Q_DECLARE_LOGGING_CATEGORY(gmAudioEditor)
Q_DECLARE_LOGGING_CATEGORY(gmAudioEditorFileBrowser)
Q_DECLARE_LOGGING_CATEGORY(gmAudioExporter)
Q_DECLARE_LOGGING_CATEGORY(gmAudioMusic)
Q_DECLARE_LOGGING_CATEGORY(gmAudioRadio)
Q_DECLARE_LOGGING_CATEGORY(gmAudioSounds)
Q_DECLARE_LOGGING_CATEGORY(gmAudioSaveLoad)
Q_DECLARE_LOGGING_CATEGORY(gmAudioMetaData)
Q_DECLARE_LOGGING_CATEGORY(gmAudioDiscord)
Q_DECLARE_LOGGING_CATEGORY(gmAudioElementModelManager)

Q_DECLARE_LOGGING_CATEGORY(gmMapsTool);
Q_DECLARE_LOGGING_CATEGORY(gmMapsMarker);
Q_DECLARE_LOGGING_CATEGORY(gmMapsMap);

Q_DECLARE_LOGGING_CATEGORY(gmDiceTool);

Q_DECLARE_LOGGING_CATEGORY(gmCombatTracker);
Q_DECLARE_LOGGING_CATEGORY(gmCombatEffect);

Q_DECLARE_LOGGING_CATEGORY(gmCharactersTool);
Q_DECLARE_LOGGING_CATEGORY(gmCharactersCharacter);
Q_DECLARE_LOGGING_CATEGORY(gmCharactersImageViewer);

Q_DECLARE_LOGGING_CATEGORY(gmShopsTool);
Q_DECLARE_LOGGING_CATEGORY(gmShopsItemShop);
Q_DECLARE_LOGGING_CATEGORY(gmShopsItemEditor);
Q_DECLARE_LOGGING_CATEGORY(gmShopsShopEditor);
Q_DECLARE_LOGGING_CATEGORY(gmShopsShopProject);

Q_DECLARE_LOGGING_CATEGORY(gmNotesTool);
Q_DECLARE_LOGGING_CATEGORY(gmNotesSaveLoad);

Q_DECLARE_LOGGING_CATEGORY(gmRESTService)
Q_DECLARE_LOGGING_CATEGORY(gmRESTServiceLocal)

Q_DECLARE_LOGGING_CATEGORY(gmGoogleDrive)
Q_DECLARE_LOGGING_CATEGORY(gmGoogleDriveLocal)
Q_DECLARE_LOGGING_CATEGORY(gmGoogleDriveServer)

Q_DECLARE_LOGGING_CATEGORY(gmNextCloud)

Q_DECLARE_LOGGING_CATEGORY(gmDiscord)

Q_DECLARE_LOGGING_CATEGORY(gmFileManager)
Q_DECLARE_LOGGING_CATEGORY(gmFileDialog)
Q_DECLARE_LOGGING_CATEGORY(gmFileAccessLocal)
Q_DECLARE_LOGGING_CATEGORY(gmFileAccessGoogle)
Q_DECLARE_LOGGING_CATEGORY(gmFileAccessNextCloud)

#endif // LOGGING_H
