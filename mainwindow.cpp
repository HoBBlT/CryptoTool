#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cryptoservice.h"
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Sign And Verify");
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPlayButtonClicked(){
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::onBackButtonClicked(){
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::onSelectButtonClicked(){
    QString path = chooseFile();
    if (path.isEmpty()) return;
    ui->filePathLabel->setText(path);
}

void MainWindow::onChoosePrivateKeyButtonClicked(){
    QString path = chooseFile();
    if (path.isEmpty()) return;
    ui->pathPrivateKeyLabel->setText(path);
}

void MainWindow::onChoosePublicKeyButtonClicked(){
    QString path = chooseFile();
    if (path.isEmpty()) return;
    ui->pathPublicKeyLabel->setText(path);
}

void MainWindow::onChoosePathDirKeyButtonClicked(){
    QString dir = QFileDialog::getExistingDirectory(
        this, "Выберите папку", QDir::homePath());
    if (dir.isEmpty()) return;
    ui->pathDirKeyLabel->setText(dir);
}

void MainWindow::onGenKeysButtonClicked(){

    QString keyDir = ui->pathDirKeyLabel->text(); // путь до папки с ключами

    if(keyDir.isEmpty() || keyDir == "Path to the directory for keys"){
        QMessageBox::critical(
            this, "Error", "Choose path to the directory for keys");
        return;
    }

    //генерируем закрытый ключ RSA 2048

    auto a = crypto::genKeys(keyDir);
    if ( !a.ok) {
        QMessageBox::critical(
            this,"Error", a.message);
        return;
    }

    QMessageBox::information(this, "Done", "Keys successfully generated\n" + keyDir);
}

void MainWindow::onSignButtonClicked(){

    //пути
    QString filePath = ui->filePathLabel->text();
    QString privateKeyPath = ui->pathPrivateKeyLabel->text();

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "First, select a file");
        return;
    }

    if (privateKeyPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "First, select a private key");
        return;
    }
    auto a = crypto::signFile(filePath, privateKeyPath);
    if (!a.ok) {
        QMessageBox::critical(this, "Error", a.message);
        return;
    }

    QMessageBox::information(this, "Done", "The file has been signed. The signature has been saved next to the file.");
}

void MainWindow::onVerifyButtonClicked(){

    QString filePath       = ui->filePathLabel->text();
    QString publicKeyPath  = ui->pathPublicKeyLabel->text();

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "First, select a file");
        return;
    }

    if (publicKeyPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "First, select a public key");
        return;
    }
    auto a = crypto::verifyFile(filePath, publicKeyPath);
    if (!a.ok) {
        QMessageBox::critical(this, "Check", a.message);
        return;

    }
    QMessageBox::information(this, "Check", "Signature is authentic.\nFile has not been modified");
}

void MainWindow::connectSignals(){
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::onPlayButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
    connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::onSelectButtonClicked);
    connect(ui->choosePrivateKeyButton, &QPushButton::clicked, this, &MainWindow::onChoosePrivateKeyButtonClicked);
    connect(ui->choosePublicKeyButton, &QPushButton::clicked, this, &MainWindow::onChoosePublicKeyButtonClicked);
    connect(ui->choosePathDirKeyButton, &QPushButton::clicked, this, &MainWindow::onChoosePathDirKeyButtonClicked);
    connect(ui->genKeysButton, &QPushButton::clicked, this, &MainWindow::onGenKeysButtonClicked);
    connect(ui->signButton, &QPushButton::clicked, this, &MainWindow::onSignButtonClicked);
    connect(ui->verifyButton, &QPushButton::clicked, this, &MainWindow::onVerifyButtonClicked);
}

QString MainWindow::chooseFile(){
    QString path = QFileDialog::getOpenFileName(
        this,                   // родитель
        "Выберите файл",        // заголовок
        QDir::homePath(),       // начальная папка
        "Все файлы (*.*)"       // фильтр
        );

    return path;
}