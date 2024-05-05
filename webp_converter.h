#ifndef WEBP_CONVERTER_H
#define WEBP_CONVERTER_H

#include <QDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

enum class ConvertTypeWebP {
    PNG,
    JPEG
};

namespace Ui {
class WebP_converter;
}

class WebP_converter : public QDialog
{
    Q_OBJECT

public:
    explicit WebP_converter(QWidget *parent = nullptr);
    ~WebP_converter();

private slots:
    void on_select_file_clicked();

    void on_Convert_clicked();

    void on_back_button_clicked();

    void updateImageDisplay();

    void updateSizeLabels();

    void closeEvent (QCloseEvent *event);

private:
    Ui::WebP_converter *ui;

    void convertFile (const QString &filePath, ConvertTypeWebP conversionType);

    QSettings settings;

    QImage originalImage;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
};

#endif // WEBP_CONVERTER_H
