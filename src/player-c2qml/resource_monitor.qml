
import QtQuick 2.9
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
/*
import com.garlic.ResourceMonitor 1.0
Dialog {
    id: resource_monitor_dialog
    standardButtons: StandardButton.Close
    visible:true
    title: "Resource monitor"
    width: 600
    height: 400

    ResourceMonitor {
        id: myresourcemonitor

    }

    Timer {
            interval: 500;
            running: true;
            repeat: true
            onTriggered: {
                myresourcemonitor.refresh();
                text_TotalMemorySystem.text = myresourcemonitor.getTotalMemorySystem();
                text_FreeMemorySystem.text  = myresourcemonitor.getFreeMemorySystem();
                text_MemoryAppUse.text      = myresourcemonitor.getMemoryAppUse();
                text_MaxMemoryAppUsed.text   = myresourcemonitor.getMaxMemoryAppUsed();
                text_ThreadsNumber.text     = myresourcemonitor.getThreadsNumber();
                text_MaxThreadsNumber.text  = myresourcemonitor.getMaxThreadsNumber();
            }
    }
    ColumnLayout {
        id: column
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 2
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.fill: parent

        Text {
            id: text_playlist_title
            width: parent
            height: 16
            text: qsTr("-")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_current_files_in_use
            width: parent
            height: 16
            text: qsTr("-")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_TotalMemorySystem
            width: parent
            text: qsTr("Total Memory System")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_FreeMemorySystem
            width: parent
            text: qsTr("Free Memory System")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_MemoryAppUse
            width: parent
            text: qsTr("App Memory used")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_MaxMemoryAppUsed
            width: parent
            text: qsTr("Max. Memory App used")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_ThreadsNumber
            width: parent
            text: qsTr("Threads Number")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }

        Text {
            id: text_MaxThreadsNumber
            width: parent
            text: qsTr("Max Threads Number")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            font.pixelSize: 12
        }
    }
}

*/
