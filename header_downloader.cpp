#include "header_downloader.h"
#include "ui_header_downloader.h"
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
QString path = "undefined";

QByteArray getData_fromURL(QString url)
{
    QEventLoop event_loop;
    QNetworkAccessManager mgr;
    QObject::connect( &mgr, SIGNAL(finished(QNetworkReply*)), &event_loop, SLOT(quit()) );

    // send request and get data from URL
    QUrl tmp = QUrl(url);
    QNetworkRequest request(tmp);
    QNetworkReply *reply = mgr.get(request);
    event_loop.exec();

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()==200)
        return reply->readAll();
    else
        return QByteArray("file download failed");
}

header_downloader::header_downloader(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::header_downloader)
{
    ui->setupUi(this);
    qDebug()<<QSslSocket::sslLibraryBuildVersionString();
    //這裡是初始化的地方
    // int obj_x = ui->header_icon->width();
    // int obj_y = ui->header_icon->height();
    // ui->header_icon->setPixmap(QPixmap(":/icons/h2.png").scaled(obj_x,obj_y,Qt::KeepAspectRatio));
}

header_downloader::~header_downloader()
{
    delete ui;
}

void header_downloader::on_download_clicked()
{
    if (path == "undefined"){
        ui->status->setText("You haven't chosen the folder.");
        return;
    }
    QString header_type = ui->header_select->currentText();
    QString PATH = path + "\\" + header_type;
    QString URL = "https://raw.githubusercontent.com/ChocomintSSR/Header_forCpp/main/" + header_type;
    ui->status->setText("downloading...");

    // write data into file by QFile
    QFile *file = new QFile(PATH);
    QByteArray file_content = getData_fromURL(URL);

    file->open(QFile::WriteOnly);
    file->write(file_content);
    file->resize(file->pos()); //因為qt是overwrite 如果原先檔案比現在寫入的資料還要長 他就不會把後面的蓋掉 所以你要幫檔案整形到現在的位置
    file->close();

    ui->status->setText(""); // 就這樣
}

void header_downloader::on_pushButton_clicked() // browse...
{
    /*auto filePath = QFileDialog::getExistingDirectory(this, "Choose the folder");
    QDir d = QFileInfo(filePath).absoluteDir();
    QString _Folder = d.absolutePath();
    ui->path->setText(_Folder);*/

    path = QFileDialog::getExistingDirectory(this, "Choose a folder", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path->setText(path);
}

