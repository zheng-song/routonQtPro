#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    fileList = new QTreeView;
    fileList->setModel(model);


    label1 = new QLabel("label1");
    label2 = new QLabel("label2");
    label3 = new QLabel("label3");
    label4 = new QLabel("label4");

    label5 = new QLabel("label5");
    QPainter paint;
    paint.begin(&p);
    paint.drawText(0,50,"hello world");
    paint.end();

//    p = p.scaled(10,10);
    label1->setPixmap(p);

    checkBox1 = new QCheckBox;
    checkBox2 = new QCheckBox;
    checkBox3 = new QCheckBox;
    checkBox4 = new QCheckBox;
    checkBox5 = new QCheckBox;

    QHBoxLayout *H1 = new QHBoxLayout;
    QHBoxLayout *H2 = new QHBoxLayout;
    QHBoxLayout *H3 = new QHBoxLayout;
    QHBoxLayout *H4 = new QHBoxLayout;
    QHBoxLayout *H5 = new QHBoxLayout;

    H1->addWidget(checkBox1);
    H1->addWidget(label1);
    H2->addWidget(checkBox2);
    H2->addWidget(label2);
    H3->addWidget(checkBox3);
    H3->addWidget(label3);
    H4->addWidget(checkBox4);
    H4->addWidget(label4);
    H5->addWidget(checkBox5);
    H5->addWidget(label5);

    QVBoxLayout *V = new QVBoxLayout;
    V->addLayout(H1);
    V->addLayout(H2);
    V->addLayout(H3);
    V->addLayout(H4);
    V->addLayout(H5);

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setHorizontalSpacing(10);
    gLayout->addLayout(V,0,0);
    gLayout->addWidget(fileList,0,1);

    setLayout(gLayout);

}

