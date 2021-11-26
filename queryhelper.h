#ifndef QUERYHELPER_H
#define QUERYHELPER_H

#include <QJsonArray>
#include <QMap>
class MyDialog;

class QJsonDocument;
class QTableWidget;

class queryHelper
{
public:
    enum
    {
        Administrator,
        Passenger,
        TicketSeller,
        NotFound,
        Sold,
        Cancelled
    };

    queryHelper(MyDialog *);
    void insertPerson(const QJsonObject &);
    void insertFlight(const QJsonObject &);
    int doSearchPersons(const QString &, const QString &);
    void doDeleteFlight(const QString &);
    void sortItems();
    bool isValidInd(const QString &_)  { return !_ind_map[_]; }
    bool isValidId(const QString &_)   { return !_pid_map[_]; }
    bool isValidName(const QString &_) { return !_pnm_map[_]; }
    bool dealWithTicket(long long, const QString &);
    QTableWidget *_generateWidget(const QString &_filter = "");
    ~queryHelper();

private:
    MyDialog *_message_sender;
    QJsonArray _persons;
    QJsonArray _flight_info;
    QMap<QString, bool> _ind_map;
    QMap<QString, bool> _pid_map;
    QMap<QString, bool> _pnm_map;
    bool __sorted__ = false;
};

#endif // QUERYHELPER_H
