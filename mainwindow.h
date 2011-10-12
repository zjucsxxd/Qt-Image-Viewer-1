#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMdiSubWindow;
class QLabel;
class QSlider;
class ImgWin;

namespace Ui {
    class MainWindow;
    class ImgResizeDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void imageEditable(bool);

private slots:
    // File operations
    void doOpen();
    void doSave();
    void doRevert();
    // Image manipulations
    void doNegate();
    void doSharpen();
    void doCrop();
    void doResize();
    void doSmooth();
    // Menu disabling functions
    void doChangeImage(QMdiSubWindow*);
    void removeWindowListItem(QAction* act);
    // Dialogs
    void doSliders();
    void doZoom();
    void doFillWindow();
    void doInfo();
    void doAbout();
    // Mouseover info
    void imgMouseInfo(QPoint);

private:
    ImgWin* getCurrent();
    QImage getImage();
    void setImage(QImage p);

    Ui::MainWindow *ui;
    QSlider *zoom_slider;
    QWidget *status_color_swatch;
};

#endif // MAINWINDOW_H
