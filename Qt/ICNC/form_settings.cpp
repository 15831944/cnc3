#include "form_settings.h"
#include <cstdint>
#include <QToolTip>
#include <QScrollArea>

#include "program_param.h"

// todo: message view
FormSettings::FormSettings(ProgramParam& par, QWidget *parent) : QWidget(parent), par(par) {
    createSettingsWidget();
    createButtons();

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(widgetSettings);
    scrollArea->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea, Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addLayout(gridButtons);

    this->setLayout(mainLayout);
}

QGroupBox* FormSettings::groupLabelNum(QLabel* label, QDoubleSpinBox* num) {
    QGroupBox* group = new QGroupBox;
    label->setBuddy(num);
    group->setLayout(new QHBoxLayout);
    group->layout()->addWidget(label);
    group->layout()->addWidget(num);
    return group;
}

QGroupBox* FormSettings::groupLabelNum(QLabel* label, QSpinBox* num, QComboBox* combo) {
    QGroupBox* group = new QGroupBox;
    label->setBuddy(num);
    group->setLayout(new QHBoxLayout);
    group->layout()->addWidget(label);
    group->layout()->addWidget(num);
    group->layout()->addWidget(combo);
    return group;
}

QGroupBox* FormSettings::groupLabelCombo(QLabel* label, QComboBox* combo) {
    QGroupBox* group = new QGroupBox;
    label->setBuddy(combo);
    group->setLayout(new QHBoxLayout);
    group->layout()->addWidget(label);
    group->layout()->addWidget(combo);
    return group;
}

