#include "MusicInterface.h"
#include "ui_MusicInterface.h"
#include <QDebug>
#include <QScrollBar>
#include <QTime>
#include <QTextBlock>
#include <QTimer>
#include <QMessageBox>

MusicInterface::MusicInterface(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MusicInterface),
    httpHandle(new HttpHandle(this)),
    musicPlayer(new MusicPlayer(this)),
    aboutInterface(new AboutInterface),
    flag(0),
    flag_volume(0),
    lastVolume(0)
{
    /*设置ui界面*/
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/ooopic_1520340986.ico"));
    setFixedSize(width(), height());
    ui->verticalSlider_volume->hide();

    
    /*生成随机数种子*/
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec() + time.second()*1000 + time.minute() * 60 * 1000);
    
    /*绑定信号与槽*/
    connect(httpHandle, SIGNAL(signalMusicInfo(QStringList)), this, SLOT(slotShowMusicInfo(QStringList)));

    connect(ui->verticalSlider_volume, SIGNAL(sliderMoved(int)), musicPlayer, SLOT(setVolume(int)));

    connect(musicPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slotReflushPlayIcon(QMediaPlayer::State)));

    connect(musicPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slotReflushPlayOrder(QMediaPlayer::MediaStatus)));

    connect(musicPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(slotReflushEndTime(qint64)));

    connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(slotReflushCurrentTime(qint64)));

    connect(musicPlayer, SIGNAL(positionChanged(qint64)), musicPlayer, SLOT(slotGetMusicLyricLine(qint64)));

    connect(musicPlayer, SIGNAL(signalShowLyricLine(int)), this, SLOT(slotSelectLyricLine(int)));

    connect(this, SIGNAL(signalVolumeSilent(int, int)), this, SLOT(slotVolumeSilent(int,int)));

    /*初始化函数*/
    initTableWidget(); //初始化播放列表

    initTextEdit(); //初始化歌词显示区

    /*显示按钮备注*/
    ui->pushButton_playPre->setToolTip("上一曲");
    ui->pushButton_play->setToolTip("播放/暂停");
    ui->pushButton_playNext->setToolTip("下一曲");
    ui->pushButton_playOrder->setToolTip("播放顺序");
    ui->pushButton_volume->setToolTip(QString::number(musicPlayer->volume())); //显示音量

    /*安装事件过滤*/
    ui->pushButton_volume->installEventFilter(this); //音量按钮监控
    ui->verticalSlider_volume->installEventFilter(this); //音量滑动条监控
}

MusicInterface::~MusicInterface()
{
    delete ui;
}

