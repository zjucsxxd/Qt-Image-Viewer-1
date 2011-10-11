#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMdiSubWindow;
class ImgWin;

namespace Ui {
    class MainWindow;
    class AboutDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void doOpen();
    void doSave();
    void doRevert();
    void doNegate();
    void doSharpen();
    void doCrop();
    void doZoom();
    //void doDesaturate();
    void doAbout();
    void doInfo();
    void doChangeImage(QMdiSubWindow*);
    void removeWindowListItem(QAction* act);

private:
    ImgWin* getCurrent();
    const QPixmap* getPixmap();
    void setPixmap(QPixmap p);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
