#ifndef HTTPHANDLE_H
#define HTTPHANDLE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QImage>
#include "JsonAnalyze.h"


class HttpHandle : public QObject
{
    Q_OBJECT
public:
    explicit HttpHandle(QObject *parent = nullptr);
    QByteArray search(QString body);
    void searchMusicList(QString searchData);
    QString searchMusicUrl(QString id);
    QString searchMusicLyric(QString id);
    QString searchPicUrl(QString picId);
    QImage getMusicPic(QString picUrl);

signals:
    void signalMusicInfo(QStringList musicInfo);

public slots:
    void slotGetMusicInfo(QStringList musicInfo);

private:
    QNetworkAccessManager *netWorkAccessMangager;
    JsonAnalyze *jsonAnalyze;
};

#endif // HTTPHANDLE_H
