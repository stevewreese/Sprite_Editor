#include "previewwindow.h"
#include "ui_previewwindow.h"

PreviewWindow::PreviewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreviewWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Preview Animation");
    connect(&timer, &QTimer::timeout, this, &PreviewWindow::timerSlot);
}

PreviewWindow::~PreviewWindow()
{
    delete ui;
}

/*
 * Receives a vector of QImages, speed of .GIF, and start timer for .GIF preview
 */
void PreviewWindow::previewSlot(int value, std::vector<QImage> _imageList)
{
    fps = value;
    int delay =  (1000 / fps);
    imageList = _imageList;
    counter = imageList.size();
    timer.start(delay);
}

/*
 * Creates a new QImage each time for each frame of the .GIF preview
 * This slot is connected to timer.timeout()
 */
void PreviewWindow::timerSlot()
{
    theScene = new QGraphicsScene(ui->previewView);
    QImage theImage = imageList.at(counter % imageList.size());
    pixImage = QPixmap::fromImage(theImage);
    theScene->addPixmap(pixImage);
    theScene->setSceneRect(pixImage.rect());
    ui->previewView->setScene(theScene);
    counter++;
}
