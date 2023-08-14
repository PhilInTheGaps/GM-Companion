cmake_minimum_required(VERSION 3.19)

# set QML import path
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

qt_add_library(gm-companion-ui-cc STATIC)

qt_add_qml_module(gm-companion-ui-cc
    URI CustomComponents
    VERSION 1.0
    QML_FILES
        CustomButton.qml
        CustomComboBox.qml
        CustomMenuItem.qml
        CustomRadioButton.qml
        CustomScrollLabel.qml
        CustomTextEdit.qml
        CustomToolBar.qml
        CustomToolBarButton.qml
        CustomToolBarComboBox.qml
        sizes.js
        colors.js
)
