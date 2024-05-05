/*
 * This program handles all functionality of the JPEG Converter page in
 * the "I Hate WebP" Project. This program contains the ability to select
 * a JPEG image from a file explorer, have that image appear on the screen,
 * give the user the ability to retain transparency if there is any, and
 * give the user the ability to scale the image to a specific size.
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


#include "jpeg_converter.h"
#include "ui_jpeg_converter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QBuffer>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QImageWriter>
#include <QPainter>


JPEG_converter::JPEG_converter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JPEG_converter)
{
    ui->setupUi(this);

    //Fill the drop down box
    ui -> convert_type -> addItem("WebP", QVariant::fromValue(ConvertTypeJPEG::WebP));
    ui -> convert_type -> addItem("PNG", QVariant::fromValue(ConvertTypeJPEG::PNG));

    //Connect the size text box to the labels
    connect(ui -> size, &QLineEdit::textChanged, this, &JPEG_converter::updateSizeLabels);

    //Handling image display
    scene = new QGraphicsScene(this);
    ui -> image_view -> setScene(scene);

    //Panning and Movement
    ui -> image_view -> setDragMode(QGraphicsView::ScrollHandDrag);
    ui -> image_view -> setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //Set "size" to 1.00 by default
    ui -> size -> setText("1.00");

    //Replace height and width labels with nothing
    ui -> height -> setText("");
    ui -> width -> setText("");
}

JPEG_converter::~JPEG_converter()
{
    delete ui;
}

//When the button is clicked, bring up a file explorer for the user to select a file
void JPEG_converter::on_select_file_clicked()
{
    //Type of filter
    QString filter = "JPEG File (*.jpg *.jpeg)";

    //Get previously used directory
    QString prevDir = settings.value("Previous Directory").toString();

    //Error handling
    if (!prevDir.isEmpty()) {
        prevDir = QDir::homePath();
    }

    //Save the path of the file the user selected
    QString path = QFileDialog::getOpenFileName(this, tr("Open JPEG File"), prevDir, filter);

    //Save the path to settings
    settings.setValue("Previous Directory", path);

    //If the user selected a file
    if(!path.isEmpty())
    {
        //Display the path name in the box
        ui -> file_path -> setText(path);

        //Load the image
        QImage image;
        if (!image.load(path)) {
            QMessageBox::warning(this, tr("Error"), tr("Failed to load the JPEG file."));
            return;
        }

        //Save the original image for later use
        originalImage = image;

        //Update the display image
        updateImageDisplay();
        updateSizeLabels();
    }
}

//This handles when the user is done with the settings and wants to convert the file
void JPEG_converter::on_Convert_clicked()
{
    //Gets the file path
    QString path = ui -> file_path -> text();

    if (path.isEmpty())
    {
        //Display error if no file was selected, then exit
        QMessageBox::warning(this, tr("Error"), tr("Please select a JPEG file."));
        return;
    }

    //Get the currently selected file extension
    ConvertTypeJPEG conversionType = ui -> convert_type -> currentData().value <ConvertTypeJPEG>();

    //Go do stuff!
    convertFile(path, conversionType);
}

//This handles converting the file
void JPEG_converter::convertFile(const QString &filePath, ConvertTypeJPEG conversionType)
{
    //Load a new image
    QImage image;
    if (!image.load(filePath)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load the JPEG file."));
        return;
    }

    //Check if the "Retain Transparency" checkbox is checked
    bool retainTransparency = ui->transparency->isChecked();

    //Transparency retention
    QImageWriter writer;

    //Format to png for transparency settings
    writer.setFormat("PNG");
    if (retainTransparency) {
        //Set option to retain transparency
        writer.setCompression(1);
        writer.setText("creation_time", "now");
    }
    else
    {
        //Set option to replace transparency with white background
        writer.setCompression(9);

        //Create a new image with a white background
        QImage newImage(image.size(), QImage::Format_ARGB32);
        newImage.fill(Qt::white);

        //Copy the original image onto the new image, replacing transparent pixels with white
        QPainter painter(&newImage);
        painter.drawImage(0, 0, image);
        painter.end();

        //Use the new image for saving
        image = newImage;
    }

    //Condense filePath down to just the name & extension
    QFile f(filePath);
    QString filename = QFileInfo(f).baseName();

    //For the extension type
    QString extension = "Images ";

    //Add the extension on the end
    switch (conversionType) {
    case ConvertTypeJPEG::WebP:
        filename += ".webp";
        extension += "(*.webp)";
        break;
    case ConvertTypeJPEG::PNG:
        filename += ".png";
        extension += "(*.png)";
        break;
    }

    //Get the previous saving location
    QString prevSave = settings.value("Previous Save").toString();

    //Prompt the user to select a directory
    QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save Image to..."), prevSave, extension);

    //If user exits this prompt
    if (saveFilePath.isEmpty())
    {
        return;
    }

    //Save the path to settings
    settings.setValue("Previous Save", saveFilePath);

    //Save the image with scaling
    bool ok;
    qreal scale = ui->size->text().toFloat(&ok);
    if (!ok || scale <= 0) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid scaling factor."));
        return;
    }

    //Scale the image
    image = image.scaled(image.width() * scale, image.height() * scale);

    //Save the image
    if (image.save(saveFilePath))
    {
        QMessageBox::information(this, tr("File Saved Successfully"), saveFilePath);
    }

}

//This function updates the display image
void JPEG_converter::updateImageDisplay()
{
    //Clear previous items from the scene
    scene->clear();

    //Create a QPixmap from the original image
    QPixmap pixmap = QPixmap::fromImage(originalImage);

    //Create a QGraphicsPixmapItem and add it to the scene
    pixmapItem = scene->addPixmap(pixmap);
}

//This function updates the labels that tell the user the size of the image
void JPEG_converter::updateSizeLabels()
{
    //Get the scale factor from the size line edit
    bool ok;
    qreal scale = ui->size->text().toFloat(&ok);
    if (!ok || scale <= 0) {
        return; // Invalid scale factor
    }

    //Update the height and width labels with the scaled image size
    int scaledWidth = originalImage.width() * scale;
    int scaledHeight = originalImage.height() * scale;
    ui->height->setText(QString::number(scaledHeight));
    ui->width->setText(QString::number(scaledWidth));
}

//If the user wants to return to the main menu
void JPEG_converter::on_back_button_clicked()
{
    //Hide current page
    this -> hide();

    //Open the main window
    QWidget *parent = this -> parentWidget();
    parent -> show();
}

//When the user closes the application
void JPEG_converter::closeEvent(QCloseEvent *event)
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
