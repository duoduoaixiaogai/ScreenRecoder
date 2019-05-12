#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mCtlThr(nullptr), mStartingRe(false)
{
    av_register_all();
    avdevice_register_all();

    av_log_set_level(AV_LOG_DEBUG);

    ui->setupUi(this);
    ui->label->hide();
    ui->progressBar->hide();

    // deprecated
    //mCtlThr = new ControlThread(true, true, this);
    //mCtlThr = new ControlThread(false, true, this);
    //mCtlThr = new ControlThread(true, false, this);

    //mCtlThr = new ControlThreadEx(true, false, this);
    //mCtlThr = new ControlThreadEx(false, true, this);
    mCtlThr = new ControlThreadEx(true, true, this);
    // deprecated
    //connect(ui->startRecoder, &QPushButton::clicked, mCtlThr, &ControlThread::startedRecoder);
    //connect(ui->stopRecoder, &QPushButton::clicked, mCtlThr, &ControlThread::stopedRecoder);
    connect(ui->startRecoder, &QPushButton::clicked, this, &MainWindow::startRe);
    connect(ui->pauseRecoder, &QPushButton::clicked, this, &MainWindow::pauseRe);
    connect(ui->stopRecoder, &QPushButton::clicked, this, &MainWindow::stopRe);
    connect(this, &MainWindow::startingRe, mCtlThr, &ControlThreadEx::startRe);
    connect(this, &MainWindow::pausingRe, mCtlThr, &ControlThreadEx::pauseRe);
    connect(this, &MainWindow::stopingRe, mCtlThr, &ControlThreadEx::stopRe);
    //connect(qApp, &QGuiApplication::lastWindowClosed, mCtlThr, &ControlThreadEx::closeApp);
    connect(mCtlThr, &ControlThreadEx::transReVFrsCo, this, &MainWindow::updateReVFrsCo);
}

MainWindow::~MainWindow()
{
    delete ui;
    Log::delInstance();
    if (mCtlThr) {
        delete mCtlThr;
        mCtlThr = nullptr;
    }
}

void MainWindow::updateReVFrsCo(const int reVFrsCo) {
    // 当剩余视频帧数量为0时录制完成
    if (0 == reVFrsCo) {
        if (ui->label->isVisible()) {
            ui->label->hide();
        }
        if (ui->progressBar->isVisible()) {
            ui->progressBar->setValue(100);
            ui->progressBar->hide();
        }

        return;
    }

    if (!ui->label->isVisible()) {
        ui->label->show();
    }
    if (!ui->progressBar->isVisible()) {
        ui->progressBar->show();
    }

    ui->progressBar->setValue(reVFrsCo);
}

void MainWindow::startRe() {
    ui->startRecoder->setEnabled(false);
    ui->pauseRecoder->setEnabled(true);

    emit startingRe();

    mStartingRe = true;
}

void MainWindow::pauseRe() {
    if (!mStartingRe) {
        QMessageBox::information(nullptr, "", tr("Please start recording first!"));
        return;
    }

    ui->pauseRecoder->setEnabled(false);
    ui->startRecoder->setEnabled(true);

    emit pausingRe();
}

void MainWindow::stopRe() {
    if (!mStartingRe) {
        return;
    }

    ui->startRecoder->setEnabled(true);
    ui->pauseRecoder->setEnabled(true);

    emit stopingRe();

    mStartingRe = false;
}
