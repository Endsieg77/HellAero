#include "abstractwidget.h"
#include "titlebar.h"
#include <QBoxLayout>

abstractWidget::abstractWidget(QWidget *_son, QWidget *parent) : QWidget(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);
    _title_bar = new TitleBar("地府航空", this);
    _main_layout = new QVBoxLayout(this);
    _main_layout->addWidget(_title_bar);
    _main_layout->addWidget(_son);
}
