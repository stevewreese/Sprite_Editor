/**
    Programmer:     Thuy Nguyen, Steven Reese, Lacey <last name> , Tambra Smith, Andrew Tsai
    Last updated:   November 07, 2016
    Description:    MainWindow connects any event that happens on the gui with its appropriate code.

*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <slideview.h>
#include "newprojectdialog.h"
#include "gif.h"
#include <QFileDialog>
#include <QString>


/**
 * Constructor for the MainWindow
 * Sets up the connection of signals and slots.
 * Sets up the parameters for the gui.
 * Initializes the fields for functioning of gui.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("GoldEn Sprite Editor");
    ui->colorPaletteWidget->installEventFilter(this);
    size = 30;
    emit colorPaletteChangedSlot(qRgba(0, 0, 0, 255));


    //Creates and empty graphicsview to act as the parent for the SlideView
    view = new QGraphicsView();


    theView = new SlideView(view, size);

    theProject = new Project("", theView, this);

    theView = theProject->getSlide(0);

    theView->setFill(false);

    theProject->addImage(theView->getImage());
    imageList.push_back(theView->getImage());

    //set spinboxes range
    ui->paintWidthSlide->setRange(1, theView->getImage().width()/2);
    ui->paintWidthSpin->setRange(1, theView->getImage().width()/2);

    //Adds a the extended slideview to the layout for frame_2
    ui->drawingGridLayout->addWidget(theView);

    //Adds the layout for the scroll area
    testLayout = new QHBoxLayout(ui->scrollAreaWidgetContents);
    testLayout->setContentsMargins(10,0,10,0);
    ui->scrollAreaWidgetContents->setLayout(testLayout);

    //Gets size of the project and sets current index display to match
    currentIndex = theProject->getSizeList();

    //Creates the initial frame button, links it to the change frame method, and sets of the preview info
    QPushButton* preButton = new QPushButton();
    preButton->setObjectName(QString::number(0));
    QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
    QSize imageSize((ui->scrollArea->height())-55,(ui->scrollArea->height())-55);

    buttons.push_back(preButton);

    QPixmap testMap = QPixmap::fromImage(theView->getImage());
    testMap = testMap.scaled(imageSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QIcon buttonIcon(testMap);

    preButton->setFixedSize(buttonSize);
    preButton->setIconSize(imageSize);
    preButton->setIcon(buttonIcon);
    preButton->setFlat(false);

    testLayout->addWidget(preButton);

    // connection of signal and slot, from gui signals to theView slots.
    connect(this, &MainWindow::undoSignal, theView, &SlideView::undoSlot);
    connect(this, &MainWindow::redoSignal, theView, &SlideView::redoSlot);
    connect(this, &MainWindow::rotateLeftSignal, theView, &SlideView::rotateLeftSlot);
    connect(this, &MainWindow::rotateRightSignal, theView, &SlideView::rotateRightSlot);
    connect(this, &MainWindow::flipHorizontalSignal, theView, &SlideView::flipHorizontalSlot);
    connect(this, &MainWindow::flipVerticalSignal, theView, &SlideView::flipVerticalSlot);
    connect(theView, &SlideView::updatePalettePreviewSignal, this, &MainWindow::colorPaletteChangedSlot);
    connect(theView, &SlideView::updatePreview, this, &MainWindow::updateButton);
    connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));
    connect(&gifPopupDialog, &gifPopup::gifFileNameEntered, this, &MainWindow::exportGifSlot);
    connect(&newProjDialog, &NewProjectDialog::createNewProj, this, &MainWindow::createNewSpriteProject);
    connect(this, &MainWindow::colorPickerSignal, theView, &SlideView::colorPickerSlot);
    connect(this, &MainWindow::showPreviewSignal, &previewWindow, &PreviewWindow::previewSlot);
}

/**
 * Destructor for MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * Brings back the previous state of the image.
 */
void MainWindow::on_actionUndo_triggered()
{
    emit undoSignal();
}

/**
 * Brings forward the previous state of the image.
 */
void MainWindow::on_actionRedo_triggered()
{
    emit redoSignal();
}

/**
 * Rotates the image 90 degrees to left.
 */
void MainWindow::on_RotateLeftButton_clicked()
{
    emit rotateLeftSignal();
}

/**
 * Rotates the image 90 degrees to right.
 */
