/**
    Programmer:     Thuy Nguyen, Steven Reese, Lacey Tay, Tambra Smith, Andrew Tsai
    Last updated:   November 07, 2016
    Description:    MainWindow connects any event on the gui with its appropriate implementation.

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <project.h>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPalette>
#include "slideview.h"
#include "gifpopup.h"
#include "newprojectdialog.h"
#include <QColorDialog>
#include "previewwindow.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void changeFrame();
    void updateButton();
    void on_AddFrameButton_clicked();
    void on_setFramePushButton_clicked();

    // for undo/redo events and frame flipping
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_RotateLeftButton_clicked();
    void on_RotateRightButton_clicked();
    void on_FlipHorzButton_clicked();
    void on_FlipVertButton_clicked();

    //Slots to set the value of the tool being used
    void on_LineButton_clicked();
    void on_CircleButton_clicked();
    void on_SquareButton_clicked();
    void on_PenButton_clicked();
    void on_PaintBrushButton_clicked();
    void on_FillButton_clicked();
    void on_EraseButton_clicked();
    void on_EyeDropperButton_clicked();

    //Slots for the checkboxes used for the tools
    void on_checkBox_2_stateChanged(int arg1);

    //Slots for the width and size changers
    void on_paintWidthSpin_valueChanged(int arg1);
    void on_paintWidthSlide_sliderMoved(int position);

    // Receives Signal from UI EyeDropperButton
    void colorPaletteChangedSlot(QColor previewColor);


    // Receives signal to bring up export dialog
    void on_actionExport_triggered();

    // Receives signal to bring up new project dialog
    void on_actionNew_triggered();

    // Receives signal to create a new project
    void createNewSpriteProject(int pixSize);

    // signals for when gui buttons are clicked.
    void on_actionOpen_triggered();

    void on_CopyFrameButton_clicked();

    void on_MergeFrameButton_clicked();

    void on_RemoveFrameButton_clicked();

    void on_IncreaseIndexButton_clicked();

    void on_DecreaseIndexButton_clicked();

    void exportGifSlot(std::string name);

    void on_frameSlider_valueChanged(int value);

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

// signals emitted to SlideView when user interacts with gui
signals:
    void undoSignal();
    void redoSignal();
    void rotateLeftSignal();
    void rotateRightSignal();
    void flipHorizontalSignal();
    void flipVerticalSignal();
    void colorPickerSignal(QColor color);
    void fpsPickerSignal(int value);
    void showPreviewSignal(int value, const std::vector<QImage>& imageList);


protected:
    bool eventFilter( QObject* sender, QEvent* event);

private:
    Ui::MainWindow *ui;
    int currentIndex = 0;
    int indexToSet = 0;
    int size = 0;
    int buttonsIndex = 0;
    int currentFrameIndex =0;

    // objects that the main window needs to set up a gui
    SlideView* theView;  // the canvas window
    Project* theProject; // holds the frames
    QHBoxLayout* testLayout;  // layout for gui
    QGraphicsView* view;
    QGraphicsPixmapItem *pixMap;  // holds canvas
    PreviewWindow previewWindow;

    // all the buttons on the gui
    std::vector<QPushButton*> buttons;

    // list of images created on the sprite editor
    std::vector<QImage> imageList;

    // dialog boxes
    gifPopup gifPopupDialog;
    NewProjectDialog newProjDialog;

};

#endif // MAINWINDOW_H
