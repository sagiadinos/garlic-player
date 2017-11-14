import QtQuick 2.7
import QtQuick.Window 2.2
import QtMultimedia 5.7
import  QtWebView 1.1
import com.sagiadinos.backend 1.0

import "main_window.js" as MainWindows;
import "region.js" as MyRegion

Window
{
    id: main_window
    visible: true
    width: 980
    height: 540
    title: qsTr("Garlic-Player")
    Backend
    {
        id: backend
    }

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
    Image
    {
        id: test_image
    }

     MouseArea {
         anchors.fill: parent
         test_image.anchors.fill: parent
     }

}
