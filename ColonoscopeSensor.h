#ifndef COLONOSCOPESENSOR_H
#define COLONOSCOPESENSOR_H

#include <QTimer>
#include "ftd2xx.h"

#define CS_CONNECT_ATTEMPT_INTERVAL 500  // ms
#define CS_INIT_COMM_TEST_DURATION  500  // ms
#define CS_NO_COMM_RESET_DURATION   1000 // ms
#define CS_TIMER_INTERVAL           2    // ms
#define CS_ELEM_PER_PCB             8    // Number of sensor elements per PCB
#define CS_NUM_PCBS                 5    // Number of daisychained PCBs
#define CS_NUM_ELEMENTS             CS_ELEM_PER_PCB * CS_NUM_PCBS
#define CS_MAX_VALUE                16384

class ColonoscopeSensor : private QObject
{
    Q_OBJECT

public:
    enum SensorStatus { CS_DISABLED = -2, CS_NO_USB_INTERFACE = -1, CS_NO_SENSOR = 0, CS_ACTIVE = 1 };

    ColonoscopeSensor(QList<QString> serials, QObject *parent = 0);
    ~ColonoscopeSensor();
    void enable(bool enable);
    SensorStatus getStatus(){return status;}
    bool isFreshValues(){return fresh;}
    void getValues(int val[CS_NUM_ELEMENTS]); // Saves raw values in val

private slots:
    void updateValues();

private:
    bool enabled;
    bool fresh;
    QList<QString> ftdiSerials;
    FT_HANDLE uart;
    SensorStatus status;
    QTimer *timer;
    int resetCount;
    int rxCount;
    char rxBuffer[25];
    int values[CS_NUM_ELEMENTS];
};

#endif
