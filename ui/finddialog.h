#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class FindDialog:public QWidget
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent=0);
    ~FindDialog();

    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void findPrevious(QString,Qt::CaseSensitivity);
    void findNext(QString,Qt::CaseSensitivity);

private slots:
    void slotEnableFindButton(const QString &);
    void slotSearchButtonClicked();
    void slotCloseButtonClicked();
    void slotFindContent(QString,Qt::CaseSensitivity);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *checkBox1;
    QCheckBox *checkBox2;
    QPushButton *searchButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
