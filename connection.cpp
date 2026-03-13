#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
db = QSqlDatabase::addDatabase("QODBC");
 db.setDatabaseName("test-bd");
db.setUserName("nedhir");//inserer nom de l'utilisateur
db.setPassword("nedhir");//inserer mot de passe de cet utilisateur


if (db.open())
test=true;

    return  test;
}
//void Connection::closeConnection(){ db.close;}
