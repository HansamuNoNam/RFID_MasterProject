#ifndef MESSAGEERRORDIALOG_H
#define MESSAGEERRORDIALOG_H

#include <QDialog>

namespace Ui {
class MessageErrorDialog;
}

class MessageErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageErrorDialog(QWidget *parent = nullptr);
    ~MessageErrorDialog();
public slots:
    void messageShow(const QString &s);
private:
    Ui::MessageErrorDialog *ui;
};

#endif // MESSAGEERRORDIALOG_H
