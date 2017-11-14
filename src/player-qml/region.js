
var qml_region_name;
var MyMedia;

var MyRegionComponent;
var MyImageComponent;
var MyVideoComponent;
var MyWebComponent;

function createRegion(region)
{
    qml_region_name   = region[0];
    MyRegionComponent =  Qt.createQmlObject('import QtQuick 2.7; Rectangle {color: "'+region[6]+'"; '+
                                   'id:' +qml_region_name+ ';' +
                                   'x:     main_window.width*'+region[2]+';'+
                                   'y: main_window.height*'+region[1]+';'+
                                   'width: main_window.width*'+region[3]+';'+
                                   'height: main_window.height*'+region[4]+';'+'}',
                                   main_window,
                                   qml_region_name);

    MyImageComponent = Qt.createQmlObject('import QtQuick 2.7; Image {id:image;}', MyRegionComponent, "");
    MyVideoComponent = Qt.createQmlObject('import QtQuick 2.7; import QtMultimedia 5.7; Video{id: video;}', MyRegionComponent, "");
    MyWebComponent   = Qt.createQmlObject('import QtQuick 2.7; import QtWebView 1.1; WebView{id: web;}', MyRegionComponent, "");
}

function destroyRegion()
{
    MyImageComponent.destroy();
    MyVideoComponent.destroy();
    MyWebComponent.destroy();
    MyRegionComponent.destroy();
}

function startShowMedia(media)
{
    MyMedia = media;
    var fill_mode      = determineQmlFillMode(media.fill_mode);
    var anchors_fill   = null;
    if (fill_mode != "Pad")
        anchors_fill = MyRegionComponent;

    if (media.type == "TImage")
    {
        MyImageComponent.anchors.fill = anchors_fill;
        MyImageComponent.fillMode     = 0; // fill_mode had to be a int here;
        MyImageComponent.source       = "file://"+media.loadable_path;
    }
    else if (media.type == "TVideo")
    {
        MyVideoComponent.anchors.fill = anchors_fill;
        MyVideoComponent.source       = "file://"+media.loadable_path;
        MyVideoComponent.fillMode     = 0; // fill_mode had to be a int here;
        MyVideoComponent.play();
        MyVideoComponent.onStopped.connect(this.stoppedVideo);
    }
    else if (media.type == "TWeb")
    {
        MyWebComponent.anchors.fill = MyRegionComponent;
        MyWebComponent.url  = media.loadable_path;
    }
}

function stoppedVideo()
{
    backend.finishedMedia(MyMedia.loadable_path)
}

function stopShowMedia(media)
{

}

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
             video_id.fillMode = VideoOut.Stretch
             break
        case "meet":
             video_id.anchors.fill = video_id.parent;
             video_id.fillMode = VideoOut.PreserveAspectCrop;
             break
        case "meetbest":
             video_id.anchors.fill = video_id.parent;
             video_id.fillMode = VideoOut.PreserveAspectFit;
             break
         default:
             video_id.anchors.fill = video_id.parent;
             video_id.fillMode = VideoOut.Stretch
             break
    }
}
