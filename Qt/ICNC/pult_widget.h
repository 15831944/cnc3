#ifndef PULT_PLOT_WIDGET_H
#define PULT_PLOT_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QCheckBox>

#include "cnc_context.h"

class PultWidget : public QWidget {
    Q_OBJECT

public:
    enum class MoveMode { MM, STEPS };
    MoveMode moveMode = MoveMode::MM;

    QPushButton *btnGo = nullptr, *btnSet = nullptr;

    QDoubleSpinBox *numMoveX = nullptr, *numMoveY = nullptr, *numMoveU = nullptr, *numMoveV = nullptr;
    QDoubleSpinBox *numSetX = nullptr, *numSetY = nullptr, *numSetU = nullptr, *numSetV = nullptr;
    std::vector<QDoubleSpinBox*> moveNum, setNum, controlNum;

    QRadioButton *moveMM = nullptr, *moveSteps = nullptr;
    QGroupBox *groupMove = nullptr;

    QLabel *labelSpeed = nullptr;
    QDoubleSpinBox *numSpeed = nullptr;
    QRadioButton *speedMMM = nullptr, *speedUMS = nullptr;
    QGroupBox *groupSpeed = nullptr;
    QGridLayout *gridSpeed = nullptr;

    QPushButton *btnCancel = nullptr, *btnHold = nullptr;
    QGridLayout *gridButton = nullptr;

    QDoubleSpinBox *numScaleX = nullptr, *numScaleY = nullptr, *numScaleU = nullptr, *numScaleV = nullptr;
    QDoubleSpinBox *numScaleEncX = nullptr, *numScaleEncY = nullptr;
    std::vector<QDoubleSpinBox*> scaleNum;
    std::vector<QDoubleSpinBox*> encoderScaleNum;
    QGridLayout *gridScale = nullptr;

    std::vector<QLabel*> controlLabels;

    std::vector<QPushButton*> buttons;
    std::vector<QLabel*> labels;
    std::vector<QDoubleSpinBox*> nums;
    std::vector<QRadioButton*> radio;

    QLabel *labelTO = nullptr, *labelFB = nullptr, *labelPWR = nullptr, *labelALM = nullptr, *labelWB = nullptr, *labelREV = nullptr, *labelFWD = nullptr;
    std::vector<QLabel*> checkLabels;
    QCheckBox *checkTO = nullptr, *checkFB = nullptr, *checkPWR = nullptr, *checkALM = nullptr, *checkWB = nullptr, *checkREV = nullptr, *checkFWD = nullptr;

    QLabel *labelGap = nullptr, *labelRev = nullptr, *labelWorkpiece = nullptr, *labelWire = nullptr, *labelHV = nullptr, *labelShunt = nullptr;
    std::vector<QLabel*> adcLabels;
    std::vector<QLabel*> adcValueLabels;

private:
    std::vector<QLabel*> posLabels, posLabelsSmall, ctrlLabels;
    QGridLayout *gridView = nullptr, *gridMain = nullptr, *gridControl = nullptr, *gridControl2 = nullptr;

    void createView();
    void createControl();
    void createMove();
    void createMmSteps();
    void createScale();
    void createSpeed();
    void createButtons();

    static QGroupBox* groupLabelNum(QLabel* label, QDoubleSpinBox* num);

public:
    explicit PultWidget(QWidget *parent = nullptr);
    ~PultWidget();

    void setMotorView(size_t axis, int value);
    void setEncoderView(size_t axis, int value);

    void setLimitSwitches(CncLimitSwitches ls);
    void setLimitSwitches(bool fwd, bool rev, bool alarm, bool wire_break, bool pwr, bool feedback, bool fb_timeout);

    void setAdc(size_t adc_num, double value, bool mV_ena = false);
    void setAdc(size_t adc_num, double_valid_t value, bool mV_ena = false);
    void setAdc(const cnc_adc_volt_t& adc);

    int32_t getMoveN(size_t i) const;
    int32_t getSetN(size_t i) const;
    int32_t getSetEncN(size_t i) const;

    double speed() const;

    double scale(size_t i) const;
    double scaleEncoder(size_t i) const;

    void setFontPointSize(QWidget* w, int pointSize);
    void setFontPointSize(int pointSize);
};

#endif // PULT_PLOT_WIDGET_H
