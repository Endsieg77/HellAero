#include "widgetsfwd.h"
#include <QtWidgets>

queryWidget::queryWidget(queryHelper *_Qh, QWidget *caller, bool _editable, QWidget *parent)
    : QWidget(parent), _query_helper{_Qh}, _caller {caller}, _editable {_editable}
{
    setFixedSize(1090, 820);
    setWindowFlag(Qt::FramelessWindowHint);

    _window_hint = new TitleBar("航班信息查询", this);

    _query_edit = new QLineEdit(this);
    _query_edit->setPlaceholderText("输入您想查询航班的信息");
    _query_edit->setFixedWidth(700);
    _query_edit->setMaxLength(40);

    _query = new QToolButton(this);
    _query->setText("搜索");
    _query->setObjectName("login-btn");

    _query_line = new QHBoxLayout;
    _query_line->addWidget(_query_edit);
    _query_line->addWidget(_query);

    _query_helper->sortItems();
    _table = _query_helper->_generateWidget();
    _table->setFixedSize(1090, 600);
    _scrollable = new QScrollArea(this);
    _scrollable->setFixedSize(1090, 600);
    _scrollable->setWidget(_table);

    _zuruck = new QToolButton(this);
    _zuruck->setText("返回上个页面");
    _zuruck->setObjectName("login-btn");

    _zuruck_line = new QHBoxLayout;
    _zuruck_line->addWidget(_zuruck);

    _main_layout = new QVBoxLayout(this);
    _main_layout->addWidget(_window_hint);
    _main_layout->addLayout(_query_line);
    _main_layout->addWidget(_scrollable);
    _main_layout->addLayout(_zuruck_line);
    _main_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    _main_layout->setMargin(0);

    _zuruck_line->setMargin(0);
    _zuruck_line->setSpacing(250);

    connect(_query_edit, &QLineEdit::returnPressed, this, &queryWidget::onQueryClicked);
    connect(_zuruck, &QAbstractButton::clicked, this, &queryWidget::onZuruckClicked);
    connect(_query, &QAbstractButton::clicked, this, &queryWidget::onQueryClicked);
    connect(_query_edit, &QLineEdit::textChanged, this, &queryWidget::onQueryClicked);
    if (_editable)
        connect(_table, &QTableWidget::cellDoubleClicked, this, &queryWidget::doDeleteItem);
}

void queryWidget::doDeleteItem(int _r, int _c)
{
    auto _id = _table->item(_r, 0)->text();
    _table->removeRow(_r);
    _query_helper->doDeleteFlight(_id);
}

void queryWidget::onQueryClicked()
{
    _query_helper->sortItems();
    delete _table;
    _table = _query_helper->_generateWidget(_query_edit->text());
    _table->setFixedSize(1090, 600);
    _scrollable->setWidget(_table);
    if (_editable)
        connect(_table, &QTableWidget::cellDoubleClicked, this, &queryWidget::doDeleteItem);
}

void queryWidget::onZuruckClicked()
{
    _caller->show();
    delete this;
}

queryWidget::~queryWidget()
{
    delete _window_hint;
    delete _scrollable;
    delete _query_edit;
    delete _query_line;
    delete _main_layout;
    delete _query;
    delete _zuruck;
}
