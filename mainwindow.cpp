#include "mainwindow.hpp"
#include "ui_mainwindowform.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, mainWindowUI{new Ui::MainWindow}
{
    mainWindowUI->setupUi(this);
    scene = new SceneView(mainWindowUI->frame);

    QGridLayout *layout = new QGridLayout(mainWindowUI->frame);
    layout->setContentsMargins(0,0,0,0);
}
