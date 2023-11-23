#include "mainwindow.h"
#include "./ui_mainwindow.h"

double firstNum;
bool isTypingSecondNum = false;
QMap<QString, QPushButton*> actionButtons = {};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMap<QString, QPushButton*> numButtons = {
                                               {"0", ui->zeroButton},
                                               {"1", ui->oneButton},
                                               {"2", ui->twoButton},
                                               {"3", ui->threeButton},
                                               {"4", ui->fourButton},
                                               {"5", ui->fiveButton},
                                               {"6", ui->sixButton},
                                               {"7", ui->sevenButton},
                                               {"8", ui->eightButton},
                                               {"9", ui->nineButton},
                                               };

    actionButtons = {
                     {"+", ui->plusButton},
                     {"-", ui->minusButton},
                     {"*", ui->timesButton},
                     {"/", ui->divideButton},
                     };

    for (auto it = numButtons.begin(); it != numButtons.end(); ++it) {
        connect(it.value(), SIGNAL(released()), this, SLOT(numbutton_pressed()));
    }

    for (auto it = actionButtons.begin(); it != actionButtons.end(); ++it) {
        connect(it.value(), SIGNAL(released()), this, SLOT(actionbutton_pressed()));
        it.value()->setCheckable(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::numbutton_pressed()
{
    QPushButton * button = (QPushButton*)sender();

    double labelNumber;
    QString labelText;

    if ((ui->plusButton->isChecked() || ui->minusButton->isChecked()
         || ui->timesButton->isChecked() || ui->divideButton->isChecked()) && (!isTypingSecondNum))
    {
        labelNumber = button->text().toDouble();
        isTypingSecondNum = true;
    }
    else
    {
        labelNumber = (ui->label->text() + button->text()).toDouble();
    }

    labelText = QString::number(labelNumber, 'g', 8);

    ui->label->setText(labelText);
}

void MainWindow::actionbutton_pressed()
{
    QPushButton * button = (QPushButton*)sender();

    firstNum = ui->label->text().toDouble();

    for (auto it = actionButtons.begin(); it != actionButtons.end(); ++it) {
        if (it.value() == button) {
            it.value()->setChecked(true);
            break;
        }
    }
}

void MainWindow::on_commaButton_released()
{
    if (ui->label->text().contains('.'))
    {
        return;
    }
    else
    {
       ui->label->setText(ui->label->text()+'.');
    }
}

void MainWindow::on_clearButton_released()
{  
    ui->plusButton->setChecked(false);
    ui->minusButton->setChecked(false);
    ui->timesButton->setChecked(false);
    ui->divideButton->setChecked(false);

    isTypingSecondNum = false;

    ui->label->setText("0");
}

void MainWindow::on_equalsButton_released()
{
    double labelNumber, secondNum;
    QString newLabel;

    secondNum = ui->label->text().toDouble();

    for (auto it = actionButtons.begin(); it != actionButtons.end(); ++it) {
       if (it.value()->isChecked()) {
            switch (it.key().toStdString()[0]) {
            case '+':
                labelNumber = firstNum + secondNum;
                break;
            case '-':
                labelNumber = firstNum - secondNum;
                break;
            case '*':
                labelNumber = firstNum * secondNum;
                break;
            case '/':
                if (secondNum != 0) {
                    labelNumber = firstNum / secondNum;
                } else {
                    ui->label->setText("0");
                    return;
                }
                break;
            }

            newLabel = QString::number(labelNumber, 'g', 8);
            ui->label->setText(newLabel);
            it.value()->setChecked(false);
            break;
       }
    }

    isTypingSecondNum = false;
}
