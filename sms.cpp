#include <QtNetwork>
#include "sms.h"


QString twilio_phone_number = "+12515122192";


sms::sms(){

}
void sms::sendSMS(const QString &to, const QString &message) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://api.twilio.com/2010-04-01/Accounts/AC16875fd749b515d11a5daf6fc90a46b4/Messages.json");
    QNetworkRequest request(url);

    QString credentials = "AC16875fd749b515d11a5daf6fc90a46b4:3403711b59253e30cda89c50019f5237";
    QByteArray base64Credentials = credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + base64Credentials);


    QByteArray postData;
    postData.append("From="+twilio_phone_number);
    postData.append("&To=" + to);
    postData.append("&Body=" + message);


    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Make the request
    QNetworkReply *reply = manager->post(request, postData);


    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SMS sent successfully";
        } else {
            qDebug() << "Failed to send SMS:" << reply->errorString();
        }


        reply->deleteLater();
        manager->deleteLater();
    });
}
