/*
    Simple GUI application using the VM167 userspace driver.
    Copyright (C) 2015  Robert Ahlskog <robert.ahlskog@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QTimer>
#include "TestApp.h"
#include "ui_TestApp.h"
#include "VM167.h"

TestApp::TestApp(QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{
	ui = new Ui::TestApp();
	ui->setupUi(this);
	this->setFixedSize(this->geometry().width(),this->geometry().height());
	
	connect(ui->d_ch1, SIGNAL(clicked(bool)), this, SLOT(changeDigital1(bool)));
	connect(ui->d_ch2, SIGNAL(clicked(bool)), this, SLOT(changeDigital2(bool)));
	connect(ui->d_ch3, SIGNAL(clicked(bool)), this, SLOT(changeDigital3(bool)));
	connect(ui->d_ch4, SIGNAL(clicked(bool)), this, SLOT(changeDigital4(bool)));
	connect(ui->d_ch5, SIGNAL(clicked(bool)), this, SLOT(changeDigital5(bool)));
	connect(ui->d_ch6, SIGNAL(clicked(bool)), this, SLOT(changeDigital6(bool)));
	connect(ui->d_ch7, SIGNAL(clicked(bool)), this, SLOT(changeDigital7(bool)));
	connect(ui->d_ch8, SIGNAL(clicked(bool)), this, SLOT(changeDigital8(bool)));
	
	connect(ui->actionCard_1, SIGNAL(triggered()), this, SLOT(selectCardOne()));
	connect(ui->actionCard_2, SIGNAL(triggered()), this, SLOT(selectCardTwo()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	
	connect(ui->d1_output, SIGNAL(clicked(bool)), this, SLOT(switchIOMode()));
	connect(ui->d2_output, SIGNAL(clicked(bool)), this, SLOT(switchIOMode()));
	
	connect(ui->pwm_ch1, SIGNAL(valueChanged(int)), this, SLOT(setPwm1(int)));
	connect(ui->pwm_ch2, SIGNAL(valueChanged(int)), this, SLOT(setPwm2(int)));
	connect(ui->pwm_freq, SIGNAL(currentIndexChanged(int)), this, SLOT(pwmFreqChange(int)));
	
	device = -1;
	int devices = OpenDevices();
	
	if (devices == -1)
	{
		ui->actionCard_1->setEnabled(false);
		ui->actionCard_2->setEnabled(false);
	}
	else
	{
		ui->actionCard_1->setEnabled(devices & 1);
		ui->actionCard_2->setEnabled(devices & 2);
	}
	
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateInputs()));
	timer->start(500);
}

TestApp::~TestApp()
{
	CloseDevices();
}

void TestApp::updateInputs()
{
	if (device == -1)
		return;
	
	int digital = ReadAllDigital(device);
	if (!ui->d1_output->isChecked())
	{
		ui->d_ch1->setChecked(digital & 1);
		ui->d_ch2->setChecked(digital & 2);
		ui->d_ch3->setChecked(digital & 4);
		ui->d_ch4->setChecked(digital & 8);
	}
	if (!ui->d2_output->isChecked())
	{
		ui->d_ch5->setChecked(digital & 16);
		ui->d_ch6->setChecked(digital & 32);
		ui->d_ch7->setChecked(digital & 64);
		ui->d_ch8->setChecked(digital & 128);
	}
	int analog[5];
	ReadAllAnalog(device, analog);
	ui->a_ch1->setValue(analog[0]);
	ui->a_ch2->setValue(analog[1]);
	ui->a_ch3->setValue(analog[2]);
	ui->a_ch4->setValue(analog[3]);
	ui->a_ch5->setValue(analog[4]);
}

void TestApp::selectCardOne()
{
	selectCard(0);
}
void TestApp::selectCardTwo()
{
	selectCard(1);
}

void TestApp::selectCard(int card)
{
	device = card;
	ui->d1_output->setEnabled(true);
	ui->d2_output->setEnabled(true);
	
	ui->pwm_freq->setEnabled(true);
	ui->pwm_ch1->setEnabled(true);
	ui->pwm_ch2->setEnabled(true);
	
	ui->a_ch1->setEnabled(true);
	ui->a_ch2->setEnabled(true);
	ui->a_ch3->setEnabled(true);
	ui->a_ch4->setEnabled(true);
	ui->a_ch5->setEnabled(true);
}

void TestApp::switchIOMode()
{
	if (device == -1)
		return;
	
	bool d1 = ui->d1_output->isChecked();
	bool d2 = ui->d2_output->isChecked();
	
	ui->d_ch1->setEnabled(d1);
	ui->d_ch2->setEnabled(d1);
	ui->d_ch3->setEnabled(d1);
	ui->d_ch4->setEnabled(d1);
	
	ui->d_ch5->setEnabled(d2);
	ui->d_ch6->setEnabled(d2);
	ui->d_ch7->setEnabled(d2);
	ui->d_ch8->setEnabled(d2);
	
	int low = !d1;
	int high = !d2;
	InOutMode(device, high, low);
}

void TestApp::setPwm1(int value)
{
	if (device == -1)
		return;

	SetPWM(device, 1, value, ui->pwm_freq->currentIndex()+1);
}

void TestApp::setPwm2(int value)
{
	if (device == -1)
		return;
	
	SetPWM(device, 2, value, ui->pwm_freq->currentIndex()+1);
}

void TestApp::pwmFreqChange(int value)
{
	if (device == -1)
		return;
	
	setPwm1(ui->pwm_ch1->value());
	setPwm2(ui->pwm_ch2->value());
}

void TestApp::changeDigital1(bool value)
{
	setDigital(1, value);
}
void TestApp::changeDigital2(bool value)
{
	setDigital(2, value);
}
void TestApp::changeDigital3(bool value)
{
	setDigital(3, value);
}
void TestApp::changeDigital4(bool value)
{
	setDigital(4, value);
}
void TestApp::changeDigital5(bool value)
{
	setDigital(5, value);
}
void TestApp::changeDigital6(bool value)
{
	setDigital(6, value);
}
void TestApp::changeDigital7(bool value)
{
	setDigital(7, value);
}
void TestApp::changeDigital8(bool value)
{
	setDigital(8, value);
}

void TestApp::setDigital(int channel, bool value)
{
	if (device == -1)
		return;
	
	if (value)
		SetDigitalChannel(device, channel);
	else
		ClearDigitalChannel(device, channel);
}

