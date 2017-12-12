#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QWidget>
#include <QTableWidget>

class SpreadSheet:public QTableWidget
{
    Q_OBJECT

public:
    SpreadSheet(QTableWidget *parent =0);
    ~SpreadSheet();

public slots:


};

#endif // SPREADSHEET_H
