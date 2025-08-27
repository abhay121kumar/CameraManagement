#ifndef RETRIEVEDIALOG_H
#define RETRIEVEDIALOG_H

#include <QDialog>
#include "mainwindow.h" // For Camera struct

namespace Ui {
class RetrieveDialog;
}

class RetrieveDialog : public QDialog {
    Q_OBJECT

public:
    explicit RetrieveDialog(const Camera &camera, QWidget *parent = nullptr);
    ~RetrieveDialog();

private:
    Ui::RetrieveDialog *ui;
};

#endif // RETRIEVEDIALOG_H
