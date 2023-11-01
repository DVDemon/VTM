#include "formcalculator.h"
#include "ui_formcalculator.h"
#include "screentools.h"

FormCalculator::FormCalculator(std::shared_ptr<IVMTMachine> machine, IVMTEnvironment * environment, QWidget * parent) :
	QWidget(parent),
	_machine(machine),
	_environment(environment),
	ui(new Ui::FormCalculator) {
	ui->setupUi(this);

	ScreenTools st;

	st.ResizeButton(ui->_plus);
	st.ResizeButton(ui->_minus);

	QString str;
	str = QString::number(machine->GetPower());
	ui->text_power->setText(str);
}

void FormCalculator::Check() {
	QString str = ui->text_power->text();
	if (str.length() < 1) {
		str = "1";
		ui->text_power->setText(str);
	}
	else {

		bool ok = true;
		for (int i = 0;i < str.length();i++)
			if ((str[i] < '0') || (str[i] > '9')) {
				str.remove(i, 1);
				ok = false;
			}
		if (!ok) ui->text_power->setText(str);
	}

}

FormCalculator::~FormCalculator() {
	delete ui;
}



void FormCalculator::on_text_power_textChanged(const QString &) {
	Check();

	QString str = ui->text_power->text();
	bool ok;
	int power = str.toInt(&ok);
	if (ok) {
		_machine->SetPower(power);
		_machine->Update(_environment);
		QRect rect;
		if (_environment)  _environment->Repaint(rect);
		// VMTProject::GetInstance().GetUndoManager()->Remember(
		 //            std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));
	}
}

void FormCalculator::on_button_plus_clicked() {


}

void FormCalculator::on_button_minus_clicked() {


}

void FormCalculator::on__plus_clicked() {
	QString str = ui->text_power->text();
	bool ok;
	int power = str.toInt(&ok);
	if (ok) {
		str = QString::number(power + 1);
		ui->text_power->setText(str);
	}
}

void FormCalculator::on__minus_clicked() {
	QString str = ui->text_power->text();
	bool ok;
	int power = str.toInt(&ok);
	if (ok) {
		if (power > 1) {
			str = QString::number(power - 1);
			ui->text_power->setText(str);
		}
	}
}
