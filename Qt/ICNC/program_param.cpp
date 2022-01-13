#include "program_param.h"
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <string>

InterfaceLanguage ProgramParam::lang = InterfaceLanguage::ENGLISH;
int ProgramParam::fontSize = 10;
bool ProgramParam::swapXY = false;
bool ProgramParam::reverseX = false;
bool ProgramParam::reverseY = false;
bool ProgramParam::showXY = false;

ProgramParam::ProgramParam() {
//    m_SettingsFile = QApplication::applicationDirPath() + "/settings.ini";
//    qDebug() << m_SettingsFile;
    setDefaultGenModeList();
    loadSettings();
//    cnc.bindCncParam(&cncParam);
}

ProgramParam::~ProgramParam() {
    saveSettings();
}

QString ProgramParam::loadString(QSettings& settings, const QString& key, const QString& defaultValue, bool& OK) {
    QString res;

    if (settings.contains(key))
        res = settings.value(key, defaultValue).toString();
    else {
        res = defaultValue;
        settings.setValue(key, defaultValue);
        OK = false;
    }

    return res;
}

bool ProgramParam::loadBool(QSettings& settings, const QString& key, const bool defaultValue, bool& OK) {
    bool res;

    if (settings.contains(key))
        res = settings.value(key, defaultValue).toBool();
    else {
        res = defaultValue;
        settings.setValue(key, defaultValue);
        OK = false;
    }

    return res;
}

unsigned ProgramParam::loadUInt(QSettings& settings, const QString& key, const unsigned minValue, const unsigned maxValue, const unsigned defaultValue, bool& OK) {
    unsigned res;
    bool l_OK = settings.contains(key);

    if (l_OK)
        res = settings.value(key, defaultValue).toUInt();

    if (!l_OK || res < minValue || res > maxValue) {
        res = defaultValue;
        settings.setValue(key, defaultValue);
        OK = false;
    }

    return res;
}

double ProgramParam::loadDouble(QSettings& settings, const QString& key, const double minValue, const double maxValue, const double defaultValue, bool& OK) {
    double res;
    bool l_OK = settings.contains(key);

    if (l_OK)
        res = settings.value(key, defaultValue).toDouble();

    if (!l_OK || res < minValue || res > maxValue) {
        res = defaultValue;
        settings.setValue(key, defaultValue);
        OK = false;
    }

    return res;
}

void ProgramParam::loadSettings() {
    bool OK = true;
    QSettings settings(org, app);
    fileDir = loadString(settings, "fileDir", "", OK);
    gcodeFileName = loadString(settings, "cncFileName", "", OK);
    contourFileName = loadString(settings, "contourFileName", "", OK);
}

void ProgramParam::saveSettings() {
//    QSettings settings(m_SettingsFile, QSettings::NativeFormat);
    QSettings settings(org, app);
    settings.setValue("fileDir", fileDir);
    settings.setValue("cncFileName", gcodeFileName);
    settings.setValue("contourFileName", contourFileName);
    //    settings.sync();
}

void ProgramParam::saveCncContext() {
    QSettings settings(org, app);
    QByteArray bytes = cncContext.getByteArray();
    settings.setValue("cnc_context_t",  bytes);
}

void ProgramParam::clearCncContext() {
    cncContext.setValid(false);
    saveCncContext();
}

void ProgramParam::loadCncContext() {
    QSettings settings(org, app);

    if (!settings.contains("cnc_context_t"))
        cncContext.setDefault();
    else {
        QByteArray bytes = settings.value("cnc_context_t", QByteArray()).toByteArray();
        cncContext.set(bytes);
    }
}

// save G-code as contex
void ProgramParam::saveGcode() {
    QSettings settings(org, app);
    clearCncContext();
    settings.setValue("gcodeCnc", gcodeCnc);
//    saveInputLevel(CncParam::inputLevel);
}

QString ProgramParam::loadGcode() {
    QSettings settings(org, app);
    bool OK = true;

    loadInputLevel(OK);

    if (OK)
        return settings.value("gcodeCnc", QString()).toString();

    return QString();
}

