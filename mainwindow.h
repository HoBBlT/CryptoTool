#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onPlayButtonClicked();
    void onBackButtonClicked();
    void onSelectButtonClicked();
    void onChoosePrivateKeyButtonClicked();
    void onChoosePublicKeyButtonClicked();
    void onChoosePathDirKeyButtonClicked();
    void onGenKeysButtonClicked();
    void onSignButtonClicked();
    void onVerifyButtonClicked();

private:
    Ui::MainWindow *ui;
    void connectSignals();
    QString chooseFile();
};
#endif // MAINWINDOW_H
