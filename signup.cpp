#include <QLabel>
#include <QFile>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QJsonObject>
#include "mydialog.h"
#include "widgetsfwd.h"
#include "titlebar.h"

signUp::signUp(queryHelper *_Qh, loginWidget *caller, QWidget *parent)
    : QWidget(parent), _query_helper {_Qh}, _caller{caller}
{
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(900, 570);

    _window_hint = new TitleBar("第一次来地府？", this);
    _uname       = new QLabel("昵称:      ", this);
    _Id          = new QLabel("身份证号:   ", this);
    _pwd         = new QLabel("密码:      ", this);
    _confirm_pwd = new QLabel("请确认密码: ", this);

    _uname_edit       = new QLineEdit(this);
    _Id_edit          = new QLineEdit(this);
    _pwd_edit         = new QLineEdit(this);
    _confirm_pwd_edit = new QLineEdit(this);

    _uname_edit->setFixedWidth(460);
    _Id_edit->setFixedWidth(460);
    _pwd_edit->setFixedWidth(460);
    _confirm_pwd_edit->setFixedWidth(460);

    _uname_edit->setPlaceholderText("您叫什么？");
    _Id_edit->setPlaceholderText("请输入您的身份证号");
    _pwd_edit->setPlaceholderText("请输入您的密码");
    _confirm_pwd_edit->setPlaceholderText("请确认您的密码");
    _pwd_edit->setEchoMode(QLineEdit::Password);
    _confirm_pwd_edit->setEchoMode(QLineEdit::Password);

    _uname_edit->setFixedHeight(40);
    _Id_edit->setFixedHeight(40);
    _pwd_edit->setFixedHeight(40);
    _confirm_pwd_edit->setFixedHeight(40);

    _confirmed = new QPushButton(this);
    _confirmed->setText("注册");
    _confirmed->setObjectName("login-btn");
    _confirmed->setFixedHeight(60);
    _confirmed->setFixedWidth(160);
    _zuruck    = new QPushButton(this);
    _zuruck->setText("返回");
    _zuruck->setObjectName("login-btn");
    _zuruck->setFixedHeight(60);
    _zuruck->setFixedWidth(160);

    _main_layout = new QVBoxLayout(this);
    _grid        = new QGridLayout;
    _grid->setColumnStretch(0, 1);
    _grid->setColumnStretch(1, 1);
    _grid->addWidget(_uname, 0, 0);
    _grid->addWidget(_uname_edit, 0, 1);
    _grid->addWidget(_Id, 1, 0);
    _grid->addWidget(_Id_edit, 1, 1);
    _grid->addWidget(_pwd, 2, 0);
    _grid->addWidget(_pwd_edit, 2, 1);
    _grid->addWidget(_confirm_pwd, 3, 0);
    _grid->addWidget(_confirm_pwd_edit, 3, 1);
    _grid->addWidget(_confirmed, 4, 0);
    _grid->addWidget(_zuruck, 4, 1);
    _grid->setRowMinimumHeight(0, 80);
    _grid->setRowMinimumHeight(1, 80);
    _grid->setRowMinimumHeight(2, 80);
    _grid->setRowMinimumHeight(3, 80);
    _grid->setRowMinimumHeight(4, 80);

    _main_layout->setMargin(0);
    _main_layout->addWidget(_window_hint);
    _main_layout->addLayout(_grid);

    _main_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    _main_layout->setStretch(0, 4);
    _main_layout->setStretch(1, 2);
    _main_layout->setStretch(2, 2);
    _main_layout->setStretch(3, 2);
    _main_layout->setStretch(4, 2);
    _main_layout->setStretch(5, 4);
    _main_layout->setStretch(6, 4);

    connect(_confirmed, &QAbstractButton::clicked, this, &signUp::onConfirmedClicked);
    connect(_zuruck, &QAbstractButton::clicked, this, &signUp::onZuruckClicked);
}

void signUp::onConfirmedClicked()
{
    auto isValid = [] (const QString &_s)
    {
        if (_s.length() < 18) return false;

        qint64 _count = 0;
        for (char _c: _s.toStdString())
        {
            _count++;
            if (!isdigit(_c) && _count != 18)
                return false;
            else if (_c == 'x' && _count == 18)
                return true;
        }
        return true;
    };

    if (!_uname_edit->text().isEmpty() &&
        isValid(_Id_edit->text()) &&
        _pwd_edit->text().length() >= 6 &&
        _pwd_edit->text() == _confirm_pwd_edit->text() &&
        _query_helper->isValidId(_Id_edit->text()) &&
        _query_helper->isValidName(_uname_edit->text()))
    {
        char
            *_uname_ = (char *)_uname_edit->text().toStdString().c_str(),
            *_Id_    = (char *)_Id_edit->text().toStdString().c_str(),
            *_pwd_   = (char *)_pwd_edit->text().toStdString().c_str();


        QJsonObject _pjson
        {
            {"Username", _uname_},
            {"Authentication", "Passenger"},
            {"Identity", _Id_},
            {"Password", _pwd_}
        };
        _caller->getQueryHelper()->insertPerson(_pjson);
        auto _ms = _caller->getMessageSender();
        _ms->setWindowTitle("注册成功！");
        _ms->setText("将会返回登录界面...");
        _ms->show();
        this->hide();
        QTime dieTime = QTime::currentTime().addMSecs(2000);
        while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        _ms->hide();
        _caller->show();
        delete this;
    }
    else
    {
        _caller->getMessageSender()->setWindowTitle("填写了不合法的信息！");
        _caller->getMessageSender()->setText("请重新填写！");
        _caller->getMessageSender()->show();
    }
}

void signUp::onZuruckClicked()
{
    _caller->show();
    delete this;
}

signUp::~signUp()
{
    delete _window_hint;
    delete _uname;
    delete _Id;
    delete _pwd,
    delete _confirm_pwd;
    delete _uname_edit;
    delete _Id_edit;
    delete _pwd_edit;
    delete _confirm_pwd_edit;
    delete _confirmed;
    delete _main_layout;
}
