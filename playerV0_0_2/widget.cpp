#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),ui(new Ui::Widget),
    isPlay(true),isSound(true),isStop(true)
{
    ui->setupUi(this);
    /*
    为按键添加图标
    */


/****************** 获得播放列表 ********************/
    directory.setPath("/home/zs/qtBuild/video");
    files = directory.entryList(QDir::AllEntries,QDir::Time);
    file_name = files[2]; //文件0和1为 /. 和 /.. ,所以从2开始播放
    file_count = 2;
    label_3.setText(files[2]);


/****************** 初始化进度条及QProcess类 ********************/
    horizontalSilder.setPageStep(1);
    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
}




Widget::~Widget()
{
    delete ui;
}
