#include "mymainwindow.h"
#include "spreadsheet.h"
#include "finddialog.h"
#include <QtWidgets>

MyWindows1::MyWindows1()
{
    //创建一个SpreadSheet窗口部件并把它设置为这个主窗口的中央窗口部件
    spreadSheet = new SpreadSheet;
    setCentralWidget(spreadSheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBar();
    createStatusBar();

    readSettings();
    findDialog = 0;//不创建对象,直接赋值为空.第一次调用slotFind()函数的时候再创建对象

    setWindowTitle(tr("myWindows1"));
    setCurrentFile("");
}

void MyWindows1::createActions()
{
    newAction = new QAction(tr("&New"),this);
    newAction->setIcon(QIcon(""));

/*动作有一个快捷键,用于特定的标准化的键盘快捷键.
 *通过使用适当的QKeySequence::StandardKey枚举值,就可以确保Qt能够为应用程序在其平台上提供正确的快捷键
 */
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction,SIGNAL(triggered(bool)),this,SLOT(slotNewFile()));

    saveAction = new QAction(tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(saveAction,SIGNAL(triggered(bool)),this,SLOT(slotSave()));


    for(int i =0;i<MaxRecentFiles;++i)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered(bool)),this,SLOT(slotOPenRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"),this);

    /*由于没有用于终止应用程序的标准化键序列,所以在此处明确的指定键的序列*/
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the Application"));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));


    selectAllAction = new QAction(tr("&All"),this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("select all the cells in the speraedsheet"));
    //selectAll()是QTableWidget的父类QAbstractItemView提供的.
    connect(selectAllAction,SIGNAL(triggered(bool)),spreadSheet,SLOT(selectAll()));

}

void MyWindows1::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
}
