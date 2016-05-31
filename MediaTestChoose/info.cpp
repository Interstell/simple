#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

Info::~Info()
{
    delete ui;
}