void FormSettings::createSettingsWidget() {
    widgetSettings = new QWidget(this);
    gridSettings = new QGridLayout;

    labelTitle = new QLabel("<h3>" + tr("Application settings") + "</h3>");
    labelCNC = new QLabel("<h3>" + tr("CNC parameters") + "</h3>");
//    labelTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelLanguage = new QLabel("Language: ");

    comboLanguage = new QComboBox;
    comboLanguage->addItem("English");
    comboLanguage->addItem("Русский");

    checkSwapXY = new QCheckBox(tr("Swap plot axes X, Y"));
    checkReverseX = new QCheckBox(tr("Reverse plot axis X"));
    checkReverseY = new QCheckBox(tr("Reverse plot axis Y"));
    checkShowXY = new QCheckBox(tr("Show plot axes names"));

    labelInputLevel = new QLabel(tr("Input Levels, bits") + ": ");
    numInputLevel = new QSpinBox;
    numInputLevel->setDisplayIntegerBase(16);
    numInputLevel->setPrefix("0x");
    numInputLevel->setRange(0, INT32_MAX);

    comboInputLevel = new QComboBox;
    comboInputLevel->addItem(tr(""));
    comboInputLevel->addItem(tr("Metal"));
    comboInputLevel->addItem(tr("Stone"));
    comboInputLevel->addItem(tr("Debug"));

    checkReverseMotorX = new QCheckBox(tr("Reverse motor X"));
    checkReverseMotorY = new QCheckBox(tr("Reverse motor Y"));
    checkSwapMotorXY = new QCheckBox(tr("Swap motors X and Y"));
#ifndef STONE
    checkStepDirEnable = new QCheckBox(tr("Enable servomotors"));
    checkEncXY = new QCheckBox(tr("Enable XY linear encoders"));
    checkReverseMotorU = new QCheckBox(tr("Reverse motor U"));
    checkReverseMotorV = new QCheckBox(tr("Reverse motor V"));    
    checkSwapMotorUV = new QCheckBox(tr("Swap motors U and V"));
    checkReverseEncX = new QCheckBox(tr("Reverse Encoder X"));
    checkReverseEncY = new QCheckBox(tr("Reverse Encoder Y"));

    labelAcc = new QLabel(tr("Acceleration") + ":");
    fnumAcc = new QDoubleSpinBox(this);
    fnumAcc->setRange(1, 1000);
    fnumAcc->setSuffix(" " + tr("um/sec2")); // per 100 V
    fnumAcc->setDecimals(0);
    fnumAcc->setSingleStep(10);

    labelDec = new QLabel(tr("Deceleration") + ":");
    fnumDec = new QDoubleSpinBox(this);
    fnumDec->setRange(1, 1000);
    fnumDec->setSuffix(" " + tr("um/sec2")); // per 100 V
    fnumDec->setDecimals(0);
    fnumDec->setSingleStep(10);

    // Feedback
    groupFeedback = new QGroupBox(tr("Feedback enable"));
    groupFeedback->setCheckable(true);

    labelHighThld = new QLabel(tr("High threshold"));
    labelLowThld = new QLabel(tr("Low threshold"));
    labelRbTimeout = new QLabel(tr("Rollback timeout"));
    labelRbAttempts = new QLabel(tr("Rollback attempts"));
    labelRbLength = new QLabel(tr("Rollback length"));
    labelRbSpeed = new QLabel(tr("Rollback speed"));

    int thld_max = static_cast<int>( round(cnc_adc_volt_t::maxVolt(0)) );

    numHighThld = new QSpinBox;
    labelHighThld->setBuddy(numHighThld);
    numHighThld->setRange(0, thld_max);
    numHighThld->setSuffix(" " + tr("V"));

    numLowThld = new QSpinBox;
    labelLowThld->setBuddy(numLowThld);
    numLowThld->setRange(0, thld_max);
    numLowThld->setSuffix(" " + tr("V"));

    numRbTimeout = new QSpinBox;
    labelRbTimeout->setBuddy(numRbTimeout);
    numRbTimeout->setRange(10, 60);
    numRbTimeout->setSuffix(" " + tr("sec"));

    numRbAttempts = new QSpinBox;
    labelRbAttempts->setBuddy(numRbAttempts);
    numRbAttempts->setRange(1, 5);

    fnumRbLength = new QDoubleSpinBox;
    labelRbLength->setBuddy(fnumRbLength);
    fnumRbLength->setRange(0.100, 0.500);
    fnumRbLength->setSuffix(" " + tr("mm"));
    fnumRbLength->setDecimals(3);
    fnumRbLength->setSingleStep(0.01);

    fnumRbSpeed = new QDoubleSpinBox;
    labelRbSpeed->setBuddy(fnumRbSpeed);
    fnumRbSpeed->setRange(0.1, 2);
    fnumRbSpeed->setSuffix(" " + tr("mm/min"));
    fnumRbSpeed->setDecimals(1);
    fnumRbSpeed->setSingleStep(0.1);

    QGridLayout* gridFeedback = new QGridLayout;
    gridFeedback->addWidget(labelHighThld, 0, 0);
    gridFeedback->addWidget(numHighThld, 0, 1);
    gridFeedback->addWidget(labelLowThld, 1, 0);
    gridFeedback->addWidget(numLowThld, 1, 1);
    gridFeedback->addWidget(labelRbTimeout, 2, 0);
    gridFeedback->addWidget(numRbTimeout, 2, 1);
    gridFeedback->addWidget(labelRbAttempts, 3, 0);
    gridFeedback->addWidget(numRbAttempts, 3, 1);
    gridFeedback->addWidget(labelRbLength, 4, 0);
    gridFeedback->addWidget(fnumRbLength, 4, 1);
    gridFeedback->addWidget(labelRbSpeed, 5, 0);
    gridFeedback->addWidget(fnumRbSpeed, 5, 1);

    groupFeedback->setLayout(gridFeedback);
#endif

    //    
    numStep = new QDoubleSpinBox;
    labelStep = new QLabel(tr("Calculation step") + ": ");
    labelStep->setBuddy(numStep);
    numStep->setRange(0.001, 1.0);
    // todo: bind range with scale
    numStep->setSingleStep(0.001);
    numStep->setDecimals(3);
    numStep->setSuffix(" " + tr("mm"));
#ifdef STONE
    numStep->setEnabled(false);
#endif

    labelX = new QLabel("X: ");
    labelY = new QLabel("Y: ");

    labelPrecision = new QLabel(tr("Precision (steps/mm)"));
    labelMotor = new QLabel(tr("Motor"));

    numScaleX = new QDoubleSpinBox;
    labelX->setBuddy(numScaleX);
    numScaleX->setRange(1, 1000000);
    numScaleX->setDecimals(0);
#ifdef STONE
    numScaleX->setEnabled(false);
#endif

    numScaleY = new QDoubleSpinBox;
    labelY->setBuddy(numScaleY);
    numScaleY->setRange(1, 1000000);
    numScaleY->setDecimals(0);
#ifdef STONE
    numScaleY->setEnabled(false);
#endif

#ifndef STONE
    labelU = new QLabel("U: ");
    labelV = new QLabel("V: ");
    labelEncX = new QLabel("X: ");
    labelEncY = new QLabel("Y: ");

    labelEncoder = new QLabel(tr("Encoder"));

    numScaleU = new QDoubleSpinBox;
    labelU->setBuddy(numScaleU);
    numScaleU->setRange(1, 1000000);
    numScaleU->setDecimals(0);

    numScaleV = new QDoubleSpinBox;
    labelV->setBuddy(numScaleV);
    numScaleV->setRange(1, 1000000);
    numScaleV->setDecimals(0);

    numScaleEncX = new QDoubleSpinBox;
    labelEncX->setBuddy(numScaleEncX);
    numScaleEncX->setRange(1, 1000);
    numScaleEncX->setDecimals(0);

    numScaleEncY = new QDoubleSpinBox;
    labelEncY->setBuddy(numScaleEncY);
    numScaleEncY->setRange(1, 1000);
    numScaleEncY->setDecimals(0);
#endif

#ifndef STONE
    checks = {
        checkReverseX, checkReverseY, checkSwapXY, checkShowXY,
        checkStepDirEnable, checkEncXY,
        checkReverseMotorX, checkReverseMotorY, checkReverseMotorU, checkReverseMotorV,
        checkSwapMotorXY, checkSwapMotorUV,
        checkReverseEncX, checkReverseEncY
    };
    scaleNums = {numScaleX, numScaleY, numScaleU, numScaleV, numScaleEncX, numScaleEncY};
#else
    checks = {
        checkReverseX, checkReverseY, checkSwapXY, checkShowXY,
        checkReverseMotorX, checkReverseMotorY,
        checkSwapMotorXY
    };
    scaleNums = {numScaleX, numScaleY};
#endif

    init();
    setFontPointSize(16);

    gridSettings = new QGridLayout;

    gridSettings->addWidget(labelTitle, 0, 1, 1, 4, Qt::AlignHCenter | Qt::AlignBottom);
    gridSettings->addWidget(new QFrame, 1, 0, 1, 4);

    gridSettings->addWidget(groupLabelCombo(labelLanguage, comboLanguage), 2, 1, 1, 2);
    gridSettings->addWidget(checkSwapXY, 3, 1, 1, 4);
    gridSettings->addWidget(checkReverseX, 4, 1, 1, 4);
    gridSettings->addWidget(checkReverseY, 5, 1, 1, 4);
    gridSettings->addWidget(checkShowXY, 6, 1, 1, 4);

    gridSettings->addWidget(new QFrame, 7, 0, 1, 4);
    gridSettings->addWidget(labelCNC, 8, 1, 1, 4, Qt::AlignHCenter | Qt::AlignBottom);

    gridSettings->addWidget(groupLabelNum(labelInputLevel, numInputLevel, comboInputLevel), 9, 1, 1, 3);

    gridSettings->addWidget(groupLabelNum(labelStep, numStep), 10, 1, 1, 2);

#ifndef STONE
    gridSettings->addWidget(checkStepDirEnable, 11, 1, 1, 4);
    gridSettings->addWidget(checkEncXY, 12, 1, 1, 4);
#endif
    gridSettings->addWidget(checkReverseMotorX, 13, 1, 1, 4);
    gridSettings->addWidget(checkReverseMotorY, 14, 1, 1, 4);
#ifndef STONE
    gridSettings->addWidget(checkReverseMotorU, 15, 1, 1, 4);
    gridSettings->addWidget(checkReverseMotorV, 16, 1, 1, 4);
#endif

    gridSettings->addWidget(checkSwapMotorXY, 17, 1, 1, 4);
#ifndef STONE
    gridSettings->addWidget(checkSwapMotorUV, 18, 1, 1, 4);

    gridSettings->addWidget(checkReverseEncX, 19, 1, 1, 4);
    gridSettings->addWidget(checkReverseEncY, 20, 1, 1, 4);

    gridSettings->addWidget(groupLabelNum(labelAcc, fnumAcc), 21, 1, 1, 2);
    gridSettings->addWidget(groupLabelNum(labelDec, fnumDec), 22, 1, 1, 2);

    gridSettings->addWidget(groupFeedback, 23, 1, 1, 2);
#endif

    gridSettings->addWidget(labelPrecision, 24, 1, 1, 4, Qt::AlignHCenter | Qt::AlignBottom);

    gridSettings->addWidget(labelMotor, 25, 0);
    gridSettings->addWidget(groupLabelNum(labelX, numScaleX), 25, 1, 1, 2);
    gridSettings->addWidget(groupLabelNum(labelY, numScaleY), 25, 3, 1, 2);

#ifndef STONE
    gridSettings->addWidget(groupLabelNum(labelU, numScaleU), 26, 1, 1, 2);
    gridSettings->addWidget(groupLabelNum(labelV, numScaleV), 26, 3, 1, 2);

    gridSettings->addWidget(labelEncoder, 27, 0);
    gridSettings->addWidget(groupLabelNum(labelEncX, numScaleEncX), 27, 1, 1, 2);
    gridSettings->addWidget(groupLabelNum(labelEncY, numScaleEncY), 27, 3, 1, 2);
#endif

//    gridSettings->addWidget(new QFrame, 0, 3, 8, 1);
//    gridSettings->addWidget(new QFrame, 8, 0, 1, 4);

//    gridSettings->setSizeConstraint(QLayout::SetFixedSize);
    QWidget* widgetInside = new QWidget(this);
    widgetInside->setLayout(gridSettings);

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(new QFrame, 0, 0);
    grid->addWidget(widgetInside, 0, 1);
    grid->addWidget(new QFrame, 0, 2);
    grid->addWidget(new QFrame, 1, 0, 1, 3);

    widgetSettings->setLayout(grid);

    //
    connect(comboLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int i) {
        switch (i) {
        case int(InterfaceLanguage::RUSSIAN):
            ProgramParam::saveInterfaceLanguage(InterfaceLanguage::RUSSIAN);
            break;
        default:
            ProgramParam::saveInterfaceLanguage(InterfaceLanguage::ENGLISH);
            break;
        }

        emit showWarning("Interface language will be changed after reboot");
    });

    connect(checkSwapXY, &QCheckBox::stateChanged, [=](int state) {
        ProgramParam::saveSwapXY(state == Qt::Checked);
    });

    connect(checkReverseX, &QCheckBox::stateChanged, [=](int state) {
        ProgramParam::saveReverseX(state == Qt::Checked);
    });

    connect(checkReverseY, &QCheckBox::stateChanged, [=](int state) {
        ProgramParam::saveReverseY(state == Qt::Checked);
    });

    connect(checkShowXY, &QCheckBox::stateChanged, [=](int state) {
        ProgramParam::saveShowXY(state == Qt::Checked);
    });

    connect(comboInputLevel, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int i) {
        numInputLevel->blockSignals(true);

        switch (i) {
        case 1: numInputLevel->setValue(CncParam::INPUT_LEVEL_METAL); break;
        case 2: numInputLevel->setValue(CncParam::INPUT_LEVEL_STONE); break;
        case 3: numInputLevel->setValue(CncParam::INPUT_LEVEL_DEBUG); break;
        }

        numInputLevel->blockSignals(false);
    });

    connect(numInputLevel, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int /*value*/) {
        selectComboInputLevel(numInputLevel->value());
    });

    connect(checkReverseMotorX, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);
    connect(checkReverseMotorY, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);
    connect(checkSwapMotorXY, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);

