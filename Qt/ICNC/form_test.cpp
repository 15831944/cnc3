#include "form_test.h"
#include "ui_formtest.h"

#include <QDebug>
#include <string>
#include <iostream>
#include <sstream>
#include <QFile>

#include "cnc.h"
#include "aux_items.h"

using namespace std;
using namespace auxItems;

FormTest::FormTest(ProgramParam &par, QWidget *parent): QWidget(parent), ui(new Ui::FormTest), par(par), m_report(ui->txtMsg) {
    this->setObjectName(tr("Diagnostics"));
    ui->setupUi(this);
}

FormTest::~FormTest() {
    if (ui) {
        delete ui;
        ui = nullptr;
    }
}

void FormTest::on_btnHome_clicked() {
    emit homePageClicked();
}

void FormTest::on_btnQuickTest_clicked() {
    m_report.clear();
    par.cnc.bindReporter(&m_report);

    string s;

    if (par.cnc.isOpen()) {
        try {
            s = par.cnc.readVersion();
            m_report.write(s);
        } catch (string& s) {
            qDebug("%s\n", s.c_str());
        } catch (exception& e) {
            qDebug("Exception: %s\n", e.what());
        } catch (...) {
            qDebug("Unknown exception\n");
        }
    }
    else
        m_report.write("No CNC connection");
}

void FormTest::on_btnFullTest_clicked() {
    m_report.clear();
    par.cnc.bindReporter(&m_report);

    string s;

    if (par.cnc.isOpen()) {
        try {
            s = par.cnc.readVersion();
            m_report.write(s);

            if (par.cnc.testRegs())
                ui->txtMsg->append("Info: RW registor OK");
            else
                ui->txtMsg->append("Error: RW registor error");

            if (par.cnc.testProgArray())
                ui->txtMsg->append("Info: RW program memory OK");
            else
                ui->txtMsg->append("Error: RW program memory error");

            if (par.cnc.testFpga())
                ui->txtMsg->append("Info: FPGA test OK");
            else
                ui->txtMsg->append("Error: FPGA test error");
        } catch (string& s) {
            qDebug("%s\n", s.c_str());
        } catch (exception& e) {
            qDebug("Exception: %s\n", e.what());
        } catch (...) {
            qDebug("Unknown exception\n");
            throw;
        }
    }
    else
        m_report.write("No CNC connection");
}

void FormTest::on_btnGCode_clicked() {
    emit editPageClicked();
}

void FormTest::on_btnImit_clicked() {
    m_report.clear();
    par.cnc.bindReporter(&m_report);

    if (par.gcodeText.length() == 0) {
        m_report.write(tr("Error: No G-code program") + "\n");
        return;
    }

    list<string> gframes = par.gcode.toFrameList();
    if (gframes.empty()) {
        m_report.write(tr("Error: No G-code program") + "\n");
        return;
    }

    try {
        string s = par.cnc.readVersion();
        m_report.write(s);

        if (par.cnc.testRegs())
            ui->txtMsg->append("Info: RW registor OK");
        else {
            ui->txtMsg->append("Error: RW registor error");
            return;
        }

        par.cnc.write(gframes);
        list<string> read_gframes = par.cnc.read();

        string error_text;
        bool OK = GCode::CompareFrames(gframes, read_gframes, error_text);

        if (OK) {
            string txt = GCode::toText(read_gframes);
            ui->txtMsg->append(QString(txt.c_str()));
        }
        else {
            ui->txtMsg->append(QString(error_text.c_str()));
            return;
        }

        if (OK) {
            par.cnc.imitEna(true);
            par.cnc.runReq();
            par.cnc.readImitFifo();
            par.cnc.saveImitData(job_dir + "motor_xy.dat");

//            system(("python " + job_dir + "test_plot1.py").c_str());
            int res = system("python c:\\Work\\Meatec\\Prototype\\NC\\test_plot1.py");
            qDebug() << res;
//            FILE* in = popen(("python " + job_dir + "test_plot1.py").c_str(), "r");
//            FILE* in = popen("python c:\\Work\\Meatec\\Prototype\\NC\\test_plot1.py", "r");
//            pclose(in);
        }
    } catch (string& s) {
        qDebug("%s\n", s.c_str());
    } catch (exception& e) {
        qDebug("Exception: %s\n", e.what());
    } catch (...) {
        qDebug("Unknown exception\n");
        throw;
    }
}

void FormTest::on_btnReadGCode_clicked() {
    list<string> read_gframes = par.cnc.read();
    string txt = GCode::toText(read_gframes);

    if (txt.empty())
        ui->txtMsg->setPlainText("No G-code program in CNC\n");
    else
        ui->txtMsg->setPlainText(QString(txt.c_str()));

    QFile file("readGcode.log");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(txt.c_str());
        file.close();
    }
}

void FormTest::on_btnMemTest_clicked() {
    bool OK;
    std::vector<uint8_t> bytes;
    QElapsedTimer timer;

    clear();

    writeln("Write/Read CNC memory test");
    writeln("**************************");
    size_t pa_size = par.cnc.readUInt32(ADDR::PA_SIZE, 0, &OK);
    writeln(QString::asprintf("Program array size: %d bytes\n", (int)pa_size));

    if (!pa_size) {
        writeln("Program array size error\n");
        return;
    }

    writeln("Write a line code\n");

    bytes.resize(pa_size);
    for (int i = 0; i < (int)(pa_size >> 2); i++) {
        uint32_t data = pa_size - i;
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&data);
        bytes[(i<<2)] = ptr[0];
        bytes[(i<<2) + 1] = ptr[1];
        bytes[(i<<2) + 2] = ptr[2];
        bytes[(i<<2) + 3] = ptr[3];
    }

    par.cnc.clearReadPort(); // don't necessary

    timer.start();
    OK = par.cnc.writeProgArrayFast(bytes); // todo to async
    int t = timer.elapsed();

    double wr_speed = bytes.size() * 1e3 / t;

    par.cnc.clearReadPort(); // don't necessary

    if (OK)
        writeln(QString::asprintf("Elapsed time: %g sec. Write speed: %g kB/s\n", t / 1e3, wr_speed / 1024.0));
    else {
        writeln("Write error\n");
        return;
    }

//    writeln("Read");

//    timer.start();
//    vector<uint8_t> rddata = par.cnc.readProgArray();
//    t = timer.elapsed();

//    double rd_speed = rddata.size() * 1e3 / t;

//    par.cnc.clearReadPort(); // don't necessary

//    writeln(QString::asprintf("Read: %d bytes, Elapsed time: %g sec., Read speed: %g kB/s\n", (int)rddata.size(), t / 1e3, rd_speed / 1024.0));

//    if (bytes.size() == rddata.size()) {
//        for (int i = 0; i < (int)rddata.size(); i++)
//            if (bytes[i] != rddata[i]) {
//                writeln(QString::asprintf("Read data error at %d byte", i));
//                break;
//            }
//    } else {
//        writeln("Read Program Array size error");
//    }

//    writeln("Program memory test completed");
}

void FormTest::clear() {
    ui->txtMsg->clear();
    ui->txtMsg->repaint();
}

void FormTest::writeln(const QString &s){
    ui->txtMsg->append(s + "\n");
    ui->txtMsg->repaint();
}

void FormTest::on_btnConn_clicked() {
    par.cnc.reconnect();
}
