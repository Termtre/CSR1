#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Фомичев Дмитрий 3821Б1ПМмм1");
    ui->tabWidget->setTabText(0, "Значения с контролем");
    ui->tabWidget->setTabText(1, "Результаты вычислений");
    ui->tabWidget->setTabText(2, "Значения без контроля");
    ui->tabWidget_2->setTabText(0, "Постановка задачи");
    ui->tabWidget_2->setTabText(1, "Уравнение 5 варианта");
    ui->tabWidget_2->setTabText(2, "Метод Рунге-Кутта");
    ui->tabWidget_3->setTabText(0, "Параметры");
    ui->tabWidget_3->setTabText(1, "Параметры задачи");

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget_2->setCurrentIndex(0);
    ui->tabWidget_3->setCurrentIndex(0);

    tableColumn << "n" << "h_n-1" << "xn" << "vn" << "vn удв" << "ОЛП" << "vn итог" << "Ум. шага" << "Ув. шага";
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels(tableColumn);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->setHorizontalHeaderLabels(tableColumn);
    //ui->tableWidget->setRowCount(1);
    //ui->tableWidget->setVerticalHeaderLabels(QStringList(QString::number(ui->tableWidget->rowCount() - 1)));
    ui->textEdit->setText("Справка");
    ui->textEdit->append("U0 = ");
    ui->textEdit->append("Последний v:     Последний x:");
    ui->textEdit->append("Число итераций: ");
    ui->textEdit->append("Число удвоений: ");
    ui->textEdit->append("Число делений: ");
    ui->textEdit->append("Максимальный шаг: ");
    ui->textEdit->append("Минимальный шаг: ");
    ui->textEdit->append("Максимальная ошибка: ");

    chart = new QChart();
    chart->legend()->setVisible(true);
    chart->setTitle("Контрольная работа. Фомичев Д.Е. ПМмм1 Вариант 6");

    axisX = new QValueAxis();
    axisX->setLabelFormat("%f");
    axisX->setTickCount(10);
    axisX->setTitleText("x, время");
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(10);
    axisY->setTitleText("I, сила тока");
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    if (var6Series || var6SeriesC || var5Series)
    {
        chart->removeAllSeries();
        ui->graphicsView->zoomIt(false);
        var6Series = nullptr;
        var5Series = nullptr;
        var6SeriesC = nullptr;
    }
}


