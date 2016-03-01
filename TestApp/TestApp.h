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


#ifndef TESTAPP_H
#define TESTAPP_H

#include <QtGui/QMainWindow>

namespace Ui { class TestApp; }
class TestApp : public QMainWindow
{
	Q_OBJECT
public:
	explicit TestApp(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~TestApp();

public slots:
	void updateInputs();
	void selectCardOne();
	void selectCardTwo();
	
	void selectCard(int card);
	void switchIOMode();
	void setPwm1(int value);
	void setPwm2(int value);
	void pwmFreqChange();
	void changeDigital1(bool value);
	void changeDigital2(bool value);
	void changeDigital3(bool value);
	void changeDigital4(bool value);
	void changeDigital5(bool value);
	void changeDigital6(bool value);
	void changeDigital7(bool value);
	void changeDigital8(bool value);
	
private:
	void setDigital(int channel, bool value);
	
	Ui::TestApp *ui;
	int device;
};

#endif // TESTAPP_H
