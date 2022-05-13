#ifndef MUSICINTERFACE_H
#define MUSICINTERFACE_H

#include <QMainWindow>
#include "HttpHandle.h"
#include "MusicPlayer.h"
#include "AboutInterface.h"

namespace Ui {
class MusicInterface;
}

class MusicInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicInterface(QWidget *parent = 0);
    ~MusicInterface();

    void initTableWidget();

    void initTextEdit();


signals:
    void signalVolumeSilent(int volume, int flag_volume);
private slots:

    bool eventFilter(QObject *watched, QEvent *event); //事件过滤

    void slotVolumeSilent(int volume, int flag_volume);

    void slotSelectLyricLine(int line);

    void slotReflushEndTime(qint64 msec);

    void slotReflushCurrentTime(qint64 msec);

    void slotReflushPlayOrder(QMediaPlayer::MediaStatus status);

    void slotReflushPlayIcon(QMediaPlayer::State state);

    void slotShowMusicInfo(QStringList musicInfo);

    void on_lineEdit_search_returnPressed();

    void on_pushButton_search_clicked();

    void on_tableWidget_onlineList_cellDoubleClicked(int row, int column);

    void on_pushButton_playPre_clicked();

    void on_pushButton_playNext_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_playOrder_clicked();

    void on_horizontalSlider_processBar_sliderMoved(int position);

    void on_pushButton_volume_clicked();

    void on_verticalSlider_volume_sliderMoved(int position);

    void on_action_triggered();

private:
    Ui::MusicInterface *ui;
    HttpHandle *httpHandle;
    MusicPlayer *musicPlayer;
    AboutInterface *aboutInterface;
    int flag;  //播放类型标志位 0:列表循环 1：随机播放 2：单曲循环
    int flag_volume; //音量标志位 0：播放 1：静音
    int lastVolume; //保存上一次的音量
};

#endif // MUSICINTERFACE_H
