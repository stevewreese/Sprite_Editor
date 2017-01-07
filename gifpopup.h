/**
    Programmer:     Tambra Smith
    Last updated:   November 7, 2016
    Description:    This is the pop up box code to create the name of the gif to be exported

*/
#ifndef GIFPOPUP_H
#define GIFPOPUP_H

#include <QDialog>
#include <string>

namespace Ui {class gifPopup;}

class gifPopup : public QDialog
{
    Q_OBJECT

public:
    explicit gifPopup(QWidget *parent = 0);
    ~gifPopup();
signals:
    void gifFileNameEntered(std::string name);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::gifPopup *ui;
};

#endif // GIFPOPUP_H

/*
 * Code Review: Lacey Taylor
 */
