#include "widgetsfwd.h"
#include <QTableWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include <QHeaderView>
#include <algorithm>
#include <QDebug>

void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

queryHelper::queryHelper(MyDialog *_ms) : _message_sender {_ms}
{
    // read users:
    QFile _fusers {"./users.json"};
    if (_fusers.open(QIODevice::ReadOnly))
    {
        QByteArray _ba {_fusers.readAll()};
        _persons = QJsonDocument::fromJson(_ba).array();
        // init map:
        for (auto _ps: _persons)
        {
            auto _Obj = _ps.toObject();
            _pid_map[_Obj["Identity"].toString()] = true;
            _pnm_map[_Obj["Username"].toString()] = true;
        }
        _fusers.close();
    }
    // read flight info:
    QFile _fflight {"./flight-info.json"};
    if (_fflight.open(QIODevice::ReadOnly))
    {
        QByteArray _ba {_fflight.readAll()};
        _flight_info = QJsonDocument::fromJson(_ba).array();
        // init map:
        for (auto _ps: _flight_info)
        {
            auto _Obj = _ps.toObject();
            _ind_map[_Obj["Index"].toString()] = true;
        }
        _fflight.close();
    }
}

bool queryHelper::dealWithTicket(long long _pattern, const QString &_ind)
{
    bool is_success = false;
    if (_pattern == Sold)
    {
        bool _found = false;
        for (auto _fl: _flight_info)
        {
            auto _Obj = _fl.toObject();
            if (_Obj["Index"].toString() == _ind)
            {
                _found = true;
                auto _ab = QTime::fromString(_Obj["Abfahrt"].toString(), "HH:mm");
                auto _rem = _Obj["Remainder"].toString().toInt();
                auto _cur = QTime::currentTime();
                if (_rem > 0 && _cur.secsTo(_ab) > 7200)
                {
                    is_success = true;
                    _Obj["Remainder"] = QString::number(_rem - 1);
                    _fl = _Obj;
                }
                else if (_rem == 0)
                {
                    _message_sender->setWindowTitle("余量不足！");
                    _message_sender->setText("本班次人数已满！无法售票！");
                    _message_sender->show();
                }
                else if (_cur.secsTo(_ab) <= 7200)
                {
                    _message_sender->setWindowTitle("售票失败！");
                    _message_sender->setText("已经超过最后的售票时间！");
                    _message_sender->show();
                }
                break;
            }
        }
        // not found:
        if (!_found)
        {
            _message_sender->setWindowTitle("售票失败！");
            _message_sender->setText("没有找到相应的班次。");
            _message_sender->show();
        }
    }
    else if (_pattern == Cancelled)
    {
        bool _found = false;
        for (auto _fl: _flight_info)
        {
            auto _Obj = _fl.toObject();
            if (_Obj["Index"].toString() == _ind)
            {
                _found = true;
                auto _ab = QTime::fromString(_Obj["Abfahrt"].toString(), "HH:mm");
                auto _rem = _Obj["Remainder"].toString().toInt();
                auto _cap = _Obj["Capacity"].toString().toInt();
                auto _cur = QTime::currentTime();
                if (_rem < _cap && _cur.secsTo(_ab) > 7200)
                {
                    _Obj["Remainder"] = QString::number(_rem + 1);
                    _fl = _Obj;
                    is_success = true;
                }
                else if (_rem == _cap)
                {
                    _message_sender->setWindowTitle("退票失败！");
                    _message_sender->setText("这个班次没有人买票！怎么退？");
                    _message_sender->show();
                }
                else if (_cur.secsTo(_ab) <= 7200)
                {
                    _message_sender->setWindowTitle("退票失败！");
                    _message_sender->setText("已经超过最后的退票时间！");
                    _message_sender->show();
                }
                break;
            }
        }
        // not found:
        if (!_found)
        {
            _message_sender->setWindowTitle("退票失败！");
            _message_sender->setText("没有找到相应的班次。");
            _message_sender->show();
        }
    }
    return is_success;
}

int queryHelper::doSearchPersons(const QString &_uname, const QString &_pwd)
{
    for (auto _ps = _persons.begin(); _ps != _persons.end(); ++_ps)
    {
        auto _info = _ps->toObject();
        if ((_info["Username"] == _uname || _info["Identity"] == _uname)
                &&
             _info["Password"] == _pwd)
        {
            if (auto _auth = _info["Authentication"];
                     _auth == "Administrator")
                return Administrator;
            else if (_auth == "Passenger")
                return Passenger;
            else
                return TicketSeller;
        }
    }
    return NotFound;
}

