#ifndef JPEG_CONVERTER_H
#define JPEG_CONVERTER_H

#include <QDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

enum class ConvertTypeJPEG {
    WebP,
    PNG
};

namespace Ui {
class JPEG_converter;
}

class JPEG_converter : public QDialog
{
    Q_OBJECT

public:
    explicit JPEG_converter(QWidget *parent = nullptr);
    ~JPEG_converter();

private slots:
    void on_select_file_clicked();

    void on_Convert_clicked();

    void on_back_button_clicked();

    void updateImageDisplay();

    void updateSizeLabels();

    void closeEvent (QCloseEvent *event);

private:
    Ui::JPEG_converter *ui;

    void convertFile (const QString &filePath, ConvertTypeJPEG conversionType);

    QSettings settings;

    QImage originalImage;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
};

#endif // JPEG_CONVERTER_H
