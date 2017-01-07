/**
    Programmer:     Thuy Nguyen, Steven Reese, Lacey Tay, Tambra Smith, Andrew Tsai
    Last updated:   November 07, 2016
    Description:    Slide view provides the algorithm to modify the sprite editor canvas, enabling user to create pixel art.

*/
#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

//Description: Constructor for the new project popup, takes in the parent widget for inheritance
//Parameters:    QWidget parent to initialize QDialog
//Return:        N/A
NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    // set the default size to be 10px
    size = 10;
    this->setWindowTitle("New Sprite Project Size");
    // populate the combo box
    ui->sizeComboBox->addItem("10x10");
    ui->sizeComboBox->addItem("30x30");
    ui->sizeComboBox->addItem("50x50");
    ui->sizeComboBox->addItem("100x100");

    // make a connection to changes in the values
    connect(ui->sizeComboBox, SIGNAL(activated(int)), this, SLOT(onUpdateSize()));
}

//Description: Destructor for the pop up, cleares the UI
//Parameters:    N/A
//Return:        N/A
NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

//Description: Listener method to change the size of the new project based on change in ui
//Parameters:    N/A
//Return:        N/A
void NewProjectDialog::onUpdateSize()
{
    // update size variable
    int pixSize = ui->sizeComboBox->currentIndex();
    switch (pixSize)
    {
      case 0:
         size = 10;
         break;
      case 1:
         size = 30;
         break;
      case 2:
         size = 50;
         break;
      case 3:
         size = 100;
         break;
   }
}

void NewProjectDialog::on_buttonBox_accepted()
{
    emit createNewProj(size);
    this->close();
}

/*
 * Code Review: Lacey Taylor
 */
