#ifndef TASK_H
#define TASK_H

#include <vector>
#include <cmath>
#include <QLineSeries>
#include <QTableWidget>

class Task
{
private:
    double x0;
    double X;
    double u0;
    double h0;
    int Nmax;
    double Eps;
    double Epsmin;
    double Epsmax;
    double L, R, E0, w;
    double V;

private:
    void RK46(double&, double&, double&);
    void RK45(double&, double&, double&);
    double var6(double, double);
    double var5(double, double);

public:
    Task(const std::vector<double>&);
    void nextDot6(QLineSeries*&, QTableWidget*&);
    void nextDot6C(QLineSeries*&, QTableWidget*&);
    void nextDot5(QLineSeries*&);
    void nextDot5C(QLineSeries*&);

};

#endif // TASK_H
