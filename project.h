#ifndef PROJECT_H
#define PROJECT_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QImage>
#include <string>
#include <stack>
#include <list>
#include "slideview.h"
//#include "gif.h"

class Project : public QObject
{
    std::string fileName;
    std::list<SlideView*> framesList;
    std::vector<QImage> imageList;

    int width;
    int height;

public:
    Project();
    Project(std::string name, SlideView* frame, QObject *parent);
    SlideView* getSlide(int index);
    void addSlideAt(int index, SlideView* frame);
    void addSlide(SlideView* frame);
    int getSizeList();
    int getFramesPerSec();
    void deleteAllSlidesAndRefresh();

    QImage getImage(int index);
    void addImage(QImage image);

    int framesPerSec;
public slots:
    void addFrameSlot(SlideView* frame);
    void addImageToButton();
//    void exportGifSlot(std::string name);
    void framesPerSecSlot(int value);
//  void callGifNamePopup();
};

#endif // PROJECT_H
