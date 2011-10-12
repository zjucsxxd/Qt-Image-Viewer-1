#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMdiSubWindow;
class ImgWin;

namespace Ui {
    class MainWindow;
    class AboutDialog;
    class ImgResizeDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    void doInfo();
    void doAbout();

private:
    ImgWin* getCurrent();
    const QPixmap* getPixmap();
    void setPixmap(QPixmap p);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