void ProgramParam::setDefaultGenModeList() {
    genModesList.clear();
    genModesList.push_back(GeneratorMode());
    genModesList.push_back(GeneratorMode());
    genModesList.push_back(GeneratorMode());
    genModesList.push_back(GeneratorMode());
    genModesList.push_back(GeneratorMode());
    genModesList.push_back(GeneratorMode());
    genModesList.push_back(GeneratorMode());
}

void ProgramParam::setDefaultCutParam() {
    cut = cut_t();
}

// Parse a text file into the binary structure
GCode ProgramParam::compile(const QString& txt) {
    using namespace std;

    GCode gcode;
    int err = gcode.parse(txt.toStdString());

    if (err == 0) {
        string new_txt = gcode.toText();
        qDebug() << new_txt.c_str();
    }
    else
        gcode.clear();

    return gcode;
}

void ProgramParam::loadBackup() {
    loadCncContext();

    QString txt = loadGcode();
    gcode = compile(txt);

    if (gcode.empty())
        clearCncContext();
    else {
        gcodeSettings = gcode.getSettings();
        gcodeText = gcode.toText().c_str();
    }
}

void ProgramParam::saveInterfaceLanguage(InterfaceLanguage lang) {
    QSettings settings(org, app);
    settings.setValue("language", static_cast<uint8_t>(lang));
    settings.sync();
}

InterfaceLanguage ProgramParam::loadInterfaceLanguage() {
    bool OK = true;
    QSettings settings(org, app);
    return lang = static_cast<InterfaceLanguage>(
                loadUInt(settings, "language", 0, static_cast<unsigned>(InterfaceLanguage::END) - 1, static_cast<unsigned>(InterfaceLanguage::ENGLISH), OK)
                );
}

QString ProgramParam::helpSubDir() {
#if defined(STONE)
    switch (lang) {
    case InterfaceLanguage::ENGLISH: return "stone/en";
    case InterfaceLanguage::RUSSIAN: return "stone/ru";
    default: return "stone/en";
    }
#else
    switch (lang) {
    case InterfaceLanguage::ENGLISH: return "metal/en";
    case InterfaceLanguage::RUSSIAN: return "metal/ru";
    default: return "metal/en";
    }
#endif
}

void ProgramParam::saveFontSize(int fontSize) {
    QSettings settings(org, app);
    ProgramParam::fontSize = fontSize;
    settings.setValue("fontSize", static_cast<uint8_t>(fontSize));
    settings.sync();
}

int ProgramParam::loadFontSize() {
    bool OK = true;
    QSettings settings(org, app);
    return fontSize = loadUInt(settings, "fontSize", 8, 32, 10, OK);
}

void ProgramParam::saveInputLevel(uint16_t value) {
    QSettings settings(org, app);
    CncParam::inputLevel = value;
    settings.setValue("inputLevel", CncParam::inputLevel);
    settings.sync();
}

uint16_t ProgramParam::loadInputLevel(bool& OK) {
    QSettings settings(org, app);    
    CncParam::inputLevel = static_cast<uint16_t>( loadUInt(settings, "inputLevel", 0, UINT16_MAX, CncParam::DEFAULT_INPUT_LEVEL, OK) );
    return CncParam::inputLevel;
}

void ProgramParam::saveStepDir(bool sdEna) {
    QSettings settings(org, app);
    CncParam::sdEnable = sdEna;
    settings.setValue("sdEnable", CncParam::sdEnable);
    settings.sync();
}

bool ProgramParam::loadStepDir(bool &OK) {
    QSettings settings(org, app);
    return CncParam::sdEnable = loadBool(settings, "sdEnable", CncParam::DEFAULT_SD_ENA, OK);
}

void ProgramParam::saveMotorDir(bool revX, bool revY, bool revU, bool revV, bool swapXY, bool swapUV, bool reverseEncX, bool reverseEncY) {
    QSettings settings(org, app);
    CncParam::reverseX = revX;
    CncParam::reverseY = revY;
    CncParam::reverseU = revU;
    CncParam::reverseV = revV;
    CncParam::swapXY = swapXY;
    CncParam::swapUV = swapUV;
    CncParam::reverseEncX = reverseEncX;
    CncParam::reverseEncY = reverseEncY;

    settings.setValue("motorReverseX", CncParam::reverseX);
    settings.setValue("motorReverseY", CncParam::reverseY);
    settings.setValue("motorReverseU", CncParam::reverseU);
    settings.setValue("motorReverseV", CncParam::reverseV);
    settings.setValue("motorSwapXY", CncParam::swapXY);
    settings.setValue("motorSwapUV", CncParam::swapUV);
    settings.setValue("encReverseX", CncParam::reverseEncX);
    settings.setValue("encReverseY", CncParam::reverseEncY);
    settings.sync();
}

