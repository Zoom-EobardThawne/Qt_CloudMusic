#include "MusicPlayer.h"
#include <QDebug>
#include <QTime>

MusicPlayer::MusicPlayer(QObject *parent) : QMediaPlayer(parent)
{
    setVolume(50);
    setNotifyInterval(200); //修改通知信号发送间隔时间
}

QString MusicPlayer::parsingLyric(QString musicLyric)
{
    QStringList lyricList = musicLyric.split('\n');
    QString newLyric;
    musicTime.clear();
    int line = 0;

    /*解析歌词*/
    for (int i = 0; i < lyricList.size(); ++i) {
        QStringList lyricInfo = lyricList[i].split(']');
        if (lyricInfo.size() != 2) {
            continue;
        }
        newLyric += lyricInfo[1].trimmed() + "\r\n"; //trimmed() 去除首尾空格

        /*解析歌词时间获取当前播放的行号*/
        QString strTime = lyricInfo[0].remove('[');
        if (strTime.size() == 8) {
            strTime += '0'; //有的歌曲时间最后不是ms，是百分秒 // mm:ss.zzz
        }
        QTime time = QTime::fromString(strTime, "mm:ss.zzz");
        qint64 msec = time.minute() * 60000 +
                time.second() * 1000 +
                time.msec();

        musicTime[msec] = line++;
    }
    return newLyric;
}

void MusicPlayer::slotGetMusicLyricLine(qint64 time)
{
    if (musicTime.empty()) { //没有歌词直接返回
        return;
    }
    QMap<qint64,int>::iterator it = musicTime.end() -1;
    int endLine = it.value();
    it = musicTime.begin();
    int startLine = it.value();

    it = musicTime.upperBound(time);
    /*判断迭代器是否需要自减*/
    if (it != musicTime.begin()) {
        --it;
    }
    /*让当前选中歌词居中*/
    if (it.value() - 3 >= startLine) {
        emit signalShowLyricLine(it.value() - 3);
    }
    if (it.value() + 3 <= endLine) {
        emit signalShowLyricLine(it.value() + 3);
    }

    emit signalShowLyricLine(it.value()); //把当前行号发给主窗口
}
