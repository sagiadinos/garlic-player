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

}

function stopShowMedia(media)
{

}

function determineQmlFillMode(fill_mode)
{
    if (fill_mode == "fill")
       return "Stretch";
    else if (fill_mode == "meet")
        return "PreserveAspectCrop";
    else if (fill_mode == "meetbest")
        return "PreserveAspectFit";
    else
        return "Pad";
}
