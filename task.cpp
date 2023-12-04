#include "task.h"

void Task::RK46(double& x, double& v, double& h)
{
    double k1 = var6(x, v);
    double k2 = var6(x + h * 0.5, v + h * 0.5 * k1);
    double k3 = var6(x + h * 0.5, v + h * 0.5 * k2);
    double k4 = var6(x + h , v + h * k3);
    v += h / 6. * (k1 + 2. * k2 + 2. * k3 + k4);
    x += h;
}

void Task::RK45(double& x, double& v, double& h)
{
    double k1 = var5(x, v);
    double k2 = var5(x + h * 0.5, v + h * 0.5 * k1);
    double k3 = var5(x + h * 0.5, v + h * 0.5 * k2);
    double k4 = var5(x + h , v + h * k3);
    v += h / 6. * (k1 + 2. * k2 + 2. * k3 + k4);
    x += h;
}

double Task::var6(double x, double u)
{
    return (E0 * sin(w * x) - R * u) / L;
}

double Task::var5(double x, double u)
{
    return (V - R * u) / L;
}

Task::Task(const std::vector<double>& param)
{
    x0 = param[0];
    X = param[1];
    u0 = param[2];
    h0 = param[3];
    Nmax = static_cast<int>(param[4]);
    Eps = param[5];
    Epsmin = param[6];
    Epsmax = param[7];
    L = param[8];
    R = param[9];
    V = param[12];
    E0 = param[10];
    w = param[11];
}

void Task::nextDot6(QLineSeries*& series, QTableWidget*& table)
{
    double h = (X - x0) / Nmax;
    double v = u0;
    double x = x0;
    table->setRowCount(Nmax);

    *series << QPointF(x, v);
    table->setItem(0, 0, new QTableWidgetItem(QString::number(0)));
    table->setItem(0, 1, new QTableWidgetItem(QString::number(x)));
    table->setItem(0, 2, new QTableWidgetItem(QString::number(v)));

    for (int i = 0; i < Nmax; i++)
    {
        RK46(x, v, h);

        *series << QPointF(x, v);
        table->setItem(i + 1, 0, new QTableWidgetItem(QString::number(h)));
        table->setItem(i + 1, 1, new QTableWidgetItem(QString::number(x)));
        table->setItem(i + 1, 2, new QTableWidgetItem(QString::number(v)));
    }
}

void Task::nextDot6C(QLineSeries *&, QTableWidget *&)
{

}

void Task::nextDot5(QLineSeries *& series)
{
    double h = (X - x0) / Nmax;
    double v = u0;
    double x = x0;

    *series << QPointF(x, v);

    for (int i = 0; i < Nmax; i++)
    {
        RK45(x, v, h);

        *series << QPointF(x, v);
    }
}

void Task::nextDot5C(QLineSeries *&)
{

}
