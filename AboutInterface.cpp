#include "AboutInterface.h"
#include "ui_AboutInterface.h"

AboutInterface::AboutInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutInterface)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/ooopic_1520340986.ico"));
    setFixedSize(width(), height());
}

AboutInterface::~AboutInterface()
{
    delete ui;
}
