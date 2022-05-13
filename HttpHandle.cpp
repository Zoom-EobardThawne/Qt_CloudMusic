#include "HttpHandle.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

HttpHandle::HttpHandle(QObject *parent) : QObject(parent),
    netWorkAccessMangager(new QNetworkAccessManager(this)), //网络访问管理器
    jsonAnalyze (new JsonAnalyze(this))  //json解析类
{
    connect(jsonAnalyze, SIGNAL(signalShowMusicInfo(QStringList)), this, SLOT(slotGetMusicInfo(QStringList)));
}

QByteArray HttpHandle::search(QString body)
{
    QNetworkRequest netWorkRequest(QUrl("https://l-by.cn/yinyue/api.php"));
    netWorkRequest.setRawHeader("Content-type","application/x-www-form-urlencoded; charset=UTF-8");

    QEventLoop eventLoop; //事件循环
    QNetworkReply *networkReply = netWorkAccessMangager->post(netWorkRequest,body.toUtf8());
    connect(networkReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents); //不处理用户输入操作，但操作会被保留，下次循环加载进去

    return networkReply->readAll();
}

void HttpHandle::searchMusicList(QString searchData)
{
    QString body = QString("types=search&count=100&source=netease&pages=1&name=%1").arg(searchData);
    QByteArray data = search(body);
    jsonAnalyze->getMusicList(data);
}

QString HttpHandle::searchMusicUrl(QString id)
{
    QString body = QString("types=url&id=%1&source=netease").arg(id);
    QByteArray data = search(body);

    return jsonAnalyze->getUrl(data);
}

QString HttpHandle::searchMusicLyric(QString id)
{
    QString body = QString("types=lyric&id=%1&source=netease").arg(id);
    QByteArray data = search(body);

    return jsonAnalyze->getLyric(data);
}

QString HttpHandle::searchPicUrl(QString picId)
{
    QString body = QString("types=pic&id=%1&source=netease").arg(picId);
    QByteArray data = search(body);

    return jsonAnalyze->getUrl(data);
}

QImage HttpHandle::getMusicPic(QString picUrl)
{
    QNetworkRequest networkRequest(picUrl);

    QEventLoop eventLoop;
    QNetworkReply *networkReply = netWorkAccessMangager->get(networkRequest);
    connect(networkReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    QByteArray imageData = networkReply->readAll();
    return QImage::fromData(imageData);
}

void HttpHandle::slotGetMusicInfo(QStringList musicInfo)
{
    emit signalMusicInfo(musicInfo);
}
