#ifndef JSONANALYZE_H
#define JSONANALYZE_H

#include <QObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class JsonAnalyze : public QObject
{
    Q_OBJECT
public:
    explicit JsonAnalyze(QObject *parent = nullptr);
    bool isJsonDocument(const QByteArray &data);
    void getMusicList(const QByteArray &data);
    QString getUrl(const QByteArray &data);
    QString getLyric(const QByteArray &data);

signals:
    void signalShowMusicInfo(QStringList musicInfo);
public slots:

private:
    QJsonDocument jsonDocument;
};

#endif // JSONANALYZE_H
