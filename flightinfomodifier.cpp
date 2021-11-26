#include "flightinfomodifier.h"
#include <QtWidgets>
#include <QJsonObject>
#include "widgetsfwd.h"

flightInfoModifier::flightInfoModifier(queryHelper *_Qh, adminWidget *caller, QWidget *parent)
    : QWidget(parent), _query_helper {_Qh}, _caller {caller}
{
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(900, 720);

    _window_hint = new TitleBar("飞行信息修改助手", this);

    _ind      = new QLabel("班次:", this);
    _abfahrt  = new QLabel("起飞时间:", this);
    _von      = new QLabel("发站:", this);
    _bis      = new QLabel("到站:", this);
    _duration = new QLabel("飞行时间:", this);
    _capacity = new QLabel("载客量:", this);

    _ind_edit      = new QLineEdit(this);
    _abfahrt_edit  = new QLineEdit(this);
    _von_edit      = new QLineEdit(this);
    _bis_edit      = new QLineEdit(this);
    _duration_edit = new QLineEdit(this);
    _capacity_edit = new QLineEdit(this);

    _ind_edit->setPlaceholderText("格式 AB1234");
    _abfahrt_edit->setPlaceholderText("格式 HH:mm");
    _von_edit->setPlaceholderText("填写出发站");
    _bis_edit->setPlaceholderText("填写终点站");
    _duration_edit->setPlaceholderText("格式 HH:mm");
    _capacity_edit->setPlaceholderText("单位: 人");

    _ind_edit->setMaxLength(6);
    _abfahrt_edit->setMaxLength(5);
    _von_edit->setMaxLength(32);
    _bis_edit->setMaxLength(32);
    _duration_edit->setMaxLength(32);
    _capacity_edit->setMaxLength(3);

    _commit_change = new QToolButton(this);
    _commit_change->setText("提交飞行信息");
    _commit_change->setObjectName("login-btn");
    _zuruck = new QToolButton(this);
    _zuruck->setText("返回管理页面");
    _zuruck->setObjectName("login-btn");

    // init layouts:
    _main_layout = new QVBoxLayout(this);
    _grid        = new QGridLayout;
    _grid->addWidget(_ind, 0, 0);           _grid->addWidget(_ind_edit, 0, 1);
    _grid->addWidget(_abfahrt, 1, 0);       _grid->addWidget(_abfahrt_edit, 1, 1);
    _grid->addWidget(_von, 2, 0);           _grid->addWidget(_von_edit, 2, 1);
    _grid->addWidget(_bis, 3, 0);           _grid->addWidget(_bis_edit, 3, 1);
    _grid->addWidget(_duration, 4, 0);      _grid->addWidget(_duration_edit, 4, 1);
    _grid->addWidget(_capacity, 5, 0);      _grid->addWidget(_capacity_edit, 5, 1);
    _grid->addWidget(_commit_change, 6, 0); _grid->addWidget(_zuruck, 6, 1);
    _grid->setRowMinimumHeight(0, 80);
    _grid->setRowMinimumHeight(1, 80);
    _grid->setRowMinimumHeight(2, 80);
    _grid->setRowMinimumHeight(3, 80);
    _grid->setRowMinimumHeight(4, 80);
    _grid->setRowMinimumHeight(5, 80);
    _grid->setRowMinimumHeight(6, 80);

    _main_layout->setMargin(0);
    _main_layout->addWidget(_window_hint);
    _main_layout->addLayout(_grid);
    _main_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    _grid->setMargin(50);

    connect(_commit_change, &QAbstractButton::clicked, this, &flightInfoModifier::onCommitChangeClicked);
    connect(_zuruck, &QAbstractButton::clicked, this, &flightInfoModifier::onZuruckClicked);
}

void flightInfoModifier::onCommitChangeClicked()
{
    auto _Ind = _ind_edit->text();
    auto _Ab  = _abfahrt_edit->text();
    auto _Von = _von_edit->text();
    auto _Bis = _bis_edit->text();
    auto _Dur = _duration_edit->text();
    auto _Cap = _capacity_edit->text();

    // test index validity:
    auto testIndValidity = [] (const QString &_s)
    {
        if (_s.length() < 6) return false;
        if (!(_s[0].isLetter() && _s[1].isLetter()))
            return false;
        for (auto _it = _s.begin() + 3; _it != _s.end(); ++_it)
        {
            if (!_it->isDigit()) return false;
        }
        return true;
    };

    // test abfahrt validity:
    auto testAbValidity = [] (const QString &_s)
    {
        if (!_s.contains(':')) return false;
        auto li = _s.split(":");
        if (li.length() != 2) return false;
        auto H = li[0].toLongLong();
        auto m = li[1].toLongLong();
        if (H >= 0 && H < 24 &&
            m >= 0 && m < 60) return true;
        else return false;
    };

    // test duration validity:
    auto testDurValidity = [] (const QString &_s)
    {
        if (_s.contains(":"))
        {
            auto _li = _s.split(":");
            auto m = _li[1].toLongLong();
            if (m >= 0 && m < 60) return true;
        }
        else return false;
        return false;
    };

    // test capacity validity:
    auto testCapValidity = [] (const QString &_s)
    {
        for (auto _c: _s)
            if (!_c.isDigit()) return false;
        return true;
    };

    if (testIndValidity(_Ind) &&
        _query_helper->isValidInd(_Ind) &&
        testAbValidity(_Ab) &&
        !_Von.isEmpty() && !_Bis.isEmpty() &&
        testDurValidity(_Dur) && testCapValidity(_Cap))
    {
        if (_Ab.length() == 4) _Ab = "0" + _Ab;

        QJsonObject _flight
        {
            {"Index", _Ind},
            {"Abfahrt", _Ab},
            {"Von", _Von},
            {"Bis", _Bis},
            {"Duration", _Dur},
            {"Capacity", _Cap},
            {"Remainder", _Cap}
        };
        _caller->getLoginWidget()->getQueryHelper()->insertFlight(_flight);
        auto _ms = _caller->getLoginWidget()->getMessageSender();
        _ms->setWindowTitle("Success!");
        _ms->setText("飞行信息提交成功!");
        _ms->show();
        _ind_edit->clear();
        _abfahrt_edit->clear();
        _von_edit->clear();
        _bis_edit->clear();
        _duration_edit->clear();
        _capacity_edit->clear();
    }
    else
    {
        auto _ms = _caller->getLoginWidget()->getMessageSender();
        _ms->setWindowTitle("Failure!");
        _ms->setText("输入错误的信息!");
        _ms->show();
    }
}

void flightInfoModifier::onZuruckClicked()
{
    _caller->show();
    delete this;
}

flightInfoModifier::~flightInfoModifier()
{
    delete _window_hint;
    delete _ind;
    delete _abfahrt;
    delete _von;
    delete _bis;
    delete _duration;
    delete _capacity;
    delete _ind_edit;
    delete _abfahrt_edit;
    delete _von_edit;
    delete _bis_edit;
    delete _duration_edit;
    delete _capacity_edit;
    delete _commit_change;
    delete _zuruck;
    delete _main_layout;
}
