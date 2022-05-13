#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMap>

class MusicPlayer :  public QMediaPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);

    QString parsingLyric(QString musicLyric); //歌词解析

signals:
    
    void signalShowLyricLine(int line);

public slots:
    
    void slotGetMusicLyricLine(qint64 time);



private:
    QMap<qint64, int> musicTime;

};

#endif // MUSICPLAYER_H
