/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :  QWidget(parent)
{
    layout = new QHBoxLayout();
    setLayout(layout);
    MyIndexFile = new TIndexFile();
    connect(MyIndexFile, SIGNAL(isLoaded()), SLOT(setSmilIndex()));
    MyConfiguration = new TConfiguration();
}

void MainWindow::setInitialSmilIndex(QString path)
{
    smil_index_path = path;

    // only for developing
    if (smil_index_path == "")
        smil_index_path = "http://smil-admin.com/resources/smil/test/index.smil";
//        smil_index_path = "http://smil-admin.com/resources/smil/test/full_smil/simple_body.smil";

    if (smil_index_path == "")
        smil_index_path = MyConfiguration->getIndexServer();

    MyIndexFile->load(smil_index_path, MyConfiguration);
}

void MainWindow::setSmilIndex()
{
    MySmil = new TSmil(MyConfiguration);
    MySmil->init(smil_index_path);
    setRegions(MyIndexFile->getHead());
    connect(MySmil, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MySmil, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    setGeometry(0,0, width(), height());
    MySmil->beginSmilParsing(MyIndexFile->getBody());
    return;
}

void MainWindow::setRegions(QDomElement head)
{
    THead MyHead;
    MyHead.parse(head);
    setStyleSheet("background-color:"+MyHead.getRootBackgroundColor()+";");
    QList<Region>  region_list = MyHead.getLayout();
    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list.length(); i++)
    {
        j = ar_regions.insert(region_list.at(i).regionName, new TRegion(this));
        layout->addWidget(ar_regions[j.key()]);
        ar_regions[j.key()]->setRegion(region_list.at(i));
        ar_regions[j.key()]->setRootSize(width(), height());
    }
}

QString MainWindow::selectRegion(QString region_name)
{
    QMap<QString, TRegion *>::iterator i;
    if (!ar_regions.contains(region_name))
        i = ar_regions.begin();
    else
       i = ar_regions.find(region_name);
    return i.key();
}

void MainWindow::startShowMedia(TMedia *media)
{
    QString type   = media->objectName();

    if (type == "TImage")
        playImage(qobject_cast<TImage *>(media));
    else if (type == "TVideo")
        playVideo(qobject_cast<TVideo *>(media));
    else if (type == "TAudio")
        playAudio(qobject_cast<TAudio *>(media));
    else if (type == "TWeb")
        playWeb(qobject_cast<TWeb *>(media));
}

void MainWindow::stopShowMedia(TMedia *media)
{
    QString type        = media->objectName();
    if (type == "TImage")
        removeImage(qobject_cast<TImage *>(media));
    else if (type == "TVideo")
        removeVideo(qobject_cast<TVideo *>(media));
    else if (type == "TWeb")
        removeWeb(qobject_cast<TWeb *>(media));
    return;
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event);
    QMap<QString, TRegion *>::iterator i;
    for (i = ar_regions.begin(); i != ar_regions.end(); ++i)
        ar_regions[i.key()]->setRootSize(width(), height());
}

void MainWindow::playImage(TImage *MyImage)
{
    QString region_name = selectRegion(MyImage->getRegion());
    ar_regions[region_name]->playImage(MyImage->getMediaForShow());
    ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::playVideo(TVideo *MyVideo)
{
    QString region_name = selectRegion(MyVideo->getRegion());
    ar_regions[region_name]->playVideo(MyVideo->getMediaForShow());
    ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::playAudio(TAudio *MyAudio)
{
    QString region_name = selectRegion(MyAudio->getRegion());
    ar_regions[region_name]->playAudio(MyAudio->getMediaForShow());
    //ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::playWeb(TWeb *MyWeb)
{
    QString region_name = selectRegion(MyWeb->getRegion());
    ar_regions[region_name]->playWeb(MyWeb->getMediaForShow());
    ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::removeImage(TImage *MyImage)
{
    QString region_name = selectRegion(MyImage->getRegion());
    ar_regions[region_name]->removeImage(MyImage->getMediaForShow());
}

void MainWindow::removeVideo(TVideo *MyVideo)
{
    QString region_name = selectRegion(MyVideo->getRegion());
    ar_regions[region_name]->removeVideo(MyVideo->getMediaForShow());
}

void MainWindow::removeWeb(TWeb *MyWeb)
{
    QString region_name = selectRegion(MyWeb->getRegion());
    ar_regions[region_name]->removeWeb(MyWeb->getMediaForShow());
}

MainWindow::~MainWindow()
{
    ar_regions.clear();
    delete MySmil;
    delete layout;
}

