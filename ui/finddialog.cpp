#include "finddialog.h"
#include <QtWidgets>

FindDialog::FindDialog(QWidget *parent)
    :QWidget(parent)
{
    label = new QLabel("Find &What");
    lineEdit = new QLineEdit;
    lineEdit->setEchoMode(QLineEdit::Password);
    label->setBuddy(lineEdit);

    checkBox1 = new QCheckBox("Match &case");
    checkBox2 = new QCheckBox("Search &backward");

    searchButton = new QPushButton("&Find");
    searchButton->setEnabled(false);
    searchButton->setDefault(true);

    closeButton = new QPushButton("Close");
    QObject::connect(lineEdit,SIGNAL(textChanged(const QString &)),\
                     this,SLOT(slotEnableFindButton(const QString &)));
    QObject::connect(searchButton,SIGNAL(clicked(bool)),\
                     this,SLOT(slotSearchButtonClicked()));
    QObject::connect(closeButton,SIGNAL(clicked(bool)),\
                     this,SLOT(slotCloseButtonClicked()));
//    QObject::connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));

/********************** add widget layout***********************************/
    QHBoxLayout  *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(checkBox1);
    leftLayout->addWidget(checkBox2);
    leftLayout->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(searchButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);
    this->setLayout(mainLayout);


/*************************************************************************/
    this->setWindowTitle("Find");
    this->setFixedHeight(sizeHint().height());
    this->setMinimumWidth(sizeHint().width());

    closeButton->installEventFilter(this);
}


FindDialog::~FindDialog(){}


void FindDialog::slotEnableFindButton(const QString & str)
{
    searchButton->setEnabled(!str.isEmpty());
}

void FindDialog::slotSearchButtonClicked()
{
    QString textContent = lineEdit->text();
//    if(textContent.isEmpty())
//        QMessageBox::warning(this,"Warning",\
                             "you have enter no text",QMessageBox::Ok);

    Qt::CaseSensitivity cs = checkBox1->isChecked() ? Qt::CaseSensitive\
                                                    :Qt::CaseInsensitive;

    if(checkBox2->isChecked())
    {
        emit findPrevious(textContent,cs);
    }
    else
    {
        emit findNext(textContent,cs);
    }
}

void FindDialog::slotFindContent(QString,Qt::CaseSensitivity)
{

}

void FindDialog::slotCloseButtonClicked()
{
    QWidget::close();
}

bool FindDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == closeButton && event->type() ==  QEvent::MouseButtonPress)
    {
        return true;
    }


    return QObject::eventFilter(watched,event);
}
