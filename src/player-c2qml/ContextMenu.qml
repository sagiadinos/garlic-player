import QtQuick 2.7
import QtQuick.Controls 2.2


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
  //      action: network_options
    }
    MenuItem
    {
        text: "Resource monitor..."
        onTriggered:
        {
            var component = Qt.createComponent("resource_monitor.qml");
            var window = component.createObject(root, {"x": 100, "y": 300});
            window.show();
        }
    }
    MenuSeparator {}
    MenuItem
    {
        text: "Quit"
        onTriggered: Qt.quit()
    }
    Shortcut
    {
        sequence: "Ctrl+Q"
        onActivated: Qt.quit()
    }
    Shortcut
    {
        sequence: "Ctrl+D"
        onActivated:
        {
            var component = Qt.createComponent("resource_monitor.qml");
            var window = component.createObject(root, {"x": 100, "y": 300});
            window.show();
        }
    }
}