void MusicInterface::initTableWidget()
{
    //ui->tableWidget_onlineList->setStyleSheet("selection-background-color:rgba(255,255,255,50%); selection-color: white;"); //设置tableWidget QSS样式表，选中时背景为红色，字体为白色
    ui->tableWidget_onlineList->setStyleSheet("background-color:rgba(255,255,255,40%); color: black;"); //设置tableWidget QSS样式表
    ui->tableWidget_onlineList->hideColumn(0);	//隐藏第一列
    ui->tableWidget_onlineList->hideColumn(1);	//隐藏第二列
    ui->tableWidget_onlineList->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget_onlineList->setSelectionBehavior(QAbstractItemView::SelectRows);//一次选一行
    ui->tableWidget_onlineList->setFocusPolicy(Qt::NoFocus);	//去掉虚线框
    ui->tableWidget_onlineList->setSortingEnabled(true);	//增加表头排序功能

    ui->tableWidget_onlineList->horizontalHeader()->setFixedHeight(30);    //固定列头高度为30像素
    ui->tableWidget_onlineList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgba(170,170,255,40%);}");   //设置tableWidget列头QSS样式表
    ui->tableWidget_onlineList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表头拉伸
    ui->tableWidget_onlineList->horizontalHeader()->setHighlightSections(false);       //选中列头不在高亮

    ui->tableWidget_onlineList->verticalHeader()->hide();//隐藏行头
    //ui->tableWidget_onlineList->verticalHeader()->setHighlightSections(false);         //选中行头不在高亮


    ui->tableWidget_onlineList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"        //垂直滑块整体

                                                                   "background:rgba(255,255,255,10%);"  //背景色

                                                                   "padding-top:20px;"    //上预留位置（放置向上箭头）

                                                                   "padding-bottom:20px;" //下预留位置（放置向下箭头）

                                                                   "padding-left:3px;"    //左预留位置（美观）

                                                                   "padding-right:3px;"   //右预留位置（美观）

                                                                   "border-left:1px solid #d7d7d7;}"//左分割线

                                                                   "QScrollBar::handle:vertical{"//滑块样式

                                                                   "background:rgba(170,170,255,30%);"  //滑块颜色

                                                                   "border-radius:10px;"   //边角圆润

                                                                   "min-height:10px;}"    //滑块最小高度

                                                                   "QScrollBar::handle:vertical:hover{"//鼠标触及滑块样式

                                                                   "background:rgba(170,170,255,80%);}" //滑块颜色

                                                                   "QScrollBar::add-line:vertical{"//向下箭头样式

                                                                   "background:url(:/images/resource/images/checkout/down.png) center no-repeat;}"

                                                                   "QScrollBar::sub-line:vertical{"//向上箭头样式

                                                                   "background:url(:/images/resource/images/checkout/up.png) center no-repeat;}");

    ui->textEdit_lyric->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"        //垂直滑块整体

                                                           "background:rgba(255,255,255,10%);"  //背景色

                                                           "padding-top:20px;"    //上预留位置（放置向上箭头）

                                                           "padding-bottom:20px;" //下预留位置（放置向下箭头）

                                                           "padding-left:3px;"    //左预留位置（美观）

                                                           "padding-right:3px;"   //右预留位置（美观）

                                                           "border-left:1px solid #d7d7d7;}"//左分割线

                                                           "QScrollBar::handle:vertical{"//滑块样式

                                                           "background:rgba(170,170,255,30%);"  //滑块颜色

                                                           "border-radius:10px;"   //边角圆润

                                                           "min-height:10px;}"    //滑块最小高度

                                                           "QScrollBar::handle:vertical:hover{"//鼠标触及滑块样式

                                                           "background:rgba(170,170,255,80%);}" //滑块颜色

                                                           "QScrollBar::add-line:vertical{"//向下箭头样式

                                                           "background:url(:/images/resource/images/checkout/down.png) center no-repeat;}"

                                                           "QScrollBar::sub-line:vertical{"//向上箭头样式

                                                           "background:url(:/images/resource/images/checkout/up.png) center no-repeat;}");
}

void MusicInterface::initTextEdit()
{
    QPalette palette = ui->textEdit_lyric->palette();			//获取textEdit的调色板
    palette.setColor(QPalette::Highlight, QColor(Qt::transparent));	//将选中区域背景改为透明
    palette.setColor(QPalette::HighlightedText, QColor(Qt::red));	//将选中区域的文字改为红色
    ui->textEdit_lyric->setPalette(palette);				//将调色板设置到textEdit
    ui->textEdit_lyric->setReadOnly(true);				//设置为只读
    ui->textEdit_lyric->document()->setDefaultTextOption(QTextOption(Qt::AlignHCenter));	//文字居中显示
}

bool MusicInterface::eventFilter(QObject *watched, QEvent *event) //事件过滤
{
    if (watched == ui->pushButton_volume) {
        if (event->type() == QEvent::Enter) {
            ui->verticalSlider_volume->show();
        }
    }
    if (watched == ui->verticalSlider_volume) {
        if (event->type() == QEvent::Enter) {
            ui->verticalSlider_volume->show();
        }
        else if(event->type() == QEvent::Leave) {
            ui->verticalSlider_volume->hide();
        }
    }
    return false;
}

void MusicInterface::slotVolumeSilent(int volume, int flag_volume)
{
    if (flag_volume == 1) {
        lastVolume = volume;
        emit ui->verticalSlider_volume->sliderMoved(0);
        ui->verticalSlider_volume->setValue(0);
    }
    else {
        emit ui->verticalSlider_volume->sliderMoved(lastVolume);
        ui->verticalSlider_volume->setValue(lastVolume);
    }
}

