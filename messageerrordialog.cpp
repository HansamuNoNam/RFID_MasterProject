#include "messageerrordialog.h"
#include "ui_messageerrordialog.h"
#include <QtGui>
#include <Qt>
MessageErrorDialog::MessageErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageErrorDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Error!!!");
    this->setWindowIcon(QIcon(":/Rec/images/iconfinder_shield-warning_299112.png"));
    this->setWindowModality(Qt::WindowModal);
}

MessageErrorDialog::~MessageErrorDialog()
{
    delete ui;
}
void MessageErrorDialog:: messageShow(const QString &s)
{
    ui->messageLabel->setText(tr("%1").arg(s));
    this->show();
}
