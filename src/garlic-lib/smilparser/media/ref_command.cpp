#include "ref_command.h"

TRefCommand::TRefCommand(TContainer *pc, Files::MediaManager *mm, MainConfiguration *config, QObject *parent) : BaseMedia(mm, config, parent)
{
    parent_container = pc;
    setParentTag(pc->getRootElement().nodeName());
    setObjectName("TRefCommand");
    is_media = true;
}

TRefCommand::~TRefCommand()
{
}

void TRefCommand::prepareDurationTimerBeforePlay()
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

void TRefCommand::setAttributes()
{
    parseBaseMediaAttributes();
}