void MainWindow::on_RotateRightButton_clicked()
{
    emit rotateRightSignal();
}

/**
 * Flips the image horizontally.
 */
void MainWindow::on_FlipHorzButton_clicked()
{
    emit flipHorizontalSignal();
}

/**
 * Flips the image vertically.
 */
void MainWindow::on_FlipVertButton_clicked()
{
    emit flipVerticalSignal();
}

/**
 * Fills the a bounded region on the image with a color.
 */
void MainWindow::on_FillButton_clicked()
{
    theView->setTool("paintBucket");
}

/**
 * Enables the use of the mouse to draw a line on the image.
 */
void MainWindow::on_LineButton_clicked()
{
    std::string line = "line";
    theView->setTool(line);
}

/**
 * Enables the use of the mouse to draw a circle on the image.
 */
void MainWindow::on_CircleButton_clicked()
{
    theView->setTool("circle");
}

/**
 * Enables the use of the mouse to draw a rectangle on the image.
 */
void MainWindow::on_SquareButton_clicked()
{
    theView->setTool("rect");

}


/**
 * Enables the use of the mouse to draw a circle on the image.
 */
void MainWindow::on_PaintBrushButton_clicked()
{
    theView->setTool("paintBrush");
}

/**
 * Enables the use of mouse to draw pixels of width 1 on image.
 */
void MainWindow::on_PenButton_clicked()
{
    theView->setTool("pen");
}

/**
 * Enables the selection of a color on the image by clicking on that color.
 */
void MainWindow::on_EyeDropperButton_clicked()
{
    theView->setTool("eyedropper");
}

/**
 * Enables the disappearance of pixel colors by dragging mouse over those colors.
 */
void MainWindow::on_EraseButton_clicked()
{
    theView->setTool("erase");
}

/**
 * Enables an ellipse or rectangle to be filled.
 */
void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 == 0){
        theView->setFill(false);
    }
    if(arg1 == 2){
        theView->setFill(true);
    }

}

/**
 * Increases pixel width for drawing tools via selection box.
 */
void MainWindow::on_paintWidthSpin_valueChanged(int arg1)
{
    ui->paintWidthSlide->setValue(arg1);
    theView->setPaintWidth(arg1);
    theView->setShapeWidth(arg1);
}

/**
 * Increases pixel width for drawing tools via slider.
 */
void MainWindow::on_paintWidthSlide_sliderMoved(int position)
{
    ui->paintWidthSpin->setValue(position);
    theView->setPaintWidth(position);
    theView->setShapeWidth(position);
}

/**
 * Enables current color to be changed for drawing tools.
 */
void MainWindow::colorPaletteChangedSlot(QColor previewColor){
    QPalette palette = ui->colorPaletteWidget->palette();
    palette.setColor(QPalette::Window, previewColor);
    ui->colorPaletteWidget->setPalette(palette);
}

/**
 * Enables the exportation of a gif image.
 */
void MainWindow::on_actionExport_triggered()
{
    gifPopupDialog.show();
}

/**
 * Enables a new image to be created.
 */
void MainWindow::on_actionNew_triggered()
{
    // Create a new file and project
    newProjDialog.show();
}

/**
 * Creates a new sprite project
 */
