#include "ColonoscopeSensor.h"

ColonoscopeSensor::ColonoscopeSensor(QList<QString> serials, QObject *parent) : QObject(parent)
{
    ftdiSerials = serials;
    timer = new QTimer(this);
    parent->connect(timer, SIGNAL(timeout()), this, SLOT(updateValues()));
    timer->setInterval(CS_TIMER_INTERVAL);
    timer->setSingleShot(false);
    enable(false);
}

ColonoscopeSensor::~ColonoscopeSensor()
{
    timer->stop();
    FT_Close(uart);
    delete timer;
}

void ColonoscopeSensor::enable(bool enable)
{
    timer->stop();
    fresh = false;
    resetCount = 0;
    rxCount = 0;
    for(int i=0; i<CS_NUM_ELEMENTS; i++)
        values[i] = -1;
    FT_Close(uart);
    uart = NULL;
    if(enable) {
        enabled = true;
        status = CS_NO_USB_INTERFACE;
        timer->start();
    }
    else {
        enabled = false;
        status = CS_DISABLED;
    }
}

void ColonoscopeSensor::getValues(int val[CS_NUM_ELEMENTS])
{    
    fresh = false;
    for(int i=0; i<CS_NUM_ELEMENTS; i++){
        if (status == CS_ACTIVE)
            val[i] = values[CS_NUM_ELEMENTS-1-i]; // Flip the order of the elements, first is the tip
        else
            val[i] = 0;
    }
}

void ColonoscopeSensor::updateValues()
{
    if(status == CS_DISABLED) {
        enable(false);
        return;
    }

    resetCount += CS_TIMER_INTERVAL;

    if(status == CS_NO_USB_INTERFACE) {
        if(resetCount < CS_CONNECT_ATTEMPT_INTERVAL) return;
        resetCount = 0;
        for (int i = 0; i < ftdiSerials.count(); i++) {
            if(FT_OpenEx((PVOID)ftdiSerials[i].toStdString().c_str(), FT_OPEN_BY_SERIAL_NUMBER, &uart) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetLatencyTimer(uart, 2) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetBaudRate(uart, 115200) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetDataCharacteristics(uart, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetFlowControl(uart, FT_FLOW_NONE, 0, 0) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetTimeouts(uart, 2, 2) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetUSBParameters(uart, 64, 64) != FT_OK) { FT_Close(uart); continue; }
            if(FT_SetRts(uart) != FT_OK) { FT_Close(uart); continue; }
            if(FT_Purge(uart, FT_PURGE_RX | FT_PURGE_TX) != FT_OK) { FT_Close(uart); continue; }
            status = CS_NO_SENSOR;
            rxCount = 0;
            break;
        }
        return;
    }

    if(((status == CS_NO_SENSOR) && (resetCount > CS_INIT_COMM_TEST_DURATION)) ||
       ((status == CS_ACTIVE) && (resetCount > CS_NO_COMM_RESET_DURATION))){
        resetCount = 0;
        status = CS_NO_SENSOR;
        fresh = false;
        rxCount = 0;
        for(int i=0; i<CS_NUM_ELEMENTS; i++)
            values[i] = -1;
        if(FT_SetRts(uart) != FT_OK) {
            FT_Close(uart);
            uart = NULL;
            status = CS_NO_USB_INTERFACE;
            return;
        }
    }

    unsigned int bytes, read;
    if(FT_GetQueueStatus(uart, &bytes) != FT_OK) return;
    if(bytes == 0) return;
    unsigned char *buff = new unsigned char[bytes];
    if(FT_Read(uart, buff, bytes, &read) != FT_OK) { delete [] buff; return; }

    for(unsigned int i=0; i<read; i++){
        if(buff[i] == 0xFF) {
            if((status == CS_ACTIVE || (status == CS_NO_SENSOR && resetCount < CS_INIT_COMM_TEST_DURATION)) && rxCount == (1+2*CS_ELEM_PER_PCB)){
                unsigned int index = rxBuffer[0] - 1;
                if(index < CS_NUM_PCBS){
                    if (index == CS_NUM_PCBS-1) fresh = true;
                    for (int i=0; i<CS_ELEM_PER_PCB; i++)
                        values[index*CS_ELEM_PER_PCB + i] = (((unsigned int)rxBuffer[i*2+1] << 7) + (unsigned int)rxBuffer[i*2+2]);
                    resetCount = 0;
                    status = CS_ACTIVE;
                }
            }
            rxCount = 0;
        }
        else if(rxCount<25){
            rxBuffer[rxCount] = buff[i];
            rxCount++;
        }
    }
    delete [] buff;
}