#ifndef STONE
    connect(checkReverseMotorU, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);
    connect(checkReverseMotorV, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);
    connect(checkSwapMotorUV, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);    
    connect(checkReverseEncX, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);
    connect(checkReverseEncY, &QCheckBox::stateChanged, this, &FormSettings::onStateChangedMotor);
#endif

//    connect(numHighThld, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value) {
//        if (value < numLowThld->value())
//            numLowThld->setValue(value);
//    });

//    connect(numLowThld, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value) {
//        if (value > numHighThld->value())
//            numHighThld->setValue(value);
//    });

    connect(numScaleX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &FormSettings::onNumScaleChanged);
    connect(numScaleY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &FormSettings::onNumScaleChanged);
//    connect(numScaleU, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &FormSettings::onNumScaleChanged);
//    connect(numScaleV, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &FormSettings::onNumScaleChanged);

//    connect(numScaleEncX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &FormSettings::onNumScaleChanged);
//    connect(numScaleEncY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &FormSettings::onNumScaleChanged);
}

void FormSettings::init() {
    comboLanguage->setCurrentIndex(int(ProgramParam::lang));
    checkSwapXY->setCheckState(ProgramParam::swapXY ? Qt::Checked : Qt::Unchecked);
    checkReverseX->setCheckState(ProgramParam::reverseX ? Qt::Checked : Qt::Unchecked);
    checkReverseY->setCheckState(ProgramParam::reverseY ? Qt::Checked : Qt::Unchecked);
    checkShowXY->setCheckState(ProgramParam::showXY ? Qt::Checked : Qt::Unchecked);
    numInputLevel->setValue(int(CncParam::inputLevel));

    selectComboInputLevel(numInputLevel->value());

#ifndef STONE
    checkStepDirEnable->setCheckState(CncParam::sdEnable ? Qt::Checked : Qt::Unchecked);
    checkEncXY->setCheckState(CncParam::encXY ? Qt::Checked : Qt::Unchecked);
#endif

    for (QCheckBox* const o: checks)
        o->blockSignals(true);

    checkReverseMotorX->setCheckState(CncParam::reverseX ? Qt::Checked : Qt::Unchecked);
    checkReverseMotorY->setCheckState(CncParam::reverseY ? Qt::Checked : Qt::Unchecked);
    checkSwapMotorXY->setCheckState(CncParam::swapXY ? Qt::Checked : Qt::Unchecked);

#ifndef STONE
    checkReverseMotorU->setCheckState(CncParam::reverseU ? Qt::Checked : Qt::Unchecked);
    checkReverseMotorV->setCheckState(CncParam::reverseV ? Qt::Checked : Qt::Unchecked);
    checkSwapMotorUV->setCheckState(CncParam::swapUV ? Qt::Checked : Qt::Unchecked);
    checkReverseEncX->setCheckState(CncParam::reverseEncX ? Qt::Checked : Qt::Unchecked);
    checkReverseEncY->setCheckState(CncParam::reverseEncY ? Qt::Checked : Qt::Unchecked);
#endif

    onStateChangedMotor(0);

    for (QCheckBox* const o: checks)
        o->blockSignals(false);

#ifndef STONE
    fnumAcc->setValue(CncParam::acc);
    fnumDec->setValue(CncParam::dec);
    groupFeedback->setChecked(CncParam::fb_ena);

    numHighThld->setValue(CncParam::high_thld);
    numLowThld->setValue(CncParam::low_thld);
    numRbTimeout->setValue(CncParam::rb_to);
    numRbAttempts->setValue(CncParam::rb_attempts);
    fnumRbLength->setValue(CncParam::rb_len);
    fnumRbSpeed->setValue(CncParam::rb_speed);
#endif

    numStep->setValue(CncParam::step);

    for (QDoubleSpinBox* const o: scaleNums)
        o->blockSignals(true);

    numScaleX->setValue(CncParam::scaleX);
    numScaleY->setValue(CncParam::scaleY);
#ifndef STONE
    numScaleU->setValue(CncParam::scaleU);
    numScaleV->setValue(CncParam::scaleV);

    numScaleEncX->setValue(CncParam::scaleEncX);
    numScaleEncY->setValue(CncParam::scaleEncY);
#endif

    onNumScaleChanged(0);

    for (QDoubleSpinBox* const o: scaleNums)
        o->blockSignals(false);
}

