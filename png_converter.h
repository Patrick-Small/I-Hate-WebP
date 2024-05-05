#ifndef PNG_CONVERTER_H
#define PNG_CONVERTER_H

#include <QDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

enum class ConvertTypePNG {
    WebP,
    JPEG
};

namespace Ui {
class PNG_converter;
}

class PNG_converter : public QDialog
{
    Q_OBJECT

public:
    explicit PNG_converter(QWidget *parent = nullptr);
    ~PNG_converter();

private slots:
    void on_select_file_clicked();

    void on_Convert_clicked();

    void on_back_button_clicked();

    void updateImageDisplay();

    void updateSizeLabels();

    void closeEvent (QCloseEvent *event);

private:
    Ui::PNG_converter *ui;

    void convertFile (const QString &filePath, ConvertTypePNG conversionType);

    QSettings settings;

    QImage originalImage;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
};

#endif // PNG_CONVERTER_H
