#include "JsonAnalyze.h"
#include <QDebug>

JsonAnalyze::JsonAnalyze(QObject *parent) : QObject(parent)
{

}

bool JsonAnalyze::isJsonDocument(const QByteArray &data)
{
    QJsonParseError error;
    jsonDocument = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "json解析失败,错误码:" << error.errorString();
        return false;
    }
    return true;
}

void JsonAnalyze::getMusicList(const QByteArray &data)
{
    if (!isJsonDocument(data)) {
        return;
    }
    QJsonArray jsonArray = jsonDocument.array();
    
    for (int i = 0; i < jsonArray.size(); ++i ) {
        QJsonObject musicObject = jsonArray[i].toObject();

        int id = musicObject["id"].toInt();
        QString strId = QString::number(id);
        QString picId = musicObject["pic_id"].toString();
        QString name = musicObject["name"].toString();
        QJsonArray artist = musicObject["artist"].toArray();
        QString strArtist;
        for (int j = 0; j < artist.size(); ++j) {
            QString art = artist[j].toString();
            strArtist = strArtist + art + " ";
        }
        strArtist.chop(1); //吞掉最后一个空格
        QString album = musicObject["album"].toString();
        QStringList musicInfo;
        musicInfo << strId << picId << name << strArtist << album;
        emit signalShowMusicInfo(musicInfo);
    }  
}

QString JsonAnalyze::getUrl(const QByteArray &data)
{
    if (!isJsonDocument(data)) {
        return QString();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject["url"].toString();
}

QString JsonAnalyze::getLyric(const QByteArray &data)
{
    if (!isJsonDocument(data)) {
        return QString();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject["lyric"].toString();

}