void MainWindow::createNewSpriteProject(int pixSize)
{
    currentIndex = 0;

    size = pixSize;

    while(testLayout->count() > 0)
    {
        QLayoutItem * item = testLayout->takeAt(0);
        delete item->widget();
    }

    testLayout->update();

    // Remove and replace the view with the new view
    ui->drawingGridLayout->removeWidget(theView);


   // --------------------------The rest seems to work great for creating new projects -----------------------
    // Create new objects and reconnect the slots
    //Create an empty graphicsview to act as the parent for the SlideView
    view = new QGraphicsView();
    //Creates a slide view: extended qGraphicsView with view as its parent

    // If we don't fill theImage before applying it, we get artifacts.
    // I suggest the default background be white.
    //QColor defaultColor = qRgba(255, 255, 255, 0);

     theView = new SlideView(view, size);
    // This makes it so we only use the new slides

    imageList.clear();
    buttons.clear();

    QImage image (size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    theView->setImage(image);

    imageList.push_back(image);


    //set spinboxes range
    ui->paintWidthSlide->setRange(1, theView->getImage().width()/2);
    ui->paintWidthSpin->setRange(1, theView->getImage().width()/2);

   //Adds a the extended slideview to the layout for frame_2
    ui->drawingGridLayout->addWidget(theView);

    // Set up the mini-slide previews so we can see how many slides we have
    QPushButton* preButton = new QPushButton();
    QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
    QSize imageSize((ui->scrollArea->height())-55,(ui->scrollArea->height())-55);
    preButton->setObjectName(QString::number(currentIndex));

    buttons.push_back(preButton);
    buttonsIndex = 0;
    connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));

    QPixmap testMap = QPixmap::fromImage(image);
    testMap = testMap.scaled(imageSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QIcon buttonIcon(testMap);

    preButton->setFixedSize(buttonSize);
    preButton->setIconSize(imageSize);
    preButton->setIcon(buttonIcon);
    preButton->setFlat(false);

    ui->scrollAreaWidgetContents->layout()->addWidget(preButton);

    indexToSet = 0;

    // Reset the connections
    connect(this, &MainWindow::undoSignal, theView, &SlideView::undoSlot);
    connect(this, &MainWindow::redoSignal, theView, &SlideView::redoSlot);
    connect(this, &MainWindow::rotateLeftSignal, theView, &SlideView::rotateLeftSlot);
    connect(this, &MainWindow::rotateRightSignal, theView, &SlideView::rotateRightSlot);
    connect(this, &MainWindow::flipHorizontalSignal, theView, &SlideView::flipHorizontalSlot);
    connect(this, &MainWindow::flipVerticalSignal, theView, &SlideView::flipVerticalSlot);

    connect(theView, &SlideView::updatePalettePreviewSignal, this, &MainWindow::colorPaletteChangedSlot);
    //connect(this, &MainWindow::frameSliderSignal, theView, &SlideView::frameSliderSlot);
    connect(this, &MainWindow::fpsPickerSignal, theProject, &Project::framesPerSecSlot);

    connect(theView, &SlideView::updatePreview, this, &MainWindow::updateButton);
    connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));

    connect(this, &MainWindow::colorPickerSignal, theView, &SlideView::colorPickerSlot);

    connect(this, &MainWindow::colorPickerSignal, theView, &SlideView::colorPickerSlot);
    connect(this, &MainWindow::showPreviewSignal, &previewWindow, &PreviewWindow::previewSlot);

}

/**
 * Opens and loads in a saved sprite file
 */
