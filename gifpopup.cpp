/**
    Programmer:     Tambra Smith
    Last updated:   November 7, 2016
    Description:    This is the pop up box code to create the name of the gif to be exported

*/
#include "gifpopup.h"
#include "ui_gifpopup.h"
#include <string>


//Description: Constructor for the pop up, sets window title
//Parameters:    N/A
//Return:        N/A
gifPopup::gifPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gifPopup)
{
    ui->setupUi(this);  
    this->setWindowTitle("Export Gif");
}

//Description: Destuctor for the pop up, clears the ui
//Parameters:    N/A
//Return:        N/A
gifPopup::~gifPopup()
{
    delete ui;
}

//Description: Listener method for the Accept button in the pop up. sets gif name
//Parameters:    N/A
//Return:        N/A
void gifPopup::on_buttonBox_accepted()
{
    std::string name = ui->gifFileNameBox->text().toStdString();
    if(name.empty())
    {
        emit gifFileNameEntered("untitled");
    }
    else
    {
        emit gifFileNameEntered(name);
    }
    this->close();
}


/*
 * Code Review: Lacey Taylor
 */
