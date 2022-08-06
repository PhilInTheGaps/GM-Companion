import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../defines.js" as Defines
import "./generators"

Page {
    id: generators_page

    contentItem: NameGenerator {
        id: name_generator_page
    }
}
