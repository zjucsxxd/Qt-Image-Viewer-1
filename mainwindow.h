#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void doOpen();
    void doNegate();
    void doSharpen();
    void doCrop();
    //void doDesaturate();
    void doAbout();

private:
    const QPixmap* getPixmap();
    void setPixmap(QPixmap p);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
