#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CSR1");
    ui->tabWidget->setTabText(0, "Таблица значений без контроля");
    ui->tabWidget->setTabText(1, "Результаты вычислений");
    ui->tabWidget->setTabText(2, "Таблица с контролем");
    ui->tabWidget_2->setTabText(0, "Постановка задачи");
    ui->tabWidget_2->setTabText(1, "Уравнение 5 варианта");
    ui->tabWidget_2->setTabText(2, "Метод Рунге-Кутта");
    ui->tabWidget_3->setTabText(0, "Параметры");
    ui->tabWidget_3->setTabText(1, "Параметры 6 варианта");
    ui->tabWidget_3->setTabText(2, "Параметры 5 варианта");
    tableColumn << "h_n-1" << "xn" << "vn" << "vn удв" << "S*" << "vn итог" << "Ум. шага" << "Ув. шага";
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(tableColumn);
    //ui->tableWidget->setRowCount(1);
    //ui->tableWidget->setVerticalHeaderLabels(QStringList(QString::number(ui->tableWidget->rowCount() - 1)));
    ui->textEdit->setText("Справка");
    ui->textEdit->append("U0 = ");
    ui->textEdit->append("N = ");
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
    axisX->setTitleText("x");
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(10);
    axisY->setTitleText("u");
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    if (var6Series || var6SeriesC || var5Series || var5SeriesC)
    {
        chart->removeAllSeries();
        ui->graphicsView->zoomIt(false);
        var6Series = nullptr;
        var5Series = nullptr;
        var6SeriesC = nullptr;
        var5SeriesC = nullptr;
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->zoomIt(true);
    std::vector<double> param = { ui->lineEdit1->text().toDouble(), ui->lineEdit2->text().toDouble(),
                                  ui->lineEdit3->text().toDouble(), ui->lineEdit4->text().toDouble(),
                                  ui->lineEdit5->text().toDouble(), ui->lineEdit6->text().toDouble(),
                                  ui->lineEdit7->text().toDouble(), ui->lineEdit8->text().toDouble(),
                                  ui->lineEdit9->text().toDouble(), ui->lineEdit10->text().toDouble(),
                                  ui->lineEdit11->text().toDouble(), ui->lineEdit12->text().toDouble(),
                                  ui->lineEdit13->text().toDouble()};


    var6Series = new QLineSeries();
    var6Series->setName("Вариант 6");
    //var6Series->setUseOpenGL(true);
    var5Series = new QLineSeries();
    var5Series->setName("Вариант 5");
    //var5Series->setUseOpenGL(true);
    Task* tsk1 = new Task(param);

    tsk1->nextDot6(var6Series, ui->tableWidget);
    tsk1->nextDot5(var5Series);
    if (ui->checkBox->isChecked())
    {

        var6Series->setPointsVisible(true);
        var5Series->setPointsVisible(true);
    }
    else
    {
        var6Series->setPointsVisible(false);
        var5Series->setPointsVisible(false);
    }
    chart->addSeries(var6Series);
    chart->addSeries(var5Series);
    var6Series->attachAxis(axisX);
    var6Series->attachAxis(axisY);
    var5Series->attachAxis(axisX);
    var5Series->attachAxis(axisY);
    ui->graphicsView->setChart(chart);

    var6Series->setVisible(false);
    var5Series->setVisible(false);
    if (ui->comboBox->currentIndex() == 0) var6Series->setVisible(true);
    else if (ui->comboBox->currentIndex() == 1) var6Series->setVisible(false);
    else if (ui->comboBox->currentIndex() == 2) var5Series->setVisible(true);
    else if (ui->comboBox->currentIndex() == 3) var5Series->setVisible(false);
    else if (ui->comboBox->currentIndex() == 4)
    {
        var6Series->setVisible(true);
        var5Series->setVisible(true);
    }
    else if (ui->comboBox->currentIndex() == 5)
    {
        var6Series->setVisible(false);
        var5Series->setVisible(false);
    }
    else if (ui->comboBox->currentIndex() == 6)
    {
        var6Series->setVisible(true);
        var5Series->setVisible(true);
    }

    delete tsk1;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (var5Series)
    {
        if (index == 0)
        {
            var5Series->setVisible(false);
            var6Series->setVisible(true);
        }
        else if (index == 1)
        {
            var5Series->setVisible(false);
            var6Series->setVisible(false);
        }
        else if (index == 2)
        {
            var5Series->setVisible(true);
            var6Series->setVisible(false);
        }
        else if (index == 3)
        {
            var5Series->setVisible(false);
            var6Series->setVisible(false);
        }
        else if (index == 4)
        {
            var6Series->setVisible(true);
            var5Series->setVisible(true);
        }
        else if (index == 5)
        {
            var6Series->setVisible(false);
            var5Series->setVisible(false);
        }
        else if (index == 6)
        {
            var6Series->setVisible(true);
            var5Series->setVisible(true);
        }
    }
}
