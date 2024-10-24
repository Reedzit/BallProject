#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include "sceneview.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new SceneView();

    QGridLayout *layout = new QGridLayout(ui->frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(scene);
    ui->frame->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