void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit9->text().toDouble() == 0.)
    {
        QMessageBox msgBox;
        msgBox.setText("Коэффициент самоиндукции не должно быть равно нулю");
        msgBox.exec();
        return;
    }
    ui->graphicsView->zoomIt(true);
    std::vector<double> param = { ui->lineEdit1->text().toDouble(), ui->lineEdit2->text().toDouble(),
                                  ui->lineEdit3->text().toDouble(), ui->lineEdit4->text().toDouble(),
                                  ui->lineEdit5->text().toDouble(), ui->lineEdit6->text().toDouble(),
                                  ui->lineEdit7->text().toDouble(),
                                  ui->lineEdit9->text().toDouble(), ui->lineEdit10->text().toDouble(),
                                  ui->lineEdit11->text().toDouble(), ui->lineEdit12->text().toDouble(),
                                  ui->lineEdit13->text().toDouble()};


    var6Series = new QLineSeries();
    var6Series->setName("Вариант 6");

    var6SeriesC = new QLineSeries();
    var6SeriesC->setName("Вариант 6 с контролем");

    //var6Series->setUseOpenGL(true);
    var5Series = new QLineSeries();
    var5Series->setName("Вариант 5");

    //var5Series->setUseOpenGL(true);

    Task* tsk1 = new Task(param);

    tsk1->nextDot6(var6Series, ui->tableWidget_2);
    tsk1->nextDot6C(var6SeriesC, ui->tableWidget);
    tsk1->nextDot5(var5Series);

    if (ui->checkBox->isChecked())
    {
        var6Series->setPointsVisible(true);
        var6SeriesC->setPointsVisible(true);
        var5Series->setPointsVisible(true);
    }
    else
    {
        var6SeriesC->setPointsVisible(false);
        var6Series->setPointsVisible(false);
        var5Series->setPointsVisible(false);
    }

    chart->addSeries(var6SeriesC);
    chart->addSeries(var6Series);
    chart->addSeries(var5Series);

    var6Series->attachAxis(axisX);
    var6Series->attachAxis(axisY);

    var6SeriesC->attachAxis(axisX);
    var6SeriesC->attachAxis(axisY);

    var5Series->attachAxis(axisX);
    var5Series->attachAxis(axisY);

    ui->graphicsView->setChart(chart);

    if (ui->checkBox_2->isChecked())
    {
        var6Series->setVisible(true);
    }
    else
    {
        var6Series->setVisible(false);
    }

    if (ui->checkBox_3->isChecked())
    {
        var6SeriesC->setVisible(true);
    }
    else
    {
        var6SeriesC->setVisible(false);
    }

    if (ui->checkBox_4->isChecked())
    {
        var5Series->setVisible(true);
    }
    else
    {
        var5Series->setVisible(false);
    }

    int udv = 0, umn = 0;
    int maxi = 0;
    int mini = 0;
    int maxit = 0;
    double maxh = ui->lineEdit4->text().toDouble();
    double minh = ui->lineEdit4->text().toDouble();
    double maxe = 0.;

    for (int i = 1; i < ui->tableWidget->rowCount(); i++)
    {
        udv += ui->tableWidget->item(i, 8)->text().toInt();
        umn += ui->tableWidget->item(i, 8)->text().toInt();

        if (maxh < ui->tableWidget->item(i, 1)->text().toDouble())
        {
            maxh = ui->tableWidget->item(i, 1)->text().toDouble();
            maxi = i;
        }

        if (minh > ui->tableWidget->item(i, 1)->text().toDouble())
        {
            minh = ui->tableWidget->item(i, 1)->text().toDouble();
            mini = i;
        }

        if (abs(maxe) < abs(ui->tableWidget->item(i, 5)->text().toDouble()))
        {
            maxe = ui->tableWidget->item(i, 5)->text().toDouble();
            maxit = i;
        }
    }

    ui->textEdit->clear();
    ui->textEdit->setText("Справка");
    ui->textEdit->append("Последний v: " + ui->tableWidget->item(ui->tableWidget->rowCount() - 1, 3)->text() +
                         "    Последний x: " + ui->tableWidget->item(ui->tableWidget->rowCount() - 1, 2)->text());
    ui->textEdit->append("Число итераций: " + QString().number(ui->tableWidget->rowCount() - 1));
    ui->textEdit->append("Число удвоений: " + QString().number(udv));
    ui->textEdit->append("Число делений: " + QString().number(umn));
    ui->textEdit->append("Максимальный шаг: " + QString().number(maxh) + " на " + QString().number(maxi) + " итерации");
    ui->textEdit->append("Минимальный шаг: " + QString().number(minh) + " на " + QString().number(mini) + " итерации");
    ui->textEdit->append("Максимальная ошибка: " + QString().number(maxe) + " на " + QString().number(maxit) + " итерации");

    delete tsk1;
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if (checked)
    {
        if (var6Series)var6Series->setPointsVisible(true);
        if (var6SeriesC)var6SeriesC->setPointsVisible(true);
        if (var5Series)var5Series->setPointsVisible(true);
    }
    else
    {
        if (var6SeriesC)var6SeriesC->setPointsVisible(false);
        if (var6Series)var6Series->setPointsVisible(false);
        if (var5Series)var5Series->setPointsVisible(false);
    }
}


void MainWindow::on_checkBox_2_clicked(bool checked)
{
    if (var6Series)
    {
        if (checked)
        {
            var6Series->setVisible(true);
        }
        else
        {
            var6Series->setVisible(false);
        }
    }
}


void MainWindow::on_checkBox_3_clicked(bool checked)
{
    if (var6SeriesC)
    {
        if (checked)
        {
            var6SeriesC->setVisible(true);
        }
        else
        {
            var6SeriesC->setVisible(false);
        }
    }
}


void MainWindow::on_checkBox_4_clicked(bool checked)
{
    if (var5Series)
    {
        if (checked)
        {
            var5Series->setVisible(true);
        }
        else
        {
            var5Series->setVisible(false);
        }
    }
}