void MainWindow::on_actionOpen_triggered()
{
    std::cout<<"open"<<std::endl;
    // Open the file and read the data
    // -------------------------------------------------------------------------
    QString filenamePicked = QFileDialog::getOpenFileName(
                this,
                tr("Open Sprite"),
                "C:://",
                "Sprite files (*.ssp);;Text Files (*.txt)");

    QFile file(filenamePicked);

    // If we can't read the file, return
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    // loads in the saved sprite file to the canvas, sets up the canvas appropriately
    QTextStream in(&file);
    QString line = in.readLine();
    QStringList list = line.split(QRegExp("\\s"));
    int count = 0;
    int spriteWidth = list.back().toInt();
    int spriteHeight = list.front().toInt();

    // loads image onto canvas
    imageList.clear();
    line = in.readLine();
    int numFrames = line.toInt();
    if(spriteHeight > spriteWidth)
    {
        for(int i = 0; i < numFrames; i++)
        {
            QImage temp(spriteHeight, spriteHeight, QImage::Format_ARGB32);
            imageList.push_back(temp);
        }
    }
    else
    {
        for(int i = 0; i < numFrames; i++)
        {
            QImage temp(spriteWidth, spriteWidth, QImage::Format_ARGB32);
            imageList.push_back(temp);
        }
    }

    // position on canvas to set the saved image appropriately
    int pos = 0;
    int x = 0;
    int y = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;
    int tmpNum = 0;
    while(!in.atEnd())
    {
        // This reads the lines and puts them into QImages
        QString line = in.readLine();
        list = line.split(QRegExp("\\s"));
        for(auto it = list.begin(); it != list.end(); ++it) {
            tmpNum = (*it).toInt();
            switch(pos % 4) {
                case 0:
                    red = tmpNum;
                break;
                case 1:
                    green = tmpNum;
                break;
                case 2:
                    blue = tmpNum;
                break;
                case 3:
                    alpha = tmpNum;
                break;
            }
            if((pos - 3) % 4 == 0) {
                imageList.at(count).setPixel(x, y, qRgba(red, green, blue, alpha));

                std::cout<<"Setting ("<<x<<", "<<y<<") to "<<red<<", "<<green<<", "<<blue<<", "<<alpha<<std::endl;
            }
            pos++;
            if(pos % 4 == 0)
            {
                x++;
            }
        }
        y++;
        std::cout<<line.toStdString()<<std::endl;
        if(y % spriteHeight == 0)
            count++;
        if(x == spriteWidth) {
            x -= spriteWidth;
        }
        if(y == spriteHeight) {
            y -= spriteHeight;
        }
    }

    std::cout<<imageList.size()<<std::endl;

    // Use the data to initialize the view and signals

    currentIndex = 0;

    std::cout << "Loaded sprite: "<<spriteWidth<<" "<<spriteHeight <<std::endl;

    size = imageList.front().width(); // spriteWidth;

    while(testLayout->count() > 0)
    {
        QLayoutItem * item = testLayout->takeAt(0);
        delete item->widget();
    }

    testLayout->update();

    // Remove and replace the view with the new view
    ui->drawingGridLayout->removeWidget(theView);

    // Create new objects and reconnect the slots
    //Create an empty graphicsview to act as the parent for the SlideView
    view = new QGraphicsView();
    //Creates a slide view: extended qGraphicsView with view as its parent
    theView = new SlideView(view, size);

    // This makes it so we only use the new slides

    ui->paintWidthSlide->setRange(1, theView->getImage().width()/2);
    ui->paintWidthSpin->setRange(1, theView->getImage().width()/2);


    //Adds a the extended slideview to the layout for frame_2
    ui->drawingGridLayout->addWidget(theView);
    std::cout<<buttons.size()<<std::endl;
    buttons.clear();

    for(int i = 0; i < imageList.size(); i++) {
    // Set up the mini-slide previews so we can see how many slides we have
        QPushButton* preButton = new QPushButton();
        preButton->setObjectName(QString::number(i));
        connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));
        QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
        QPixmap testMap = QPixmap::fromImage(imageList.at(i));   //theProject->getSlide(i)->getImage()
        QSize imageSize((ui->scrollArea->height())-55,(ui->scrollArea->height())-55);
        testMap = testMap.scaled(imageSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        QIcon buttonIcon(testMap);
        preButton->setFixedSize(buttonSize);
        preButton->setIconSize(imageSize);
        preButton->setIcon(buttonIcon);
        preButton->setFlat(false);
        //preButton->setObjectName(indexName);
        ui->scrollAreaWidgetContents->layout()->addWidget(preButton);

        buttons.push_back(preButton);
    }
    // Set the image to be the first one in the file
    theView->setImage(imageList.at(0));
    currentFrameIndex = 0;
    ui->frameSlider->setValue(0);

    std::cout<<"imageList size: "<<imageList.size()<<std::endl;
    std::cout<<"num buttons "<<buttons.size()<<std::endl;
    indexToSet = buttons.size() - 1;
    // Reset the connections
    connect(this, &MainWindow::undoSignal, theView, &SlideView::undoSlot);
    connect(this, &MainWindow::redoSignal, theView, &SlideView::redoSlot);
    connect(this, &MainWindow::rotateLeftSignal, theView, &SlideView::rotateLeftSlot);
    connect(this, &MainWindow::rotateRightSignal, theView, &SlideView::rotateRightSlot);
    connect(this, &MainWindow::flipHorizontalSignal, theView, &SlideView::flipHorizontalSlot);
    connect(this, &MainWindow::flipVerticalSignal, theView, &SlideView::flipVerticalSlot);


    connect(theView, &SlideView::updatePalettePreviewSignal, this, &MainWindow::colorPaletteChangedSlot);

    connect(this, &MainWindow::colorPickerSignal, theView, &SlideView::colorPickerSlot);
    connect(theView, &SlideView::updatePreview, this, &MainWindow::updateButton);

}

/**
 * Changes the current image frame
 */
void MainWindow::changeFrame()
{
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();
    indexToSet = senderObjName.toInt();
    on_setFramePushButton_clicked();

}

/**
 * Updates the canvas
 */
