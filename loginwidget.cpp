#include <QtWidgets>
#include "mydialog.h"
#include "widgetsfwd.h"

loginWidget::loginWidget(QWidget *parent)
    : QWidget(parent)
{
    // Set-ups:
    setFixedSize(800, 560);
    setWindowFlag(Qt::FramelessWindowHint);
    setObjectName("main-widget");

    message_sender = new MyDialog;
    _query_helper  = new queryHelper(message_sender);
    _window_hint   = new TitleBar("地府航空", this);
    _title         = new QLabel("地府航空欢迎您！", this);
    _uname         = new QLabel("用户名:", this);
    _uname->setFixedWidth(120);
    _pwd           = new QLabel("密码:  ", this);
    _pwd->setFixedWidth(120);

    _title->setAlignment(Qt::AlignCenter);
    _title->setObjectName("login-title");
    _title->setFixedHeight(120);
    _uname->setAlignment(Qt::AlignCenter);
    _pwd->setAlignment(Qt::AlignCenter);

    _uname_edit = new QLineEdit(this);
    _pwd_edit   = new QLineEdit(this);

    _sign_up    = new QToolButton(this);
    _forgot_pwd = new QToolButton(this);
    _login      = new QPushButton(this);

    _sign_up->setText("注册账号");
    _sign_up->setObjectName("utils-btn");
    _sign_up->setFixedWidth(120);
    _forgot_pwd->setText("忘记密码");
    _forgot_pwd->setObjectName("utils-btn");
    _forgot_pwd->setFixedHeight(70);
    _forgot_pwd->setFixedWidth(120);
    _login->setText("登录");
    _login->setObjectName("login-btn");
    _login->setFixedHeight(70);
    _login->setShortcut(QKeySequence(tr("Enter")));

    _uname_edit->setFixedWidth(390);
    _pwd_edit->setFixedWidth(390);

    _uname_edit->setPlaceholderText("请输入用户名/身份证号");
    _uname_edit->setFixedHeight(40);
    _uname_edit->setMaxLength($MaxLength);
    _pwd_edit->setPlaceholderText("请输入密码");
    _pwd_edit->setFixedHeight(40);
    _pwd_edit->setEchoMode(QLineEdit::Password);
    _pwd_edit->setMaxLength($MaxLength);

    // Init-layouts:
    _main_layout = new QVBoxLayout(this);
    _grid = new QGridLayout;
    _grid->setColumnStretch(0, 3);
    _grid->setColumnStretch(1, 4);
    _grid->setColumnStretch(2, 3);
    _grid->setRowMinimumHeight(0, 90);
    _grid->setRowMinimumHeight(1, 90);
    _grid->setRowMinimumHeight(2, 90);
    _grid->setMargin(15);
    _grid->addWidget(_uname, 0, 0);
    _grid->addWidget(_uname_edit, 0, 1);
    _grid->addWidget(_forgot_pwd, 0, 2);
    _grid->addWidget(_pwd, 1, 0);
    _grid->addWidget(_pwd_edit, 1, 1);
    _grid->addWidget(_sign_up, 1, 2);
    _grid->addWidget(_login, 2, 1);

    _main_layout->setMargin(0);
    _main_layout->addWidget(_window_hint);
    _main_layout->addWidget(_title);
    _main_layout->addLayout(_grid);
    _main_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    connect(_uname_edit, &QLineEdit::returnPressed, this, &loginWidget::onLogInClicked);
    connect(_pwd_edit, &QLineEdit::returnPressed, this, &loginWidget::onLogInClicked);
    connect(_sign_up, &QAbstractButton::clicked, this, &loginWidget::onSignUpClicked);
    connect(_login, &QAbstractButton::clicked, this, &loginWidget::onLogInClicked);
}

void loginWidget::clearLineEdit()
{
    _uname_edit->clear();
    _pwd_edit->clear();
}

void loginWidget::onSignUpClicked()
{
    this->hide();
    signUp *_signUp = new signUp(_query_helper, this);
    _signUp->show();
}

void loginWidget::onLogInClicked()
{
    if (!_uname_edit->text().isEmpty() && _pwd_edit->text().length() >= 6)
    {
        if (auto _search_res = _query_helper->doSearchPersons(_uname_edit->text(),
                                               _pwd_edit->text());
            _search_res == queryHelper::Administrator)
        {
            auto _admin = new adminWidget(this);
            hide();
            _admin->show();
        }
        else if (_search_res == queryHelper::Passenger)
        {
            auto _Qw = new queryWidget(_query_helper, this, false);
            _Qw->show();
            this->hide();
        }
        else if (_search_res == queryHelper::TicketSeller)
        {
            auto _Tsw = new ticketSellerWidget(_query_helper, this);
            _Tsw->show();
            this->hide();
        }
        else
        {
            message_sender->setWindowTitle("抱歉");
            message_sender->setText("用户名或密码输入错误！");
            message_sender->show();
        }
        _uname_edit->clear();
        _pwd_edit->clear();
    }
}

void loginWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p (this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

loginWidget::~loginWidget()
{
    delete _window_hint;
    delete _main_layout;
    delete _title;
    delete _uname;
    delete _pwd;
    delete _uname_edit;
    delete _pwd_edit;
    delete _forgot_pwd;
    delete _sign_up;
    delete _login;
    delete message_sender;
    delete _query_helper;
}

