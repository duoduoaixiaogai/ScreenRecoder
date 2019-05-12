#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// deprecated
//#include "controlthread.h"

#include "controlthreadex.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void startingRe();
    void pausingRe();
    void stopingRe();
public slots:
    // 更新剩余视频帧数量
    void updateReVFrsCo(const int reVFrsCo);
    void startRe();
    void pauseRe();
    void stopRe();
private:
    Ui::MainWindow *ui;

    // deprecated
    //ControlThread *mCtlThr;
    ControlThreadEx *mCtlThr;
    bool mStartingRe;
};

#endif // MAINWINDOW_H
