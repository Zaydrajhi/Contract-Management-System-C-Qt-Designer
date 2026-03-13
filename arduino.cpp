#include "arduino.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QObject>
QString Arduino::loginnn(QString ID_REF) {
    QSqlQuery query;

    // Prepare the SQL query
    query.prepare("SELECT ID FROM EMPLOYE WHERE ID = :ID_REF");
    query.bindValue(":ID_REF", ID_REF);

    // Execute the query
    if (query.exec()) {
        if (query.next()) {
            // ID exists
            qDebug() << "ID exists:" << ID_REF;
            return "1";
        } else {
            // ID does not exist
            qDebug() << "ID does not exist:" << ID_REF;
            return "0";
        }
    } else {
        // Handle errors
        qDebug() << "Error executing query:" << query.lastError().text();
        return "Error executing query";
    }
}

QString Arduino::login(QString code) {
    QSqlQuery query;

    // Prepare the SQL query
    query.prepare("SELECT code FROM EMPLOYE WHERE code = :code");
    query.bindValue(":code", code);

    // Execute the query
    if (query.exec()) {
        if (query.next()) {
            // ID exists
            qDebug() << "ID exists:" << code;
            return "1";
        } else {
            // ID does not exist
            qDebug() << "ID does not exist:" << code;
            return "0";
        }
    } else {
        // Handle errors
        qDebug() << "Error executing query:" << query.lastError().text();
        return "Error executing query";
    }
}


Arduino::Arduino()
{
    data="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
   return serial;
}
int Arduino::connect_arduino()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
    // est connectée
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
           if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
               if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                       == arduino_uno_producy_id) {
                   arduino_is_available = true;
                   arduino_port_name=serial_port_info.portName();
               } } }
        qDebug() << "arduino_port_name is :" << arduino_port_name;
        if(arduino_is_available){ // configuration de la communication ( débit...)
            serial->setPortName(arduino_port_name);
            if(serial->open(QSerialPort::ReadWrite)){
                serial->setBaudRate(QSerialPort::Baud9600); // débit : 9600 bits/s
                serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
                serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
                serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
                serial->setFlowControl(QSerialPort::NoFlowControl);
                return 0;
            }
            return 1;
        }
        return -1;
}

int Arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}


 QByteArray Arduino::read_from_arduino()
{
    if(serial->isReadable()){
         data=serial->readAll(); //récupérer les données reçues

         return data;
    }
 }

 QByteArray Arduino::read_from_arduinoo()
 {

      QByteArray concatenatedData; // Chaîne pour stocker les données concaténées
      if (serial->isReadable()) {
          while (serial->waitForReadyRead(100)) { // Attendre la disponibilité des données pendant 100 ms
              QByteArray data = serial->readAll(); // Récupérer les données reçues
              if (!data.isEmpty()) {
                  qDebug() << "Received data from Arduino:" << data;
                  // Nettoyer les données en supprimant les caractères indésirables ("\r", "\n" et " ")
                  data = data.simplified();
                  data.replace("\r", "").replace("\n", "").replace(" ", ""); // Supprimer les sauts de ligne et les espaces
                  // Concaténer les données nettoyées à la chaîne existante
                  concatenatedData.append(data);
                  qDebug() << "Concatenated data:" << concatenatedData;
              }
          }
      } else {
          qDebug() << "Serial port is not readable or not open.";
      }
      return concatenatedData;
  }

int Arduino::write_to_arduino( QByteArray d)

{

    if(serial->isWritable()){
        serial->write(d);  // envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
    }


}
QByteArray Arduino::getdata()
{
    return data;
}
