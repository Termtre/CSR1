#include "task.h"

void Task::RK46(double& x, double& v, double& h)
{
    double k1 = var6(x, v);
    double k2 = var6(x + h * 0.5, v + h * 0.5 * k1);
    double k3 = var6(x + h * 0.5, v + h * 0.5 * k2);
    double k4 = var6(x + h , v + h * k3);
    v = v + h / 6. * (k1 + 2. * k2 + 2. * k3 + k4);
    x = x + h;
}

void Task::RK45(double& x, double& v, double& h)
{
    double k1 = var5(x, v);
    double k2 = var5(x + h * 0.5, v + h * 0.5 * k1);
    double k3 = var5(x + h * 0.5, v + h * 0.5 * k2);
    double k4 = var5(x + h , v + h * k3);
    v = v + h / 6. * (k1 + 2. * k2 + 2. * k3 + k4);
    x = x + h;
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
    L = param[7];
    R = param[8];
    V = param[11];
    E0 = param[9];
    w = param[10];
}

void Task::nextDot6(QLineSeries*& series, QTableWidget*& table)
{
    double N = abs(X - x0) / h0;
    double v = u0;
    double x = x0;

    if (N > Nmax) N = Nmax;

    table->setRowCount(N + 1);

    *series << QPointF(x, v);
    table->setItem(0, 0, new QTableWidgetItem(QString::number(0)));
    table->setItem(0, 1, new QTableWidgetItem(QString::number(0)));
    table->setItem(0, 2, new QTableWidgetItem(QString::number(x)));
    table->setItem(0, 3, new QTableWidgetItem(QString::number(v)));

    for (int i = 0; i < N; i++)
    {
        RK46(x, v, h0);

        *series << QPointF(x, v);
        table->setItem(i + 1, 0, new QTableWidgetItem(QString::number(i + 1)));
        table->setItem(i + 1, 1, new QTableWidgetItem(QString::number(h0)));
        table->setItem(i + 1, 2, new QTableWidgetItem(QString::number(x)));
        table->setItem(i + 1, 3, new QTableWidgetItem(QString::number(v)));
    }
}

void Task::nextDot6C(QLineSeries*& series, QTableWidget*& table)
{
    int C1 = 0;
    int C2 = 0;
    double v = u0;
    double v2;
    double v3;
    double x = x0;
    double x_old;
    double v_old;
    double h_old = h0;
    double h = h0;
    double S;
    double S1;

    *series << QPointF(x, v);
    table->setRowCount(1);
    table->setItem(0, 0, new QTableWidgetItem(QString::number(0)));
    table->setItem(0, 1, new QTableWidgetItem(QString::number(0)));
    table->setItem(0, 2, new QTableWidgetItem(QString::number(x)));
    table->setItem(0, 3, new QTableWidgetItem(QString::number(v)));

    for (int i = 1; i < Nmax; i++)
    {
        table->setRowCount(i + 1);
        x_old = x;
        v_old = v;
        h_old = h;

        while(1)
        {
            x = x_old;
            v2 = v_old;
            v = v_old;
            h = h_old;

            RK46(x, v, h);
            x = x_old;
            h /= 2.;
            RK46(x, v2, h);
            RK46(x, v2, h);
            S = (v2 - v) / 15.;

            if (abs(S) < Eps / 32.)
            {
                v3 = v;
                h = h_old * 2.;
                table->setItem(i, 1, new QTableWidgetItem(QString::number(h_old)));
                C2++;
                break;
            }
            else if (Eps / 32. <= abs(S) && abs(S) <= Eps)
            {
                v3 = v;
                h = h_old;
                table->setItem(i, 1, new QTableWidgetItem(QString::number(h)));
                break;
            }
            else
            {
                h_old /= 2.;
                C1++;
            }
        }

        if (x <= (X + Epsmin))
        {
            S1 = S * 16.;

            table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            table->setItem(i, 2, new QTableWidgetItem(QString::number(x)));
            table->setItem(i, 3, new QTableWidgetItem(QString::number(v)));
            table->setItem(i, 4, new QTableWidgetItem(QString::number(v2)));
            table->setItem(i, 5, new QTableWidgetItem(QString::number(S1)));
            table->setItem(i, 6, new QTableWidgetItem(QString::number(v3)));
            table->setItem(i, 7, new QTableWidgetItem(QString::number(C1)));
            table->setItem(i, 8, new QTableWidgetItem(QString::number(C2)));
            *series << QPointF(x, v3);
            C1 = 0;
            C2 = 0;

            if (X <= x && x <= (X + Epsmin)) break;
        }
        else
        {
            x = x_old;
            h_old = (X - x + Epsmin);
            h = h_old;
            v = v_old;

            RK46(x, v, h);
            x = x_old;
            v2 = v_old;
            h = h_old / 2;
            RK46(x, v2, h);
            RK46(x, v2, h);

            S = (v2 - v) / 15.;

            S1 = S * 16.;

            table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            table->setItem(i, 1, new QTableWidgetItem(QString::number(h_old)));
            table->setItem(i, 2, new QTableWidgetItem(QString::number(x)));
            table->setItem(i, 3, new QTableWidgetItem(QString::number(v)));
            table->setItem(i, 4, new QTableWidgetItem(QString::number(v2)));
            table->setItem(i, 5, new QTableWidgetItem(QString::number(S1)));
            table->setItem(i, 6, new QTableWidgetItem(QString::number(v3)));
            table->setItem(i, 7, new QTableWidgetItem(QString::number(C1)));
            table->setItem(i, 8, new QTableWidgetItem(QString::number(C2)));
            *series << QPointF(x, v);
            break;
        }
    }
}

void Task::nextDot5(QLineSeries *& series)
{
    double h = h0;
    double v = u0;
    double x = x0;
    int N = abs(X - x) / h;
    if ( N > Nmax) N = Nmax;

    *series << QPointF(x, v);

    for (int i = 0; i < N; i++)
    {
        RK45(x, v, h);

        *series << QPointF(x, v);
    }
}
