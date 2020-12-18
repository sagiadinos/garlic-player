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
}

// ====================  private methods =================================
