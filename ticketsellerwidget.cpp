#include "widgetsfwd.h"
#include <QtWidgets>

ticketSellerWidget::ticketSellerWidget(queryHelper *_Qh, QWidget *caller, QWidget *parent)
    : QWidget(parent), _query_helper {_Qh}, _caller {caller}
{
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(1090, 800);

    _window_hint = new TitleBar("售票系统", this);
    _table       = _query_helper->_generateWidget();
    _table->setFixedSize(1090, 600);
    _scrollable  = new QScrollArea(this);
    _scrollable->setFixedSize(1090, 600);
    _scrollable->setWidget(_table);
    _type_index  = new QLabel("班次:", this);
    _index_edit  = new QLineEdit(this);
    _index_edit->setPlaceholderText("输入要购买/退票的班次");
    _index_edit->setMaxLength(6);
    _sell        = new QToolButton(this);
    _sell->setText("售票");
    _sell->setObjectName("login-btn");
    _cancel      = new QToolButton(this);
    _cancel->setText("退票");
    _cancel->setObjectName("login-btn");
    _zuruck      = new QToolButton(this);
    _zuruck->setText("返回登录页面");
    _zuruck->setObjectName("login-btn");

    _ind_edit_layout = new QHBoxLayout;
    _ind_edit_layout->addWidget(_type_index);
    _ind_edit_layout->addWidget(_index_edit);
    _ind_edit_layout->addWidget(_sell);
    _ind_edit_layout->addWidget(_cancel);

    _functional_layout = new QVBoxLayout;
    _functional_layout->addLayout(_ind_edit_layout);
    _functional_layout->addWidget(_zuruck);

    _main_layout = new QVBoxLayout(this);
    _main_layout->addWidget(_window_hint);
    _main_layout->addWidget(_scrollable);
    _main_layout->addLayout(_functional_layout);
    _main_layout->setMargin(0);

    connect(_sell, &QAbstractButton::clicked, this, &ticketSellerWidget::onSellClicked);
    connect(_cancel, &QAbstractButton::clicked, this, &ticketSellerWidget::onCancelClicked);
    connect(_zuruck, &QAbstractButton::clicked, this, &ticketSellerWidget::onZuruckClicked);
}

void ticketSellerWidget::onSellClicked()
{
    if (!_index_edit->text().isEmpty())
    {
        if (_query_helper->dealWithTicket(queryHelper::Sold, _index_edit->text()))
        {
            _type_index->clear();
            delete _table;
            _table = _query_helper->_generateWidget();
            _table->setFixedSize(1090, 600);
            _scrollable->setWidget(_table);
        }
    }
}

void ticketSellerWidget::onCancelClicked()
{
    if (!_index_edit->text().isEmpty())
    {
        if (_query_helper->dealWithTicket(queryHelper::Cancelled, _index_edit->text()))
        {
            _type_index->clear();
            delete _table;
            _table = _query_helper->_generateWidget();
            _table->setFixedSize(1090, 600);
            _scrollable->setWidget(_table);
        }
    }
}

void ticketSellerWidget::onZuruckClicked()
{
    _caller->show();
    delete this;
}

ticketSellerWidget::~ticketSellerWidget()
{
    delete _window_hint;
    delete _type_index;
    delete _index_edit;
    delete _table;
    delete _scrollable;
    delete _sell;
    delete _cancel;
    delete _zuruck;
    delete _main_layout;
}
