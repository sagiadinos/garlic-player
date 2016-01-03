#include "base_playlist.h"

TBasePlaylist::TBasePlaylist(QObject * parent)
{
    Q_UNUSED(parent);
}

TFoundTag TBasePlaylist::getFoundTag()
{
    return found_tag;
}

void TBasePlaylist::reactByTag()
{
    if(actual_element.tagName() == "img")
        doImage();
    else if(actual_element.tagName() == "video")
        doVideo();
    else if(actual_element.tagName() == "audio")
        doAudio();
    else if(actual_element.tagName() == "text")
        doWeb();
    else if(actual_element.tagName() == "ref")
        doRef();
    else if(actual_element.tagName() == "prefetch")
        doPrefetch();
    else if(actual_element.tagName() == "seq")
        doSeq();
    else if(actual_element.tagName() == "par")
        doPar();
    else if(actual_element.tagName() == "excl")
        doExcl();
}

/**
 * @brief TBasePlaylist::checkRepeatCountStatus returns true, when playlist can be repeated
 * @return bool
 */
bool TBasePlaylist::checkRepeatCountStatus()
{
    bool ret = false;
    if (indefinite)
        ret = true;
    else if (repeatCount > 0 && internal_count < repeatCount)
    {
        internal_count++;
        ret = true;
    }
    return ret;
}

void TBasePlaylist::setBaseAttributes()
{
    if (actual_element.hasAttribute("id"))
        id = actual_element.attribute("id");
    if (actual_element.hasAttribute("xml:id")) // In SMIL 3.0 superset old SMIL 2.0 id.
        id = actual_element.attribute("xml:id");
    if (actual_element.hasAttribute("begin"))
        begin = actual_element.attribute("begin");
    if (actual_element.hasAttribute("end"))
        end = actual_element.attribute("end");
    if (actual_element.hasAttribute("title"))
        title = actual_element.attribute("title");
    if (actual_element.hasAttribute("title"))
        title = actual_element.attribute("title");
    if (actual_element.hasAttribute("repeatCount"))
        setRepeatCount(actual_element.attribute("repeatCount"));
    return;
}

void TBasePlaylist::doRef()
{
    if (actual_element.hasAttribute("type"))
    {
        if(actual_element.attribute("type").contains("image"))
            doImage();
        else if(actual_element.attribute("type").contains("video"))
            doVideo();
        else if(actual_element.attribute("type").contains("audio"))
            doAudio();
        else if(actual_element.attribute("type").contains("text"))
            doWeb();
    }
}

void TBasePlaylist::doImage()
{
    found_tag.name         = "img";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doVideo()
{
    found_tag.name         = "video";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doAudio()
{
    found_tag.name         = "audio";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doWeb()
{
    found_tag.name         = "web";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doSeq()
{
    found_tag.name         = "seq";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doPar()
{
    found_tag.name         = "par";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doExcl()
{
    found_tag.name         = "excl";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::doPrefetch()
{
    found_tag.name         = "prefetch";
    found_tag.dom_element  = actual_element;
    return;
}

void TBasePlaylist::setRepeatCount(QString rC)
{
    if (rC == "indefinite")
        indefinite = true;
    else
       indefinite = false;
    repeatCount = rC.toInt();
    return;
}