void MainWindow::updateButton()
{
    QPixmap testMap = QPixmap::fromImage(theView->getImage());
    imageList.at(indexToSet) = theView->getImage();

    QSize buttonSize((ui->scrollArea->height())-10,(ui->scrollArea->height())-10);
    testMap = testMap.scaled(buttonSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QIcon buttonIcon(testMap);
    buttons.at(indexToSet)->setIcon(buttonIcon);

}

/**
 * Adds frame to list of frames
 */
void MainWindow::on_setFramePushButton_clicked()
{

    std::cout<<indexToSet <<std::endl;
    theView->setImage(imageList.at(indexToSet));
    currentFrameIndex = indexToSet;

    ui->frameSlider->setValue(indexToSet);

}

/**
 * Adds an image frame to canvas
 */
void MainWindow::on_AddFrameButton_clicked()
{
    //set to inital state
    ui->paintWidthSlide->setValue(1);
    ui->paintWidthSpin->setValue(1);
    ui->checkBox_2->setChecked(false);

    currentIndex = theProject->getSizeList();

    QPushButton* preButton = new QPushButton();
    preButton->setObjectName(QString::number(buttons.size()));
    QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
    QSize imageSize((ui->scrollArea->height())-55,(ui->scrollArea->height())-55);
    connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));

    // adds new image frame with selected properties to project
    QImage image (size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    theView->setImage(image);
    theProject->addImage(theView->getImage());
    imageList.push_back(theView->getImage());
    QPixmap testMap = QPixmap::fromImage(theView->getImage());
    currentFrameIndex = buttons.size();

    testMap = testMap.scaled(imageSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QIcon buttonIcon(testMap);
    buttons.push_back(preButton);

    preButton->setFixedSize(buttonSize);
    preButton->setIconSize(imageSize);
    preButton->setIcon(buttonIcon);
    preButton->setFlat(false);

    testLayout->addWidget(preButton);

    indexToSet = buttons.size() -1;

}

/**
 * Removes image frame from canvas
 */
void MainWindow::on_RemoveFrameButton_clicked()
{
    ui->paintWidthSlide->setValue(1);
    ui->paintWidthSpin->setValue(1);
    ui->checkBox_2->setChecked(false);
    theView->setPaintWidth(1);
    theView->setShapeWidth(1);

    int spinValue = indexToSet;

    if( spinValue > 0)
    {
        QLayoutItem * item = testLayout->takeAt(indexToSet);
        delete item->widget();

        for(int i = spinValue +1; i < buttons.size(); i++)
        {
            buttons.at(i)->setObjectName(QString::number(i-1));
        }

        buttons.erase(buttons.begin() + spinValue);

        imageList.erase(imageList.begin() + spinValue);

        theView->setImage(imageList.at(spinValue-1));

        indexToSet = spinValue - 1;


    }
    else
    {
        if(buttons.size() <= 1)
        {
            imageList.clear();
            theProject->deleteAllSlidesAndRefresh();

            QImage image(32, 32, QImage::Format_ARGB32);
            image.fill(Qt::transparent);

            imageList.push_back(image);
            theView->setImage(image);
            theProject->addImage(theView->getImage());
            indexToSet = 0;


        }
        else
        {
            QLayoutItem * item = testLayout->takeAt(indexToSet);
            delete item->widget();

            for(int i = spinValue + 1; i < buttons.size(); i++)
            {
                buttons.at(i)->setObjectName(QString::number(i-1));
            }

            buttons.erase(buttons.begin() + spinValue);

            imageList.erase(imageList.begin() + spinValue);

            theView->setImage(imageList.at(spinValue));
            indexToSet = 0;

        }

    }
    std::cout<<indexToSet<<std::endl;



}

/**
 * Copies image frame on canvas
 */
void MainWindow::on_CopyFrameButton_clicked()
{
    QImage firstImage = imageList.at(indexToSet);
    int startIndex = indexToSet + 1;

    //set to inital state
    ui->paintWidthSlide->setValue(1);
    ui->paintWidthSpin->setValue(1);
    ui->checkBox_2->setChecked(false);

    QPushButton* preButton = new QPushButton();
    preButton->setObjectName(QString::number(startIndex));
    QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
    QSize imageSize((ui->scrollArea->height())-55,(ui->scrollArea->height())-55);
    connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));

    // transfers copied image to an image list
    theView->setImage(firstImage.copy());
    theProject->addImage(theView->getImage());
    imageList.insert(imageList.begin()+startIndex, theView->getImage());

    QPixmap testMap = QPixmap::fromImage(theView->getImage());
    currentFrameIndex = startIndex;

    testMap = testMap.scaled(imageSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QIcon buttonIcon(testMap);
    buttons.insert(buttons.begin() + startIndex, preButton);

    preButton->setFixedSize(buttonSize);
    preButton->setIconSize(imageSize);
    preButton->setIcon(buttonIcon);
    preButton->setFlat(false);


    testLayout->insertWidget(startIndex,preButton, 0, Qt::AlignCenter);

    for(int i = startIndex+1; i < buttons.size(); i++)
    {

        QPushButton* temp = buttons[i];
        QString s = temp->objectName();
        int nextIndex = s.toInt()+1;
        temp->setObjectName(QString::number(nextIndex));


    }
    indexToSet++;
}