void FormSettings::reset() {
    ProgramParam::loadDefaultParam();
    init();
}

void FormSettings::selectComboInputLevel(int bits) {
    comboInputLevel->blockSignals(true);

    switch (bits) {
    case (CncParam::INPUT_LEVEL_METAL): comboInputLevel->setCurrentIndex(1); break;
    case (CncParam::INPUT_LEVEL_STONE): comboInputLevel->setCurrentIndex(2); break;
    case (CncParam::INPUT_LEVEL_DEBUG): comboInputLevel->setCurrentIndex(3); break;
    default: comboInputLevel->setCurrentIndex(0); break;
    }

    comboInputLevel->blockSignals(false);
}

void FormSettings::onNumScaleChanged(double) {
    double value = std::min(numScaleX->value(), numScaleY->value());
    double step = 1.0 / value;

    if (step < 0.001)
        step = 0.001;

    numStep->setMinimum(step);
}

void FormSettings::onStateChangedMotor(int /*state*/) {
#ifndef STONE
    ProgramParam::saveMotorDir(
        checkReverseMotorX->isChecked(), checkReverseMotorY->isChecked(), checkReverseMotorU->isChecked(), checkReverseMotorV->isChecked(),
        checkSwapMotorXY->isChecked(), checkSwapMotorUV->isChecked(),
        checkReverseEncX->isChecked(), checkReverseEncY->isChecked()
    );
#else
    ProgramParam::saveMotorDir(
        checkReverseMotorX->isChecked(), checkReverseMotorY->isChecked(), false, false,
        checkSwapMotorXY->isChecked(), false,
        false, false
    );
#endif
}

