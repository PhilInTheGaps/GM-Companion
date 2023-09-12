pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import "./shop/item_editor"
import "./shop/shop_editor"
import "./shop/shop_page"

Item {
    id: root

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: shop_page_component
    }

    Component {
        id: shop_page_component

        ShopPage {
            id: shop_page

            onOpenEditor: {
                stack.push(shop_editor_component)
            }
        }
    }

    Component {
        id: shop_editor_component

        ShopEditor {
            id: shop_editor_page

            onBackToViewer: {
                stack.pop()
            }

            onSwitchToItemEditor: {
                stack.push(item_editor_component)
            }
        }
    }

    Component {
        id: item_editor_component

        ItemEditor {
            id: item_editor_page

            onBackToShopEditor: {
                stack.pop()
            }
        }
    }
}