void ProgramParam::saveFeedbackParam(bool fb_ena, double low_thld, double high_thld, double rb_to, unsigned rb_attempts, double rb_len, double rb_speed) {
    QSettings settings(org, app);
    CncParam::fb_ena = fb_ena;
    CncParam::low_thld = low_thld;
    CncParam::high_thld = high_thld;
    CncParam::rb_to = rb_to;
    CncParam::rb_attempts = rb_attempts;
    CncParam::rb_len = rb_len;
    CncParam::rb_speed = rb_speed;

    settings.setValue("feedbackEnable", CncParam::fb_ena);
    settings.setValue("lowThreshold", CncParam::low_thld);
    settings.setValue("highThreshold", CncParam::high_thld);
    settings.setValue("rollbackTimeout", CncParam::rb_to);
    settings.setValue("rollbackAttempts", CncParam::rb_attempts);
    settings.setValue("rollbackLength", CncParam::rb_len);
    settings.setValue("rollbackSpeed", CncParam::rb_speed);
    settings.sync();
}

void ProgramParam::saveAcceleration(double acc, double dec) {
    QSettings settings(org, app);
    CncParam::acc = acc;
    CncParam::dec = dec;

    settings.setValue("acceleration", CncParam::acc);
    settings.setValue("deceleration", CncParam::dec);
    settings.sync();
}

void ProgramParam::saveStep(double step, double scaleX, double scaleY, double scaleU, double scaleV, double scaleEncX, double scaleEncY, bool encXY) {
    QSettings settings(org, app);
    CncParam::step = step;
    CncParam::scaleX = scaleX;
    CncParam::scaleY = scaleY;
    CncParam::scaleU = scaleU;
    CncParam::scaleV = scaleV;
    CncParam::scaleEncX = scaleEncX;
    CncParam::scaleEncY = scaleEncY;
    CncParam::encXY = encXY;

    settings.setValue("step", CncParam::step);
    settings.setValue("scaleX", CncParam::scaleX);
    settings.setValue("scaleY", CncParam::scaleY);
    settings.setValue("scaleU", CncParam::scaleU);
    settings.setValue("scaleV", CncParam::scaleV);
    settings.setValue("scaleEncX", CncParam::scaleEncX);
    settings.setValue("scaleEncY", CncParam::scaleEncY);
    settings.setValue("encXY", CncParam::encXY);

    settings.sync();
}

bool ProgramParam::loadMotorReverseX() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::reverseX = loadBool(settings, "motorReverseX", false, OK);
}

bool ProgramParam::loadMotorReverseY() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::reverseY = loadBool(settings, "motorReverseY", false, OK);
}

bool ProgramParam::loadMotorReverseU() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::reverseU = loadBool(settings, "motorReverseU", false, OK);
}

bool ProgramParam::loadMotorReverseV() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::reverseV = loadBool(settings, "motorReverseV", false, OK);
}

bool ProgramParam::loadMotorSwapXY() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::swapXY = loadBool(settings, "motorSwapXY", false, OK);
}

bool ProgramParam::loadMotorSwapUV() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::swapUV = loadBool(settings, "motorSwapUV", false, OK);
}

bool ProgramParam::loadEncReverseX() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::reverseEncX = loadBool(settings, "encReverseX", false, OK);
}

bool ProgramParam::loadEncReverseY() {
    bool OK = true;
    QSettings settings(org, app);
    return CncParam::reverseEncY = loadBool(settings, "encReverseY", false, OK);
}

