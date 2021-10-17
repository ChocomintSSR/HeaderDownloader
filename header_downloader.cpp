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
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonArray>
QString path = "undefined";
QJsonObject current_headers_data;

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
    // initialize here //
    ui->setupUi(this);
    //當你要處理ui物件 就必須在setupUi之後處理 否則Qt會崩潰

//    int obj_x = ui->banner->width();
//    int obj_y = ui->banner->height();
//    ui->banner->setPixmap(QPixmap(":/icons/background.png").scaled(obj_x,obj_y));

    QByteArray file_content = getData_fromURL("https://raw.githubusercontent.com/ChocomintSSR/HeaderDownloader/master/sys/file_index");
    if (file_content == "file download failed")
    {
        QMessageBox::warning(this,"Header Downloader - Error!","Application start failed\n[program can't access internet]");
        exit(0); //停止所有Qt的東西 並且在main.cpp內回傳0 關閉程式
    }
    else
    {
        current_headers_data = QJsonDocument::fromJson(file_content).object();
        QJsonArray headers_data = current_headers_data.value("list_of_headers").toArray();
        for (int i = 0; i < headers_data.size(); i++)
            ui->header_select->addItem(headers_data.at(i).toObject().value("name").toString());
    }

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
    if (path == "undefined" || path == "\0")
    {
        ui->status->setText("You haven't chosen the folder.");
        return;
    }

    QString header_type = ui->header_select->currentText();
    QString PATH = path + "\\" + header_type;
    QString URL = current_headers_data.value("root_url").toString()+ header_type;
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

