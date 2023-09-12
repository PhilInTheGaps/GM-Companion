cmake_minimum_required(VERSION 3.19)

list(APPEND QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/thirdparty/qml-icon-fonts)
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/qml)
list(APPEND QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/imports)
list(APPEND QML_IMPORT_PATH ${CMAKE_BINARY_DIR})
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

add_subdirectory(Style)
add_subdirectory(CustomComponents)
add_subdirectory(FileDialog)

qt_add_library(gm-companion-ui STATIC)

qt_add_resources(gm-companion-ui "ui-resources"
    PREFIX "/"
    FILES
        resources/splash.jpg
        resources/icons/audio.png
        resources/icons/characters.png
        resources/icons/combat.png
        resources/icons/converter.png
        resources/icons/dice.png
        resources/icons/generators.png
        resources/icons/icon.png
        resources/icons/item-shop.png
        resources/icons/maps.png
        resources/icons/notes.png
        resources/icons/settings.png
        resources/icons/dice/d4.png
        resources/icons/dice/d6.png
        resources/icons/dice/d8.png
        resources/icons/dice/d10.png
        resources/icons/dice/d12.png
        resources/icons/dice/d20.png
        resources/icons/media/music_image.png
        resources/icons/media/radio_image.png
        resources/icons/media/sound_image.png)

set_source_files_properties(Colors.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(Sizes.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(gm-companion-ui
    URI ui
    VERSION 1.0
    IMPORTS
        CustomComponents
        FileDialog
        src
    OPTIONAL_IMPORTS
        Style
    QML_FILES
        Main.qml
        Sizes.qml
        Colors.qml
        common/CustomFileDialog.qml
        common/HelpAnnotation.qml
        common/ImageViewer.qml
        common/ProjectComboBoxWithEditorButton.qml
        common/TreeView.qml
        common/TreeViewItem.qml
        main/LoadingScreen.qml
        main/MessageDialog.qml
        main/NewSettingsDialog.qml
        main/SideMenuButton.qml
        main/UpdateDialog.qml
        tools/Audio.qml
        tools/audio/AudioCategoryView.qml
        tools/audio/AudioControlBar.qml
        tools/audio/AudioElementView.qml
        tools/audio/AudioInfoView.qml
        tools/audio/AudioPage.qml
        tools/audio/MetaDataDisplay.qml
        tools/audio/VolumeItem.qml
        tools/audio/VolumeSlider.qml
        tools/audio/audio_exporter/AudioExporter.qml
        tools/audio/buttons/AudioButton.qml
        tools/audio/buttons/FileBrowserButton.qml
        tools/audio/buttons/SoundButton.qml
        tools/audio/editor/Editor.qml
        tools/audio/editor/EditorElementColumn.qml
        tools/audio/editor/EditorFileBrowser.qml
        tools/audio/editor/EditorHeader.qml
        tools/audio/editor/EditorRightColumn.qml
        tools/audio/editor/IconFinder.qml
        tools/audio/editor/IconFinderUnsplash.qml
        tools/audio/editor/dialogs/AddonElementDialog.qml
        tools/audio/editor/dialogs/EditorDeleteDialog.qml
        tools/audio/editor/dialogs/EditorNewThingDialog.qml
        tools/audio/editor/dialogs/EditorRenameDialog.qml
        tools/audio/editor/views/EditorFileListView.qml
        tools/audio/editor/views/EditorMainView.qml
        tools/audio/editor/views/ElementFileAddView.qml
        tools/audio/editor/views/element_properties/ElementIconView.qml
        tools/audio/editor/views/element_properties/ElementNameView.qml
        tools/audio/editor/views/element_properties/ElementPropertiesView.qml
        tools/audio/editor/views/element_properties/PlaybackModeView.qml
        tools/Characters.qml
        tools/characters/CharacterList.qml
        tools/characters/classic/ClassicPage.qml
        tools/characters/classic/ImageControlBar.qml
        tools/CombatTracker.qml
        tools/combat_tracker/AddCharacterDialog.qml
        tools/combat_tracker/CharacterListHeader.qml
        tools/combat_tracker/CharacterListView.qml
        tools/combat_tracker/ListSpinBox.qml
        tools/combat_tracker/ListTextField.qml
        tools/Converter.qml
        tools/converter/ConverterEditorPage.qml
        tools/converter/ConverterEditorNewThingDialog.qml
        tools/converter/ConverterMenu.qml
        tools/converter/ConverterPage.qml
        tools/converter/ConverterUnitPage.qml
        tools/Dice.qml
        tools/DiceAlt.qml
        tools/dice/DiceButton.qml
        tools/dice/DiceCombatTracker.qml
        tools/dice/DiceSettings.qml
        tools/Generators.qml
        tools/generators/NameGenerator.qml
        tools/Maps.qml
        tools/maps/MapList.qml
        tools/maps/MapListItem.qml
        tools/maps/MapMarkerIcon.qml
        tools/maps/MapMarkerDetails.qml
        tools/maps/MapMarkerEditor.qml
        tools/maps/MapMarkerLabel.qml
        tools/maps/MapMarkerLayer.qml
        tools/maps/MapMarkerList.qml
        tools/maps/MapMarkerMenu.qml
        tools/maps/MapViewer.qml
        tools/maps/MapViewerControlBar.qml
        tools/Notes.qml
        tools/notes/InsertHeaderDialog.qml
        tools/notes/NotesControlBar.qml
        tools/notes/NotesControlView.qml
        tools/notes/NotesPage.qml
        tools/notes/SavePageDialog.qml
        tools/ItemShop.qml
        tools/shop/item_editor/ItemDetails.qml
        tools/shop/item_editor/ItemEditor.qml
        tools/shop/shop_editor/ShopEditorHeader.qml
        tools/shop/shop_editor/ItemList.qml
        tools/shop/shop_editor/NewThingDialog.qml
        tools/shop/shop_editor/ShopEditor.qml
        tools/shop/shop_editor/ShopInfo.qml
        tools/shop/shop_editor/EditorShopList.qml
        tools/shop/shop_page/CategoryList.qml
        tools/shop/shop_page/PageShopList.qml
        tools/shop/shop_page/ShopPage.qml
        tools/shop/shop_page/ShopView.qml
        tools/Settings.qml
        tools/settings/AccountsPage.qml
        tools/settings/accounts/BaseAccountPage.qml
        tools/settings/accounts/GoogleDriveAccount.qml
        tools/settings/accounts/NextCloudAccount.qml
        tools/settings/accounts/SpotifyAccount.qml
        tools/settings/accounts/StatusBar.qml
        tools/settings/AddonItem.qml
        tools/settings/AddonsPage.qml
        tools/settings/CreditsItem.qml
        tools/settings/GeneralPage.qml
        tools/settings/InfoPage.qml
        tools/settings/PathsPage.qml
    IMPORT_PATH
        ${QML_IMPORT_PATH})

target_link_libraries(gm-companion-ui
    PUBLIC
    gm-companion-ui-style
    gm-companion-ui-styleplugin
    gm-companion-ui-cc
    gm-companion-ui-ccplugin
    gm-companion-ui-filedialog
    gm-companion-ui-filedialogplugin
    gm-companion-lib
    gm-companion-libplugin
    common
    commonplugin
    qml-icon-fonts
    qml-icon-fontsplugin)
