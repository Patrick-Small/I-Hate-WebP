/*
 * This program handles all functionality of the Main Page in
 * the "I Hate WebP" Project. It displays 3 buttons for the user
 * to click, each one sending the user to a different image converter
 * (WebP to..., PNG to..., and JPEG to...). It also asks the user if
 * they would like to quit when they close the application.
 *
 * Written by Patrick Small
 * Created for Undergraduate Capstone Spring 2024
 * SUNY Polytechnic Institute of Technology
 * 05/05/2024
*/

/*
 * Resources Used
 *
 *
 * Qt C++ GUI Tutorial For Beginners:
 * https://www.youtube.com/playlist?list=PLS1QulWo1RIZiBcTr5urECberTITj7gjA
 *
 * This playlist was used for many purposes in learning about the QT libraries,
 * understanding the QT Creator IDE, and finding techniques for the purposes
 * of this project.
 *
 *
 * Qt C++ Documentation:
 * https://doc.qt.io/
 *
 * This documentation was heavily utilized for understanding classes and functions
 * in various Qt C++ Libraries.
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Sends the user to the WebP page when clicked
void MainWindow::on_goto_WebP_clicked()
{
    //Hide current page
    this -> hide();

    webp_converter = new WebP_converter(this);
    webp_converter -> show();
}

//Sends the user to the PNG page when clicked
void MainWindow::on_goto_PNG_clicked()
{
    //Hide current page
    this -> hide();

    png_converter = new PNG_converter(this);
    png_converter -> show();
}

//Sends the user to the JPEG page when clicked
void MainWindow::on_goto_JPEG_clicked()
{
    //Hide current page
    this -> hide();

    jpeg_converter = new JPEG_converter(this);
    jpeg_converter -> show();
}

//When the user closes the application
void MainWindow::closeEvent(QCloseEvent *event)
{
    //Ask if they want to quit
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Warning"),
                                                              "Do you really want to quit?",
                                                              QMessageBox::Yes | QMessageBox::No);

    //Check the response
    if (reply == QMessageBox::Yes)
    {
        //Close the program
        event -> accept();
        QCoreApplication::quit();
    }
    else
    {
        //Keep the program open
        event -> ignore();
    }

}

