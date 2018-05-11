import QtQuick 2.7
import QtMultimedia 5.7
import QtWebEngine 1.2
import QtQuick.Controls 2.2

Item
{
    id: root
    width: 800
    height: 600

    ContextMenu {id: context_menu}
    MouseArea
    {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
             if (mouse.button === Qt.RightButton)
                 context_menu.open()
         }
    }
 }
