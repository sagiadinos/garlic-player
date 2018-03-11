import QtQuick 2.7
import QtMultimedia 5.7
import QtWebEngine 1.4
import QtQuick 2.8
import QtQuick.Controls 2.2

Item
{
    id: root
    width: 800
    height: 600

  //  ContextMenu {id: context_menu}
    menuBar: MenuBar {
           Menu {
               title: "&File"
               MenuItem {
                   text: "E&xit"
                   shortcut: StandardKey.Quit
                   onTriggered: Qt.quit()
               }
           }
           Menu {
               title: "&Edit"
               visible: tabView.currentIndex == 2
               MenuItem { action: cutAction }
               MenuItem { action: copyAction }
               MenuItem { action: pasteAction }
           }
           Menu {
               title: "&Help"
               MenuItem {
                   text: "About..."
                   onTriggered: aboutDialog.open()
               }
           }
    MouseArea
    {
        anchors.fill: parent
        onPressAndHold: context_menu.popup()
    }
    Menu
    {
        id: context_menu
        MenuItem
        {
            text: "General options.."
        }
        MenuItem
        {
            text: "Network options..."
            action: network_options
        }
        MenuItem
        {
            text: "Resource monitor..."
            action: resource_monitor
        }
        MenuSeparator {}
        MenuItem
        {
            text: "Quit"

            action: ac_quit
        }
     }
    Action
    {
        id: network_options
        shortcut: "Ctrl+N"
        onTriggered:
        {
            var component = Qt.createComponent("network_dialog.qml");
            var window = component.createObject(root, {"x": 100, "y": 300});
            window.show();
        }
    }
    Action
    {
        id: resource_monitor
        shortcut: "Ctrl+D"
        onTriggered:
        {
            var component = Qt.createComponent("resource_monitor.qml");
            var window = component.createObject(root, {"x": 100, "y": 300});
            window.show();
        }
    }
    Action
    {
        id: ac_quit
        shortcut: "Ctrl+Q"
        onTriggered: Qt.quit()
    }
}