void MusicInterface::slotSelectLyricLine(int line)
{
    QTextCursor tc2 = ui->textEdit_lyric->textCursor();				   //获取当前光标
    int pos2 = ui->textEdit_lyric->document()->findBlockByLineNumber(line).position(); //在textEdit中找到指定行位置
    tc2.setPosition(pos2, QTextCursor::MoveAnchor);			  //将光标设置到指定位置
    tc2.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);	  //光标设置成选中一整行，并且保持此状态
    ui->textEdit_lyric->setTextCursor(tc2);					  //将设置好的光标属性放到textEdit中
}

void MusicInterface::slotReflushEndTime(qint64 msec)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(msec);
    ui->label_endTime->setText(time.toString("mm:ss"));
    ui->horizontalSlider_processBar->setMaximum(msec);
}

void MusicInterface::slotReflushCurrentTime(qint64 msec)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(msec);
    ui->label_currentTime->setText(time.toString("mm:ss"));
    ui->horizontalSlider_processBar->setValue(msec);
}

void MusicInterface::slotReflushPlayOrder(QMediaPlayer::MediaStatus status)
{
    if ( status == QMediaPlayer::EndOfMedia) {  //等待歌曲播放结束
        /*判断播放顺序标志位*/
        if (flag == 0) {
            on_pushButton_playNext_clicked();
        }
        else if (flag == 1) {
            int row = qrand() % ui->tableWidget_onlineList->rowCount();
            ui->tableWidget_onlineList->selectRow(row);
            on_tableWidget_onlineList_cellDoubleClicked(row,0);
        }
        else if (flag == 2) {
            int row = ui->tableWidget_onlineList->currentRow();
            ui->tableWidget_onlineList->selectRow(row);
            on_tableWidget_onlineList_cellDoubleClicked(row,0);
        }
    }
    else if (status == QMediaPlayer::InvalidMedia) { //不可播放
        QMessageBox :: warning(this, tr("CatMusic"),
                               tr("版权音乐！\n"
                                  "无法播放！\n"
                                  "将播放下一首歌曲\n"),
                               QMessageBox::Ok);
        on_pushButton_playNext_clicked(); //播放下一首
    }
}

void MusicInterface::slotReflushPlayIcon(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->pushButton_play->setStyleSheet("border-image: url(:/image/icon/play.png)");
        //ui->graphicsView->diskItemStop();
        ui->graphicsView->startAnimation(); //播放状态，旋转唱片
    }
    else {
        ui->pushButton_play->setStyleSheet("border-image: url(:/image/icon/pase.png)");
        ui->graphicsView->pauseAnimation(); //暂停状态，暂停旋转
    }
}

void MusicInterface::slotShowMusicInfo(QStringList musicInfo)
{
    int row = ui->tableWidget_onlineList->rowCount();
    ui->tableWidget_onlineList->insertRow(row);

    for (int i = 0; i < musicInfo.size(); ++i) {
        ui->tableWidget_onlineList->setItem(row, i, new QTableWidgetItem(musicInfo[i]));
    }
}

void MusicInterface::on_lineEdit_search_returnPressed()
{
    ui->tableWidget_onlineList->clearContents();
    ui->tableWidget_onlineList->setRowCount(0);
    QString searchData = ui->lineEdit_search->text();
    httpHandle->searchMusicList(searchData);
}

void MusicInterface::on_pushButton_search_clicked()
{
    ui->tableWidget_onlineList->clearContents();
    ui->tableWidget_onlineList->setRowCount(0);
    QString searchData = ui->lineEdit_search->text();
    httpHandle->searchMusicList(searchData);
}

