#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class SpreadSheet;
class FindDialog;
class QPushButton;

class MyWindows1:public QMainWindow
{
    Q_OBJECT
public:
    MyWindows1();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void slotNewFile();
    void slotoOpen();
    bool slotSave();
    bool slotSaveAs();
    void slotFind();
    void slotGotoCell();
    void slotSort();
    void slotAbout();
    void slotOPenRecentFile();
    void slotUpdateStatusBar();
    void slotSpreadSheetModified();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBar();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QStirng &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);


 /*-----------------------------------------*/
    SpreadSheet *spreadSheet;
    FindDialog  *findDialog;
    QLabel      *locationLabel;
    QLabel      *formulaLabel;
    QStringList *recentFiles;
    QString     curFile;

    enum {MaxRecentFiles = 5};
    QAction     *recentFileActions[MaxRecentFiles];
    QAction     *separatorAction;

    QMenu       *fileMenu;
    QMenu       *editMenu;
    QToolBar    *fileToolBar;
    QToolBar    *editToolBar;
    QAction     *newAction;
    QAction     *openAction;
    QAction     *saveAction;
    QAction     *exitAction;
    QAction     *selectAllAction;
    QAction     *aboutAction;
};

#endif // MYMAINWINDOW_H
