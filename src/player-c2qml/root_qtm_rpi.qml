import QtQuick 2.7
import QtMultimedia 5.7
import QtWebEngine 1.4

Item
{
    id: root
    width: 800
    height: 600

/*
    works in Linux or Windows from Image/Video with parent.parent.fitImage / parent.parent.fitVideo
    crappy qt/android cannot use this functions like in
    but let there here maybe there comes a miracle ;)
    function fitImage(image_id, fit_value)
    {
        switch (fit_value)
        {
            case "fill":
                 image_id.anchors.fill = image_id.parent
                 image_id.fillMode = Image.Stretch
                 break
            case "meet":
                 image_id.anchors.fill = image_id.parent;
                 image_id.fillMode = Image.PreserveAspectCrop;
                 break
            case "meetbest":
                 image_id.anchors.fill = image_id.parent;
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
}
