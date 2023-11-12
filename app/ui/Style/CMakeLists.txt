cmake_minimum_required(VERSION 3.19)

# set QML import path
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_BINARY_DIR}/../)
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

qt_add_library(gm-companion-ui-style STATIC)

set_source_files_properties(StyleColors.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(StyleSizes.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(gm-companion-ui-style
    URI Style
    VERSION 2.0
    QML_FILES
        ApplicationWindow.qml
        Button.qml
        CheckBox.qml
        CheckDelegate.qml
        CheckIndicator.qml
        ComboBox.qml
        Control.qml
        Dialog.qml
        DialogButtonBox.qml
        Frame.qml
        ItemDelegate.qml
        Label.qml
        Menu.qml
        MenuItem.qml
        MenuSeparator.qml
        Page.qml
        PageIndicator.qml
        Pane.qml
        Popup.qml
        ProgressBar.qml
        RadioButton.qml
        RadioIndicator.qml
        ScrollBar.qml
        ScrollIndicator.qml
        ScrollView.qml
        Slider.qml
        SpinBox.qml
        SplitView.qml
        StackView.qml
        TextArea.qml
        TextField.qml
        ToolBar.qml
        ToolTip.qml
        StyleColors.qml
        StyleSizes.qml
    IMPORT_PATH
        ${QML_IMPORT_PATH}
)
