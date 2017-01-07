#include "project.h"
//#include "gif.h"

Project::Project()
{

}

Project::Project(std::string name, SlideView* frame, QObject *parent) : QObject(parent)
{
    framesList.push_back(frame);
    framesPerSec = 10;
}

SlideView* Project::getSlide(int index)
{
    unsigned n = index;
    std::list<SlideView*>::iterator it;

    if(framesList.size() > n)
    {
        it = framesList.begin();
        std::advance(it, n);
    }

    return *it;
}

QImage Project::getImage(int index)
{
   return imageList.at(index);
}

void Project::addSlideAt(int index, SlideView* frame)
{
    unsigned n = index;
    std::list<SlideView*>::iterator it;
    if(framesList.size() > n)
    {
        it = framesList.begin();
        std::advance(it, n);
    }

    framesList.insert(it, frame);

}

void Project::addSlide(SlideView* frame)
{
    framesList.push_back(frame);

    qDebug()<< framesList.size();
}

void Project::addImage(QImage image)
{
    imageList.push_back(image);
}

void Project::addFrameSlot(SlideView* frame)
{
    Project::addSlide(frame);
}

int Project::getSizeList()
{
    return framesList.size();
}

//<<<<<<< HEAD
//=======
//void Project::exportGifSlot(std::string name)
//{
////    std::cout<<name<<std::endl;
//    name += ".gif";
//    const char* cname = name.c_str();
//    SlideView* slide = getSlide(0);
//    int width = slide->getImage().width();
//    int height = slide->getImage().height();
//    // updated to use slots with frame slider
//    //int framesPerSec = 10; // Set this variable when we change the frame playback speed
//    int delay = 100/framesPerSec; // This is the delay in 1/100th of a second. 5 corresponds to 25 frames per second
//    GifWriter gifWrt;
//    GifBegin(&gifWrt, cname, width, height, delay, 8, false);

//    for(auto itr = framesList.begin(); itr != framesList.end(); ++itr)
//    {
//        SlideView* tempSlide = *itr;
//        QImage tempImg = tempSlide->getImage().convertToFormat(QImage::Format_RGB32);
//        width = tempImg.width();
//        height = tempImg.height();
//        GifWriteFrame(&gifWrt, tempImg.bits(), width, height, delay, 8, false);
////        std::cout<<"Writing frame"<<std::endl;
//    }
//    GifEnd(&gifWrt);
//}
//>>>>>>> 0ac5932f0dacea4dbc16c22f15741760a2d0df2e

void Project::deleteAllSlidesAndRefresh()
{
    framesList.clear();
    imageList.clear();
}

void Project::framesPerSecSlot(int value)
{
    framesPerSec = value;
}

int Project::getFramesPerSec()
{
    return framesPerSec;
}
