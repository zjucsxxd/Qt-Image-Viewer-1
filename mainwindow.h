#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
    class ImageWindow;
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
    void doAbout();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
