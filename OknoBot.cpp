#include "OknoBot.h"
#include "ui_OknoBot.h"

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OknoBot)
{
    ui->setupUi(this);
}

OknoBot::~OknoBot()
{
    delete ui;
}