void MusicInterface::on_tableWidget_onlineList_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QString musicId = ui->tableWidget_onlineList->item(row, 0)->text(); //第一列是音乐id
    QString musicPicId = ui->tableWidget_onlineList->item(row, 1)->text();//第二列是图片id

    QString musicUrl = httpHandle->searchMusicUrl(musicId);
    QString musicLyric = httpHandle->searchMusicLyric(musicId);
    QString musicPicUrl = httpHandle->searchPicUrl(musicPicId);
    musicPicUrl.replace("300y300", "310y310");
    QImage musicImage = httpHandle->getMusicPic(musicPicUrl);
    ui->graphicsView->setImage(musicImage);

    musicLyric = musicPlayer->parsingLyric(musicLyric);
    ui->textEdit_lyric->setText(musicLyric);

    musicPlayer->setMedia(QUrl(musicUrl));
    musicPlayer->play();

    ui->graphicsView->stopAnimation();
    ui->graphicsView->startAnimation();
}

void MusicInterface::on_pushButton_playPre_clicked()
{
    int row = ui->tableWidget_onlineList->currentRow();
    if (row == -1) {
        return;
    }
    if (row == 0) {
        row = ui->tableWidget_onlineList->rowCount();
    }
    --row;
    this->on_tableWidget_onlineList_cellDoubleClicked(row,0);
    ui->tableWidget_onlineList->selectRow(row);


    //动画效果
    ui->graphicsView->stopAnimation();
    ui->graphicsView->startAnimation(); //播放状态，旋转唱片
}

void MusicInterface::on_pushButton_playNext_clicked()
{
    int row = ui->tableWidget_onlineList->currentRow();
    if (row == -1) {
        return;
    }
    if (row == ui->tableWidget_onlineList->rowCount() - 1) {
        row = -1;
    }
    ++row;
    this->on_tableWidget_onlineList_cellDoubleClicked(row,0);
    ui->tableWidget_onlineList->selectRow(row);

    //动画效果
    ui->graphicsView->stopAnimation();
    ui->graphicsView->startAnimation(); //播放状态，旋转唱片
}

void MusicInterface::on_pushButton_play_clicked()
{
    if (musicPlayer->state() == QMediaPlayer::PlayingState) {
        musicPlayer->pause();
    }
    else if (musicPlayer->state() == QMediaPlayer::PausedState) {
        musicPlayer->play();
    }
    else {
        int row = ui->tableWidget_onlineList->currentRow(); //当前行
        if (row == -1) {
            return;
        }
        this->on_tableWidget_onlineList_cellDoubleClicked(row,0);
    }
}

void MusicInterface::on_pushButton_playOrder_clicked()
{
    if (flag == 0) {
        ui->pushButton_playOrder->setStyleSheet("border-image: url(:/image/icon/play-random.png)");
        flag++;
    }
    else if (flag == 1) {
        ui->pushButton_playOrder->setStyleSheet("border-image: url(:/image/icon/loop-one.png)");
        flag++;
    }
    else if (flag == 2) {
        ui->pushButton_playOrder->setStyleSheet("border-image: url(:/image/icon/loop-list.png)");
        flag = 0;
    }

}

void MusicInterface::on_horizontalSlider_processBar_sliderMoved(int position)
{
    musicPlayer->setPosition(position);
}

void MusicInterface::on_pushButton_volume_clicked()
{
    int currentVolume = musicPlayer->volume();

    if (flag_volume == 0) {
        ui->pushButton_volume->setStyleSheet("border-image: url(:/image/icon/none-music.png)");
        flag_volume++;
    }
    else if (flag_volume == 1) {
        ui->pushButton_volume->setStyleSheet("border-image: url(:/image/icon/music-voice.png)");
        flag_volume = 0;

    }
    emit signalVolumeSilent(currentVolume, flag_volume);
}

void MusicInterface::on_verticalSlider_volume_sliderMoved(int position)
{
    if (position == 0) {
        flag_volume = 1;
        ui->pushButton_volume->setStyleSheet("border-image: url(:/image/icon/none-music.png)");
    }
    else {
        ui->pushButton_volume->setStyleSheet("border-image: url(:/image/icon/music-voice.png)");
        flag_volume = 0;
    }

    ui->pushButton_volume->setToolTip(QString::number(position)); //鼠标悬停在音量按钮显示音量
}

void MusicInterface::on_action_triggered()
{
    aboutInterface->show();
}