bool ProgramParam::loadFeedbackParam(bool& fb_ena, double& low_thld, double& high_thld, double& rb_to, unsigned& rb_attempts, double& rb_len, double& rb_speed) {
    bool OK = true;
    QSettings settings(org, app);
    fb_ena = CncParam::fb_ena = loadBool(settings, "feedbackEnable", false, OK);

    int thld_max = static_cast<int>( round(cnc_adc_volt_t::maxVolt(0)) );
    low_thld = CncParam::low_thld = loadDouble(settings, "lowThreshold", 0, thld_max, 0, OK);
    high_thld = CncParam::high_thld = loadDouble(settings, "highThreshold", 0, thld_max, thld_max, OK);

    if (low_thld > high_thld) {
        low_thld = CncParam::low_thld = 0;
        high_thld = CncParam::high_thld = thld_max;
        settings.setValue("lowThreshold", CncParam::low_thld);
        settings.setValue("highThreshold", CncParam::high_thld);
    }

    rb_to = CncParam::rb_to = loadDouble(settings, "rollbackTimeout", 0, CncParam::DEFAULT_RB_TO * 10, CncParam::DEFAULT_RB_TO, OK);
    rb_attempts = CncParam::rb_attempts = loadUInt(settings, "rollbackAttempts", 1, CncParam::DEFAULT_RB_ATTEMPTS * 10, CncParam::DEFAULT_RB_ATTEMPTS, OK);
    rb_len = CncParam::rb_len = loadDouble(settings, "rollbackLength", 0, CncParam::DEFAULT_RB_LEN * 10, CncParam::DEFAULT_RB_LEN, OK);
    rb_speed = CncParam::rb_speed = loadDouble(settings, "rollbackSpeed", 0, CncParam::DEFAULT_RB_SPEED * 10, CncParam::DEFAULT_RB_SPEED, OK);

    return OK;
}

bool ProgramParam::loadAcceleration(double &acc, double &dec) {
    bool OK = true;
    QSettings settings(org, app);
    acc = CncParam::acc = loadDouble(settings, "acceleration", CncParam::DEFAULT_ACC * 0.1, CncParam::DEFAULT_ACC * 10, CncParam::DEFAULT_ACC, OK);
    dec = CncParam::dec = loadDouble(settings, "deceleration", CncParam::DEFAULT_DEC * 0.1, CncParam::DEFAULT_DEC * 10, CncParam::DEFAULT_DEC, OK);
    return OK;
}

bool ProgramParam::loadStep(double &step, double &scaleX, double &scaleY, double &scaleU, double &scaleV, double &scaleEncX, double &scaleEncY, bool &encXY) {
    bool OK = true;
    QSettings settings(org, app);

    step = CncParam::step = loadDouble(settings, "step", CncParam::STEP_MIN, CncParam::STEP_MAX, CncParam::DEFAULT_STEP, OK);
    scaleX = CncParam::scaleX = loadDouble(settings, "scaleX", CncParam::SCALE_MIN, CncParam::SCALE_MAX, CncParam::DEFAULT_SCALE_XY, OK);
    scaleY = CncParam::scaleY = loadDouble(settings, "scaleY", CncParam::SCALE_MIN, CncParam::SCALE_MAX, CncParam::DEFAULT_SCALE_XY, OK);
    scaleU = CncParam::scaleU = loadDouble(settings, "scaleU", CncParam::SCALE_MIN, CncParam::SCALE_MAX, CncParam::DEFAULT_SCALE_UV, OK);
    scaleV = CncParam::scaleV = loadDouble(settings, "scaleV", CncParam::SCALE_MIN, CncParam::SCALE_MAX, CncParam::DEFAULT_SCALE_UV, OK);
    scaleEncX = CncParam::scaleEncX = loadDouble(settings, "scaleEncX", CncParam::SCALE_ENC_MIN, CncParam::SCALE_ENC_MAX, CncParam::DEFAULT_SCALE_ENC_XY, OK);
    scaleEncY = CncParam::scaleEncY = loadDouble(settings, "scaleEncY", CncParam::SCALE_ENC_MIN, CncParam::SCALE_ENC_MAX, CncParam::DEFAULT_SCALE_ENC_XY, OK);
    encXY = CncParam::encXY = loadBool(settings, "encXY", CncParam::DEFAULT_ENC_XY, OK);

    return OK;
}

