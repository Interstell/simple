#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

typedef enum EFFECTS{
    BLINK = 2,
    FADE,
    CANDLE,
    NOEFFECT = 256
} effect_e;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    char* fromDecToHex(int Saturation, int R, int G, int B);
    char* fromDecToHex(int Saturation, int R, int G, int B, effect_e effect, int speed);
    void MainWindow::sendEffect(effect_e effect);
    void sendColor(void);
    void updateLabels();
    bool random = false;
    bool candleEffect = false;
    bool blinkEffect = false;
    bool fadeEffect = false;
    ~MainWindow();

private slots:
    void on_slBrightness_valueChanged(int value);

    void on_slRed_valueChanged(int value);

    void on_slGreen_valueChanged(int value);

    void on_slBlue_valueChanged(int value);

    void on_pushButton_clicked();

    void on_valueBr_textEdited(const QString &arg1);

    void on_valueRed_textEdited(const QString &arg1);

    void on_valueGreen_textEdited(const QString &arg1);

    void on_valueBlue_textEdited(const QString &arg1);

    void on_btnCandleEffect_clicked();

    void on_slBlink_sliderReleased();

    void on_slFade_sliderReleased();

    void on_vol_sliderMoved(int position);

    void on_play_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