FormSettings::~FormSettings() {}

void FormSettings::setFontPointSize(int pointSize) {
    QFont font;

    for (QPushButton* b: buttons) {
        font = b->font();
        font.setPointSize(pointSize);
        b->setFont(font);
    }

    font = labelX->font();
    font.setPointSize(pointSize + 2);

    labelTitle->setFont(font);
    labelCNC->setFont(font);
    labelLanguage->setFont(font);
    labelInputLevel->setFont(font);
    labelStep->setFont(font);

    labelX->setFont(font);
    labelY->setFont(font);

    labelPrecision->setFont(font);
    labelMotor->setFont(font);

    //
    font = comboLanguage->font();
    font.setPointSize(pointSize + 2);

    comboLanguage->setFont(font);
    comboInputLevel->setFont(font);

    //
    font = numStep->font();
    font.setPointSize(pointSize + 2);

    numInputLevel->setFont(font);
    numStep->setFont(font);

    for (QDoubleSpinBox* const o: scaleNums)
        o->setFont(font);

    font = checkSwapXY->font();
    font.setPointSize(pointSize + 2);

    for (QCheckBox* const o: checks)
        o->setFont(font);

#ifndef STONE
    labelEncoder->setFont(font);
    labelU->setFont(font);
    labelV->setFont(font);
    labelEncX->setFont(font);
    labelEncY->setFont(font);

    labelAcc->setFont(font);
    labelDec->setFont(font);
    fnumAcc->setFont(font);
    fnumDec->setFont(font);

    groupFeedback->setFont(font);
    labelHighThld->setFont(font);
    labelLowThld->setFont(font);
    numHighThld->setFont(font);
    numLowThld->setFont(font);
#endif
}