/**
 * Merges canvas images together
 */
void MainWindow::on_MergeFrameButton_clicked()
{
    int startIndex = 0;

    if(indexToSet != 0)
    {
        QImage mergedImage;

        QImage topImage = imageList.at(indexToSet).copy();
        QImage bottomImage = imageList.at(indexToSet-1).copy();

        QPixmap mergedMap(bottomImage.size());
        mergedMap.fill(Qt::transparent);
        QPainter p (&mergedMap);
        p.drawImage(QPoint(0,0), bottomImage);
        p.drawImage(QPoint(0,0), topImage);
        p.end();

        mergedImage = mergedMap.toImage();

        startIndex = indexToSet + 1;

        //set to inital state;
        ui->paintWidthSlide->setValue(1);
        ui->paintWidthSpin->setValue(1);
        ui->checkBox_2->setChecked(false);

        QPushButton* preButton = new QPushButton();
        preButton->setObjectName(QString::number(startIndex));
        QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
        QSize imageSize((ui->scrollArea->height())-55,(ui->scrollArea->height())-55);
        connect(preButton,SIGNAL(clicked()),this,SLOT(changeFrame()));

        theView->setImage(mergedImage);
        theProject->addImage(theView->getImage());
        imageList.insert(imageList.begin()+startIndex, theView->getImage());

        QPixmap testMap = QPixmap::fromImage(theView->getImage());
        currentFrameIndex = startIndex;

        testMap = testMap.scaled(imageSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);
        QIcon buttonIcon(testMap);
        buttons.insert(buttons.begin() + startIndex, preButton);

        preButton->setFixedSize(buttonSize);
        preButton->setIconSize(imageSize);
        preButton->setIcon(buttonIcon);
        preButton->setFlat(false);

        testLayout->insertWidget(startIndex,preButton, 0, Qt::AlignCenter);

        for(int i = startIndex+1; i < buttons.size(); i++)
        {


            QPushButton* temp = buttons[i];
            QString s = temp->objectName();
            int nextIndex = s.toInt()+1;
            temp->setObjectName(QString::number(nextIndex));

        }

        indexToSet = startIndex;
    }
    else
    {
        on_CopyFrameButton_clicked();
    }
}

void MainWindow::on_IncreaseIndexButton_clicked()
{
    int index = indexToSet;
        if(buttons.size() - 1 == index)
        {

        }
        else
        {
            std::iter_swap(imageList.begin() + index, imageList.begin() + (index + 1));
            QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
            QPixmap testMap = QPixmap::fromImage(imageList.at(index));
            testMap = testMap.scaled(buttonSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);

            QIcon buttonIcon(testMap);


            buttons.at(index)->setIcon(buttonIcon);

            QPixmap testMap2 = QPixmap::fromImage(imageList.at(index + 1));
            testMap2 = testMap2.scaled(buttonSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);

            QIcon buttonIcon2(testMap2);

            buttons.at(index + 1)->setIcon(buttonIcon2);
            indexToSet = index + 1;
        }
}

/**
 * Rewinds the current preview window
 */
void MainWindow::on_DecreaseIndexButton_clicked()
{
    int index = indexToSet;
    if(0 == index)
    {

    }
    else
    {
        std::iter_swap(imageList.begin() + (index - 1), imageList.begin() + index);
        QSize buttonSize((ui->scrollArea->height())-40,(ui->scrollArea->height())-40);
        QPixmap testMap = QPixmap::fromImage(imageList.at(index - 1));
        testMap = testMap.scaled(buttonSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);

        QIcon buttonIcon(testMap);


        buttons.at(index - 1)->setIcon(buttonIcon);

        QPixmap testMap2 = QPixmap::fromImage(imageList.at(index));
        testMap2 = testMap2.scaled(buttonSize,Qt::IgnoreAspectRatio, Qt::FastTransformation);

        QIcon buttonIcon2(testMap2);

        buttons.at(index)->setIcon(buttonIcon2);
        indexToSet = index - 1;
    }


}

