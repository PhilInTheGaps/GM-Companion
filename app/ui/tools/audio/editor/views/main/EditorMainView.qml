import QtQuick 2.2

Loader {
    id: loader

    property bool valid: item !== null

    source: "EditorMainViewDefault.qml"

    onStatusChanged: {
        if (loader.status == Loader.Error) {
            console.warn("Warning: Could not load audio editor main view, using fallback version ...")
            loader.source = "EditorMainViewFallback.qml"
            loader.active = true
        }
    }
}
