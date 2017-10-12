#include "media/image.h"

Image::Image(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    ParserImage  = qobject_cast<TImage *>(media);
}

Image::~Image()
{
    loaded_image.load("");
    delete ImageWidget;
}

void Image::init()
{
    QString path = ParserImage->getLoadablePath();
    loaded_image.load(path);
    ImageWidget    = new QLabel;
    ImageWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ImageWidget->setPixmap(loaded_image);
}

void Image::changeSize(int w, int h)
{
    if (ParserImage->getFit() == "fill")
       ImageWidget->setPixmap(loaded_image.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else if (ParserImage->getFit() == "meet")
        ImageWidget->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else if (ParserImage->getFit() == "meetbest")
        ImageWidget->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        ImageWidget->setPixmap(loaded_image);
}

QWidget *Image::getView()
{
    return ImageWidget;
}
