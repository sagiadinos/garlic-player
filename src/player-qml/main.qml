import QtQuick 2.7
import QtQuick.Window 2.2
import QtMultimedia 5.7
import  QtWebView 1.1

import "main_window.js" as MainWindows;
import "region.js" as MyRegion

Window
{
    id: main_window
    visible: true
    width: 980
    height: 540
    title: qsTr("Garlic-Player")

    function deleteRegionsAndLayouts()
    {
        MainWindows.deleteRegionsAndLayouts();
    }
    function setRegions(regions_list)
    {
        MainWindows.setRegions(regions_list);
        return;
    }
    function startShowMedia(media)
    {
        MainWindows.startShowMedia(media);
    }
    function stopShowMedia(media)
    {
        MainWindows.startShowMedia(media);
    }

     MouseArea {
         anchors.fill: parent

     }

}
