#ifndef HEADER_DOWNLOADER_H
#define HEADER_DOWNLOADER_H

#include <QMainWindow>

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

// initial
QT_BEGIN_NAMESPACE
namespace Ui { class header_downloader; }
QT_END_NAMESPACE

class header_downloader : public QMainWindow
{
    Q_OBJECT

public:
    header_downloader(QWidget *parent = nullptr);
    ~header_downloader();

private slots:
    void on_download_clicked();

    void on_pushButton_clicked();

private:
    Ui::header_downloader *ui;
};



#endif // HEADER_DOWNLOADER_H
