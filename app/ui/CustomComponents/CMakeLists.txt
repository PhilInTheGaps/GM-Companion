cmake_minimum_required(VERSION 3.19)

qt_add_library(gm-companion-ui-cc STATIC)

set_source_files_properties(CCColors.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(CCSizes.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(gm-companion-ui-cc
    URI CustomComponents
    VERSION 1.0
    QML_FILES
        CustomButton.qml
        CustomComboBox.qml
        CustomColorButton.qml
        CustomMenuItem.qml
        CustomRadioButton.qml
        CustomScrollLabel.qml
        CustomTextEdit.qml
        CustomToolBar.qml
        CustomToolBarButton.qml
        CustomToolBarComboBox.qml
        CCColors.qml
        CCSizes.qml
    IMPORT_PATH
        ${QML_IMPORT_PATH}
)
