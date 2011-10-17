/******************************************************************************
 * main.cpp
 * The base of the application.
 * Author: N/A
 * Created by Qt Creator, no changes made.
 * Known issues: None
 *****************************************************************************/
#include <QtGui/QApplication>
#include "mainwindow.h"

/******************************************************************************
 * main(int argc, char *argv[]): Start the application.
 *****************************************************************************/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