void queryHelper::doDeleteFlight(const QString &_id)
{
    for (auto _f = _flight_info.begin(); _f != _flight_info.end(); ++_f)
    {
        auto _Obj = _f->toObject();
        if (_Obj["Index"].toString() == _id)
        {
            _flight_info.erase(_f);
            break;
        }
    }
}

void queryHelper::insertPerson(const QJsonObject &obj)
{
    _pid_map[obj["Identity"].toString()] = true;
    _pnm_map[obj["Userame"].toString()] = true;
    _persons.push_back(obj);
}

void queryHelper::sortItems()
{
    for (auto _fl: _flight_info)
    {
        auto _Obj = _fl.toObject();
        qDebug() << QTime::fromString(_Obj["Abfahrt"].toString(), "HH:mm");
    }
    // Sort by Abfahrt:
    if (!__sorted__) /* if not sorted: */
    {
        std::sort(_flight_info.begin(), _flight_info.end(),
        [] (QJsonValue _o1, QJsonValue _o2)
        {
            return
                QTime::fromString(_o1.toObject()["Abfahrt"].toString(), "HH:mm")
                    <
                QTime::fromString(_o2.toObject()["Abfahrt"].toString(), "HH:mm");
        });
    }
    __sorted__ = true;
}

void queryHelper::insertFlight(const QJsonObject &obj)
{
    _ind_map[obj["Index"].toString()] = true;
    _flight_info.push_back(obj);
    __sorted__ = false;
}

QTableWidget *queryHelper::_generateWidget(const QString &_filter)
{
    sortItems();
    auto _twptr = new QTableWidget;
    _twptr->setColumnCount(7);
    _twptr->setColumnWidth(0, 130);
    _twptr->setColumnWidth(1, 120);
    _twptr->setColumnWidth(2, 220);
    _twptr->setColumnWidth(3, 220);
    _twptr->setColumnWidth(4, 120);
    _twptr->setColumnWidth(5, 140);
    _twptr->setColumnWidth(6, 140);
    _twptr->setSelectionBehavior(QTableWidget::SelectRows);
    _twptr->setEditTriggers(QTableWidget::NoEditTriggers);
    _twptr->setFocusPolicy(Qt::NoFocus);
    _twptr->setShowGrid(false);
    _twptr->setHorizontalHeaderLabels(
        QStringList() << "班次" << "出发" << "发站" << "到站" << "飞行时间" << "载量" << "余量");
    _twptr->horizontalHeader()->setDisabled(true);
    _twptr->verticalHeader()->setDisabled(true);

    for(auto items: _flight_info)
    {
        auto _Obj = items.toObject();
        auto _Ind = _Obj["Index"].toString();
        auto _Ab  = _Obj["Abfahrt"].toString();
        auto _Von = _Obj["Von"].toString();
        auto _Bis = _Obj["Bis"].toString();
        auto _Dur = _Obj["Duration"].toString();
        auto _Cap = _Obj["Capacity"].toString();
        auto _Rem = _Obj["Remainder"].toString();
        if (_filter == ""          ||
            _Ind.contains(_filter) ||
            _Ab.contains(_filter)  ||
            _Von.contains(_filter) ||
            _Bis.contains(_filter))
        {
            auto _count = _twptr->rowCount();
            _twptr->insertRow(_count);
            QTableWidgetItem
                *_ind, *_ab, *_von, *_bis, *_dur, *_cap, *_rem;
            _ind = new QTableWidgetItem(_Ind);
            bool _hasAbfahrt = QTime::fromString(_Ab) <= QTime::currentTime();
            _ab  = new QTableWidgetItem(
                _hasAbfahrt ? "已出发" : _Ab);
            _von = new QTableWidgetItem(_Von);
            _bis = new QTableWidgetItem(_Bis);
            _dur = new QTableWidgetItem(_Dur);
            _cap = new QTableWidgetItem(_Cap);
            _rem = new QTableWidgetItem(_Rem);
            _twptr->setItem(_count, 0, _ind);
            _twptr->setItem(_count, 1, _ab);
            _twptr->setItem(_count, 2, _von);
            _twptr->setItem(_count, 3, _bis);
            _twptr->setItem(_count, 4, _dur);
            _twptr->setItem(_count, 5, _cap);
            _twptr->setItem(_count, 6, _rem);
        }
    }
    return _twptr;
}

queryHelper::~queryHelper()
{
    // write users:
    QFile _fusers {"./users.json"};
    if (_fusers.open(QIODevice::WriteOnly))
    {
        QJsonDocument _doc {_persons};
        _fusers.write(_doc.toJson());
        _fusers.close();
    }
    // write flight-info:
    QFile _fflight {"./flight-info.json"};
    if (_fflight.open(QIODevice::WriteOnly))
    {
        QJsonDocument _doc {_flight_info};
        _fflight.write(_doc.toJson());
        _fflight.close();
    }
}