void ProgramParam::loadParam() {
    bool OK = true, fb_ena;
    unsigned rb_attempts;
    double low_thld, high_thld, rb_to, rb_len, rb_speed, acc, dec;
    double step, scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY;
    bool encXY;

    loadInterfaceLanguage();
    loadFontSize();

    loadSwapXY();
    loadReverseX();
    loadReverseY();
    loadShowXY();

    loadInputLevel(OK);

    loadMotorReverseX();
    loadMotorReverseY();
    loadMotorReverseU();
    loadMotorReverseV();
    loadMotorSwapXY();
    loadMotorSwapUV();

    loadEncReverseX();
    loadEncReverseY();

    OK &= loadFeedbackParam(fb_ena, low_thld, high_thld, rb_to, rb_attempts, rb_len, rb_speed);
    OK &= loadAcceleration(acc, dec);
    OK &= loadStep(step, scaleX, scaleY, scaleU, scaleV, scaleEncX, scaleEncY, encXY);

    loadStepDir(OK);
}

//
void ProgramParam::saveSwapXY(bool value) {
    QSettings settings(org, app);
    swapXY = value;
    settings.setValue("swapXY", swapXY);
    settings.sync();
}

bool ProgramParam::loadSwapXY() {
    bool OK = true;
    QSettings settings(org, app);
    return swapXY = loadBool(settings, "swapXY", false, OK);
}

void ProgramParam::saveReverseX(bool value) {
    QSettings settings(org, app);
    reverseX = value;
    settings.setValue("reverseX", reverseX);
    settings.sync();
}

bool ProgramParam::loadReverseX() {
    bool OK = true;
    QSettings settings(org, app);
    return reverseX = loadBool(settings, "reverseX", false, OK);
}

void ProgramParam::saveReverseY(bool value) {
    QSettings settings(org, app);
    reverseY = value;
    settings.setValue("reverseY", reverseY);
    settings.sync();
}

bool ProgramParam::loadReverseY() {
    bool OK = true;
    QSettings settings(org, app);
    return reverseY = loadBool(settings, "reverseY", false, OK);
}

void ProgramParam::saveShowXY(bool value) {
    QSettings settings(org, app);
    showXY = value;
    settings.setValue("showXY", showXY);
    settings.sync();
}

bool ProgramParam::loadShowXY() {
    bool OK = true;
    QSettings settings(org, app);
    return showXY = loadBool(settings, "showXY", false, OK);
}

//enum BUTTON_T {START, REVERSE, CANCEL};
//enum CNC_STATES_T {UNCHECKED, IDLE, RUN, PAUSE, REVERSE, CANCEL, SHORT_REVERSE, ERROR};

void AppState::next(AppState::BUTTON btn) {
    switch (m_state) {
    case STATES::ST_NONE: case STATES::ST_IDLE:
        if (btn == BUTTON::SIG_START)
            m_state = STATES::ST_RUN;
        break;
    case STATES::ST_RUN:
        if (btn == BUTTON::SIG_START)
            m_state = STATES::ST_PAUSE;
        break;
    case STATES::ST_PAUSE:
        if (btn == BUTTON::SIG_START)
            m_state = STATES::ST_RUN;
        else if (btn == BUTTON::SIG_REVERSE)
            m_state = STATES::ST_REV;
        else if (btn == BUTTON::SIG_CANCEL)
            m_state = STATES::ST_CANCEL;
        break;
    case STATES::ST_REV:
        if (btn == BUTTON::SIG_START)
            m_state = STATES::ST_RUN;
        break;
    case STATES::ST_CANCEL:
        if (btn == BUTTON::SIG_REVERSE)
            m_state = STATES::ST_SHORT_REV;
        else if (btn == BUTTON::SIG_CANCEL)
            m_state = STATES::ST_NONE;
        break;
    case STATES::ST_SHORT_REV:
        if (btn == BUTTON::SIG_START)
            m_state = STATES::ST_CANCEL;
        break;
    case STATES::ST_ERROR:
        if (btn == BUTTON::SIG_CANCEL)
            m_state = STATES::ST_NONE;
        break;
    default:
        m_state = STATES::ST_ERROR;
        break;
    }
}