void FormSettings::createButtons() {
    btnHome = new QPushButton(tr("Back"));
    btnHome->setStatusTip(tr("Back to the home panel"));

    btnRead = new QPushButton(tr("Read"));
    btnRead->setStatusTip(tr("Read settings from CNC"));

    btnWrite = new QPushButton(tr("Write"));
    btnWrite->setStatusTip(tr("Write settings"));

    btnDefault = new QPushButton(tr("Default"));
    btnDefault->setStatusTip(tr("Default settings"));

    btn4 = new QPushButton();
    btn4->setText(tr(""));
    btn4->setEnabled(false);

    btn5 = new QPushButton();
    btn5->setText(tr(""));
    btn5->setEnabled(false);

    btn6 = new QPushButton();
    btn6->setText(tr(""));
    btn6->setEnabled(false);

    btn7 = new QPushButton();
    btn7->setText(tr(""));
    btn7->setEnabled(false);

    btn8 = new QPushButton();
    btn8->setText(tr(""));
    btn8->setEnabled(false);

    btn9 = new QPushButton();
    btn9->setText(tr(""));
    btn9->setEnabled(false);

    btn10 = new QPushButton();
    btn10->setText(tr(""));
    btn10->setEnabled(false);

    btn11 = new QPushButton();
    btn11->setText(tr(""));
    btn11->setEnabled(false);

    btn12 = new QPushButton();
    btn12->setText(tr(""));
    btn12->setEnabled(false);

    btnHelp = new QPushButton();
    btnHelp->setText(tr("Help"));

    buttons = {btnHome, btnRead, btnWrite, btnDefault, btn4, btn5, btn6, btn7, btn8, btn9, btn10, btn11, btn12, btnHelp};

    gridButtons = new QGridLayout();

    gridButtons->addWidget(btnHome, 0, 0);
    gridButtons->addWidget(btnRead, 0, 1);
    gridButtons->addWidget(btnWrite, 0, 2);
    gridButtons->addWidget(btnDefault, 0, 3);
    gridButtons->addWidget(btn4, 0, 4);
    gridButtons->addWidget(btn5, 0, 5);
    gridButtons->addWidget(btn6, 0, 6);
    gridButtons->addWidget(btn7, 0, 7);
    gridButtons->addWidget(btn8, 0, 8);
    gridButtons->addWidget(btn9, 0, 9);
    gridButtons->addWidget(btn10, 0, 10);
    gridButtons->addWidget(btn11, 0, 11);
    gridButtons->addWidget(btn12, 0, 12);
    gridButtons->addWidget(btnHelp, 0, 13);

    connect(btnHome, &QPushButton::clicked, this, [&]() {
        emit showInfo(QString());
        emit homePageClicked();
    });

    connect(btnRead, &QPushButton::clicked, this, [&]() {
        uint16_t input_lvl;
        bool sdEna, revX, revY, revU, revV, swapXY, swapUV, revEncX, revEncY;
        double acc, dec;

        bool OK = par.cnc.readSettings(
                    input_lvl, sdEna, sdEna, revX, revY, revU, revV,
                    swapXY, swapUV,
                    revEncX, revEncY,
                    acc, dec
        );

        if (OK) {
            ProgramParam::saveInputLevel(input_lvl);
            ProgramParam::saveStepDir(sdEna);
            ProgramParam::saveMotorDir(revX, revY, revU, revV, swapXY, swapUV, revEncX, revEncY);
            ProgramParam::saveAcceleration(acc, dec);

            numInputLevel->setValue(input_lvl);
            checkReverseMotorX->setCheckState(revX ? Qt::Checked : Qt::Unchecked);
            checkReverseMotorY->setCheckState(revY ? Qt::Checked : Qt::Unchecked);
            checkSwapMotorXY->setCheckState(swapXY ? Qt::Checked : Qt::Unchecked);            
#ifndef STONE
            checkStepDirEnable->setCheckState(sdEna ? Qt::Checked : Qt::Unchecked);
            checkReverseMotorU->setCheckState(revU ? Qt::Checked : Qt::Unchecked);
            checkReverseMotorV->setCheckState(revV ? Qt::Checked : Qt::Unchecked);
            checkSwapMotorUV->setCheckState(swapUV ? Qt::Checked : Qt::Unchecked);
            checkReverseEncX->setCheckState(revEncX ? Qt::Checked : Qt::Unchecked);
            checkReverseEncY->setCheckState(revEncY ? Qt::Checked : Qt::Unchecked);
            fnumAcc->setValue(acc);
            fnumDec->setValue(dec);
#endif
        }
        else
            qDebug("Read Input Levels ERROR!\n");

#ifndef STONE
        bool fbEna;
        uint32_t rbAttempts;
        double low_thld, high_thld, rbTimeout, rbLength, rbSpeed;

        if (OK)
            OK = par.cnc.readFeedback(fbEna, low_thld, high_thld, rbTimeout, rbAttempts, rbLength, rbSpeed);

        if (OK) {            
            ProgramParam::saveFeedbackParam(fbEna, low_thld, high_thld, rbTimeout, rbAttempts, rbLength, rbSpeed);

            numLowThld->blockSignals(true);
            numHighThld->blockSignals(true);

            // todo: save param
            groupFeedback->setChecked(fbEna);
            numLowThld->setValue(static_cast<int>(round(low_thld)));
            numHighThld->setValue(static_cast<int>(round(high_thld)));
            numRbTimeout->setValue(rbTimeout);
            numRbAttempts->setValue(rbAttempts);
            fnumRbLength->setValue(rbLength);
            fnumRbSpeed->setValue(rbSpeed);

            numLowThld->blockSignals(false);
            numHighThld->blockSignals(false);
        } else {
            groupFeedback->setChecked(false);
        }

        float step;
        bool encXY;
        float scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY;

        if (OK)
            OK = par.cnc.readStep(step, scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY, encXY);

        if (OK) {
            ProgramParam::saveStep(step, scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY, encXY);
            numStep->setValue(step);
            numScaleX->setValue(scaleX);
            numScaleY->setValue(scaleY);
            numScaleU->setValue(scaleU);
            numScaleV->setValue(scaleV);
            numScaleEncX->setValue(scaleEncX);
            numScaleEncY->setValue(scaleEncY);
            checkEncXY->setCheckState(encXY ? Qt::Checked : Qt::Unchecked);
        }
#endif

        if (OK)
            emit showInfo("Read OK");
        else
            emit showError("Read ERROR!");
    });

    connect(btnWrite, &QPushButton::clicked, this, [&]() {
        uint16_t input_lvl = static_cast<uint16_t>(numInputLevel->value() & 0xFFFF);
        bool sd_oe = checkStepDirEnable ? checkStepDirEnable->isChecked() : false;
        bool encXY = checkEncXY ? checkEncXY->isChecked() : false;
        bool revX = checkReverseMotorX->isChecked();
        bool revY = checkReverseMotorY->isChecked();
        bool revU = checkReverseMotorU ? checkReverseMotorU->isChecked() : false;
        bool revV = checkReverseMotorV ? checkReverseMotorV->isChecked() : false;
        bool swapXY = checkSwapMotorXY->isChecked();
        bool swapUV = checkSwapMotorUV ? checkSwapMotorUV->isChecked() : false;
        bool revEncX = checkReverseEncX ? checkReverseEncX->isChecked() : false;
        bool revEncY = checkReverseEncY ? checkReverseEncY->isChecked() : false;
        double acc = fnumAcc ? fnumAcc->value() : CncParam::DEFAULT_ACC;
        double dec = fnumDec ? fnumDec->value() : CncParam::DEFAULT_DEC;

#ifndef STONE
        float step = numStep->value();
#else
        const float step = CncParam::DEFAULT_STEP;
#endif

#ifndef STONE
        float scaleX = numScaleX->value();
        float scaleY = numScaleY->value();
#else
        const float scaleX = CncParam::DEFAULT_SCALE_XY;
        const float scaleY = CncParam::DEFAULT_SCALE_XY;
#endif
        float scaleU = numScaleU ? numScaleU->value() : CncParam::DEFAULT_SCALE_UV;
        float scaleV = numScaleV ? numScaleV->value() : CncParam::DEFAULT_SCALE_UV;
        float scaleEncX = numScaleEncX ? numScaleEncX->value() : CncParam::DEFAULT_SCALE_ENC_XY;
        float scaleEncY = numScaleEncY ? numScaleEncY->value() : CncParam::DEFAULT_SCALE_ENC_XY;

        ProgramParam::saveInputLevel(input_lvl);
        ProgramParam::saveMotorDir(revX, revY, revU, revV, swapXY, swapUV, revEncX, revEncY);
#ifndef STONE
        ProgramParam::saveAcceleration(acc, dec);
#endif
        ProgramParam::saveStep(step, scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY, encXY);

        bool OK =   par.cnc.writeSettings(
                        input_lvl,
                        sd_oe, sd_oe,
                        revX, revY, revU, revV,
                        swapXY, swapUV,
                        revEncX, revEncY,
                        acc, dec
                    );

#ifndef STONE
        if (numHighThld->value() < numLowThld->value())
            numHighThld->setValue( numLowThld->value() );

        ProgramParam::saveFeedbackParam(
                    groupFeedback->isChecked(),
                    numLowThld->value(),
                    numHighThld->value(),
                    numRbTimeout->value(),
                    numRbAttempts->value(),
                    fnumRbLength->value(),
                    fnumRbSpeed->value()
                );

        if (OK)
            OK &=   par.cnc.writeFeedback(
                    groupFeedback->isChecked(),
                    numLowThld->value(),
                    numHighThld->value(),
                    numRbTimeout->value(),
                    numRbAttempts->value(),
                    fnumRbLength->value(),
                    fnumRbSpeed->value()
                );

        if (OK)
            OK &= par.cnc.writeStep(step, scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY, encXY);
#endif

#ifdef STONE
        if (OK)
            OK &= par.cnc.writeSemaphoreCncEnable(true);
#else
        if (OK)
            OK &= par.cnc.writeCncEnable(true);
#endif

        if (OK) {
            emit showInfo("Write OK");
            btnRead->click();
        }
        else
            emit showError("Write ERROR!");
    });

    connect(btnDefault, &QPushButton::clicked, this, [&]() {
        reset();
    });

    connect(btnHelp, &QPushButton::clicked, this, [&]() { emit helpPageClicked(help_file); });
}
