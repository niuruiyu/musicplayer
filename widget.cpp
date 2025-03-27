#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<QFileDialog>//文件对话框
#include<QDir>
#include<QMediaPlayer>//播放音乐
#include<QAudioOutput>//控制音乐声音
#include<QUrl>
//过程
//先写界面，可视化
//获取音乐，播放音乐
//交互操作
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //如何播放音乐
    //先加载模块multimedia
    //new一个output对象
    audioOutput=new QAudioOutput(this);
    //创建一个mediaPlayer对象
    mediaPlayer=new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    //给播放器设置音乐
   // mediaPlayer->setSource(QUrl::fromLocalFile("D:/Users/Lenovo/source/qtproject/musicplayer/Resource/music/That-Girl.mp3"));
    //播放
   // mediaPlayer->play();
    //获取当前媒体的时长,通过信号关联获取
    connect(mediaPlayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
        ui->totalLable->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60,2,10,QChar('0')));
        ui->playCourseSlide->setRange(0,duration);
    });
    //获取当前播放时长
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 pos){
        ui->curLable->setText(QString("%1:%2").arg(pos/1000/60,2,10,QChar('0')).arg(pos/1000%60,2,10,QChar('0')));
        ui->playCourseSlide->setValue(pos);

    }
            );
    //拖动滑块，让音乐播放的进度改变
    connect(ui->playCourseSlide,&QSlider::sliderMoved,mediaPlayer,&QMediaPlayer::setPosition);
    //audioOutput->setVolume(0.5);




}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{//打开文件对话框，让用户选择音乐所在的目录
    //qInfo()<<"hello student";//测试，输出hellostudent
    auto path=QFileDialog::getExistingDirectory(this,"请选择音乐所在的目录","D:\\Users\\Lenovo\\source\\qtproject\\musicplayer\\Resource\\music");
    //根据这个路径获取里面的所有mp3文件，wav文件
    QDir dir(path);
    auto musicList=dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    //qInfo()<<musicList;
    //把音乐名字放到listWidget中展示
    ui->listWidget->addItems(musicList);
    //默认选中第一个音乐
    ui->listWidget->setCurrentRow(0);

    //把音乐完整路径保存起来
    for(auto file:musicList)
        playList.append(QUrl::fromLocalFile(path+"/"+file));
    //qInfo()<<playList;







}

//播放
void Widget::on_pushButton_4_clicked()
{
    if(playList.empty()){
        return;
    }
    switch(mediaPlayer->playbackState()){
    case QMediaPlayer::PlaybackState::StoppedState://停止状态
    {  //如果没有播放-》播放当前选中的音乐
        //获取选中的行号
        curPlayIndex=ui->listWidget->currentRow();
        //播放对应行号的音乐
        mediaPlayer->setSource(playList[curPlayIndex]);
        mediaPlayer->play();


        break;}
    case QMediaPlayer::PlaybackState::PlayingState://播放状态
    {
         //如果现在正在播放-》暂停音乐
        mediaPlayer->pause();
        break;}
    case QMediaPlayer::PlaybackState::PausedState://暂停状态
    {
        //如果现在是在暂停-》继续播放
        mediaPlayer->play();
        break;
    }
    }
}

//上一曲
void Widget::on_pushButton_3_clicked()

{
    if(curPlayIndex==0){
        curPlayIndex=playList.size()-1;
    }
    else{
        curPlayIndex--;
    }

    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}

//下一曲
void Widget::on_pushButton_5_clicked()
{

    // //ranglistWidget选中下一行
    // //若果越界
    // // if(curPlayIndex>=playList.size()-1){
    // //     curPlayIndex=0;
    // // }
    // // else{//没有越界的话
    // // curPlayIndex++;
    // //  }
    //更简单的代码
    curPlayIndex=(curPlayIndex+1)%playList.size();
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}


//双击选择音乐
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayIndex=index.row();
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();

}

