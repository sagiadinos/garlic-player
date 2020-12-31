#include "widget.h"

TWidget::TWidget(TContainer *pc, Files::MediaManager *mm, MainConfiguration *config, QObject *parent) : BaseMedia(mm, config, parent)
{
    parent_container = pc;
    setObjectName("TWidget");
    is_media = true;

}


TWidget::~TWidget()
{
}

void TWidget::prepareDurationTimerBeforePlay()
{
    if (!MyExpr.executeQuery() || getLoadablePath().isEmpty())
    {
        skipElement();
        return;
    }

    if (startDurTimer() || isEndTimerActive())
    {
        emit startedMedia(parent_container, this);
    }
    else
    {
        skipElement();
    }
}

// ====================  protected methods =================================

void TWidget::setAttributes()
{
    parseBaseMediaAttributes();
    params_as_query.clear();
    QDomNodeList childs = root_element.childNodes();
    for (int i = 0; i < childs.length(); i++)
    {
        parseWidgetCallParameters(childs.item(i).toElement());
    }
}

void TWidget::parseWidgetCallParameters(QDomElement param)
{
    if (param.tagName() != "param" || !param.hasAttribute("name") || !param.hasAttribute("value"))
        return;

    QString attribute_name = param.attribute("name");
    if (attribute_name == "cacheControl" || attribute_name == "filename" || attribute_name == "logContentId")
        return;

    params_as_query.append(attribute_name + "=" + param.attribute("value"));
}

// ====================  private methods =================================
