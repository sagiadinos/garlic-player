import QtQuick 2.7
import QtMultimedia 5.7
import QtWebView 1.1

Item
{
    id: root
    width: 800
    height: 600

    ContextMenu {id: context_menu}
    MouseArea
    {
        anchors.fill: parent
        onPressAndHold: context_menu.popup()
    }
}
