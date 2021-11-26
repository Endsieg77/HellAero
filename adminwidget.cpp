#include "widgetsfwd.h"
#include <QBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QTimer>

adminWidget::adminWidget(loginWidget *caller, QWidget *parent)
    : QWidget(parent), _caller {caller}
{
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(900, 700);

    _window_hint = new TitleBar("地府航空", this);
    _text        = new QLabel("你好，Administrator！\n"
                              "现在是" +
                               QDateTime::currentDateTime().toString(),
                               this);
    _enter_modification_GUI = new QToolButton(this);
    _data_query             = new QToolButton(this);
    _timer                  = new QTimer(this);
    _zuruck_login           = new QToolButton(this);

    _text->setAlignment(Qt::AlignCenter);
    _enter_modification_GUI->setText("修改班次信息");
    _data_query->setText("查询数据库");
    _enter_modification_GUI->setObjectName("login-btn");
    _data_query->setObjectName("login-btn");
    _zuruck_login->setText("返回登陆界面");
    _zuruck_login->setObjectName("login-btn");

    _timer->setInterval(1000);
    _timer->start();

    _ui_layout   = new QVBoxLayout(this);
    _btn_layout  = new QHBoxLayout;

    _btn_layout->addWidget(_enter_modification_GUI);
    _btn_layout->addWidget(_data_query);
    _btn_layout->addWidget(_zuruck_login);
    _btn_layout->setAlignment(Qt::AlignHCenter);

    _ui_layout->setMargin(0);
    _ui_layout->addWidget(_window_hint);
    _ui_layout->addWidget(_text);
    _ui_layout->addLayout(_btn_layout);
    _ui_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    _ui_layout->setStretch(0, 2);
    _ui_layout->setStretch(1, 2);
    _ui_layout->setStretch(2, 3);

    _btn_layout->setMargin(50);

    // build connections:
    connect(_timer, &QTimer::timeout, this, &adminWidget::updateText);
    connect(_enter_modification_GUI, &QAbstractButton::clicked, this, &adminWidget::enterModification);
    connect(_data_query, &QAbstractButton::clicked, this, &adminWidget::enterQuery);
    connect(_zuruck_login, &QAbstractButton::clicked, this, &adminWidget::zuruckLoginHelper);
}

void adminWidget::enterModification()
{
    flightInfoModifier *modification = new flightInfoModifier(_caller->getQueryHelper(), this);
    modification->show();
    hide();
}

void adminWidget::enterQuery()
{
    this->hide();
    auto _query_widget = new queryWidget(_caller->getQueryHelper(), this, true);
    _query_widget->show();
}

void adminWidget::zuruckLoginHelper()
{
    _caller->show();
    delete this;
}

void adminWidget::updateText()
{
    _text->setText("你好，Administrator！\n现在是" + QDateTime::currentDateTime().toString());
}

adminWidget::~adminWidget()
{
    delete _window_hint;
    delete _ui_layout;
    delete _text;
    delete _enter_modification_GUI;
    delete _zuruck_login;
    delete _timer;
}
