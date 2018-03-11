/*
import QtQuick 2.8
import QtQuick.Controls 2.3

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
*/
/*
    works in Linux or Windows from Image/Video with parent.parent.fitImage / parent.parent.fitVideo
    crappy qt/android cannot use this functions like in
    but let there here maybe there comes a miracle ;)
    function fitImage(image_id, fit_value)
    {
        switch (fit_value)
        {
            case "fill":
                 image_id.anchors.fill = the_image.parent
                 image_id.fillMode = Image.Stretch
                 break
            case "meet":
                 image_id.anchors.fill = the_image.parent;
                 image_id.fillMode = Image.PreserveAspectCrop;
                 break
            case "meetbest":
                 image_id.anchors.fill = the_image.parent;
                 image_id.fillMode = Image.PreserveAspectFit;
                 break
             default:
                 image_id.anchors.fill = null;
                 image_id.fillMode = Image.Pad;
                 break
        }
    }

    function fitVideo(video_id, fit_value)
    {
        switch (fit_value)
        {
            case "fill":
                 video_id.anchors.fill = video_id.parent;
                 video_id.fillMode = VideoOutput.Stretch;
                 break
            case "meet":
                 video_id.anchors.fill = video_id.parent;
                 video_id.fillMode = VideoOutput.PreserveAspectCrop;
                 break
            case "meetbest":
                 video_id.anchors.fill = video_id.parent;
                 video_id.fillMode = VideoOutput.PreserveAspectFit;
                 break
             default:
                 video_id.anchors.fill = video_id.parent;
                 video_id.fillMode = VideoOutput.Stretch;
                 break
        }
    }
*/
