import QtQuick 2.9
import QtQuick.Controls 2.2
import "./shop/item_editor"
import "./shop/shop_editor"
import "./shop/shop_page"

Item {
    id: root

    StackView {
        id: shop_stack_view
        anchors.fill: parent
        initialItem: shop_page

        ShopPage {
            id: shop_page

            onOpenEditor: {
                if (shop_stack_view.currentItem != shop_editor_page) {
                    item_editor_page.visible = false
                    shop_page.visible = false
                    shop_editor_page.visible = true

                    shop_stack_view.pop(null)
                    shop_stack_view.push(shop_editor_page)
                }
            }
        }

        ShopEditor {
            id: shop_editor_page
            visible: false

            onBackToViewer: {
                if (shop_stack_view.currentItem != shop_page) {
                    item_editor_page.visible = false
                    shop_page.visible = true
                    shop_editor_page.visible = false

                    shop_stack_view.pop(null)
                    shop_stack_view.push(shop_page)
                }
            }

            onSwitchToItemEditor: {
                if (shop_stack_view.currentItem != item_editor_page) {
                    item_editor_page.visible = true
                    shop_page.visible = false
                    shop_editor_page.visible = false

                    shop_stack_view.pop(null)
                    shop_stack_view.push(item_editor_page)
                }
            }
        }

        ItemEditor {
            id: item_editor_page
            visible: false

            onBackToShopEditor: {
                if (shop_stack_view.currentItem != shop_editor_page) {
                    item_editor_page.visible = false
                    shop_page.visible = false
                    shop_editor_page.visible = true

                    shop_stack_view.pop(null)
                    shop_stack_view.push(shop_editor_page)
                }
            }
        }
    }
}