/**
 * Selects the color for the canvas
 */
bool MainWindow::eventFilter(QObject *sender, QEvent *event)
{
    if (sender == ui->colorPaletteWidget)
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            std::cout << "clicked" << std::endl;
            QColor color;
            color = QColorDialog::getColor(Qt::white, this, "Color Picker", QColorDialog::DontUseNativeDialog);
            if (color.isValid()) {
                emit colorPaletteChangedSlot(color);
                emit colorPickerSignal(color);
            }
        }
    }
    return QWidget::eventFilter(sender,event);
}

/**
 * Slides the preview window the target image
 */
void MainWindow::on_frameSlider_valueChanged(int value)
{
    if (imageList.size() == 1) {

    }
    else {
        ui->frameSlider->setMaximum(imageList.size() - 1);
        std::cout << value << std::endl;

        theView->setImage(imageList.at(value));
        currentFrameIndex = value;

    }
}

/**
 * For working purpose of gui
*/
//// error if removed.
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    std::cout << "what" << std::endl;
}

/**
 * Selects the speed at which the animation plays
 */
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    emit fpsPickerSignal(arg1);
}

/**
 * Enables gif file to be exported
 */
void MainWindow::exportGifSlot(std::string name)
{

    name += ".gif";
    const char* cname = name.c_str();

    QImage slide = imageList.front();
    int width = slide.width();
    int height = slide.height();
    int framesPerSec = theProject->getFramesPerSec(); // Set this variable when we change the frame playback speed
    int delay = 100/framesPerSec; // This is the delay in 1/100th of a second. 5 corresponds to 25 frames per second
    GifWriter gifWrt;
    GifBegin(&gifWrt, cname, width, height, delay, 8, false);

    for(auto itr = imageList.begin(); itr != imageList.end(); ++itr)

    {

        QImage tempImg = *itr; // tempSlide.convertToFormat(QImage::Format_RGB32);
        width = tempImg.width();
        height = tempImg.height();

        GifWriteFrame(&gifWrt, tempImg.bits(), width, height, delay, 8, false);

        std::cout<<"Writing frame"<<std::endl;

    }
    GifEnd(&gifWrt);
}

/**
 * Shows the preview window
 */
void MainWindow::on_pushButton_clicked()
{

    emit showPreviewSignal(ui->fpsBox->value(), imageList);
    previewWindow.show();
}

/**
 * Saves the current sprite image to a file
 */
void MainWindow::on_actionSave_triggered()
{
    // Save it
    QString filenamePicked = QFileDialog::getSaveFileName(
                    this,
                    tr("Save Sprite"),
                    "C:://",
                    "Sprite file (*.ssp)");
        std::cout<<filenamePicked.toStdString()<<std::endl;
        QFile saveFile(filenamePicked);
        if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
        QTextStream out(&saveFile);
        // Start writing to file
    std::cout<<"Ready to save"<<std::endl;
    QImage temp = imageList.front();
    int saveWidth = temp.width();
    int saveHeight = temp.height();
    int numFrames2Save = imageList.size();
    int count = 0;

    out<<saveHeight<<" "<<saveWidth<<"\n";
    out<<numFrames2Save<<"\n";
    for(auto it = imageList.begin(); it != imageList.end(); ++it) {
        temp = *it;
        for(int y = 0; y < saveHeight; y++) {
            // This will go from top to bottom
            for(int x = 0; x < saveWidth; x++) {
                QColor qc = temp.pixelColor(x,y);
                out<<qc.red()<<" "<<qc.green()<<" "<<qc.blue()<<" "<<qc.alpha();
                if(x + 1 != saveWidth)
                    out<<" ";
            }
            if((y != saveHeight-1) || (count != numFrames2Save-1))
                out << "\n";
        }
        count++;
    }
    saveFile.close();
}

/**
 * Closes the current sprite editor.
 */
void MainWindow::on_actionExit_triggered()
{
    this->close();
}
