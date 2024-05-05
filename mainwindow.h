#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "webp_converter.h"
#include "png_converter.h"
#include "jpeg_converter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_goto_WebP_clicked();

    void on_goto_PNG_clicked();

    void on_goto_JPEG_clicked();

    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    WebP_converter *webp_converter;
    PNG_converter *png_converter;
    JPEG_converter *jpeg_converter;
};
#endif // MAINWINDOW_H
