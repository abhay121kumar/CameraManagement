#include "retrievedialog.h"
#include "ui_retrievedialog.h"

RetrieveDialog::RetrieveDialog(const Camera &camera, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RetrieveDialog)
{
    ui->setupUi(this);

    ui->lblId->setText("ID: " + QString::number(camera.id));
    ui->lblName->setText("Name: " + camera.name);
    ui->lblIp->setText("IP Address: " + camera.ipAddress);
    ui->lblStatus->setText("Status: " + camera.status);

    connect(ui->btnClose, &QPushButton::clicked, this, &RetrieveDialog::close);
}

RetrieveDialog::~RetrieveDialog() {
    delete ui;
}
