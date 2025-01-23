#include <iostream>
#include <iomanip>

namespace {
const char * credits = "===========================================================\n"
                       "Лабораторная работа №1 по дисциплине \"Программирование\"\n"
                       "Автор: Иванов Григорий Денисович\n"
                       "Группа: 4335 (подгруппа 1)\n"
                       "Версия: 2\n"
                       "Период выполнения работы: 10.01.2025 - 12.01.2025\n"
                       "===========================================================";

const char * description = "Описание:\n"
                           "В этой программе будет проведен расчет значения математического\n"
                           "выражения R(x) = F(x)/Q(x), где:\n"
                           " F(x) = -3.01x^7 + 4324249x^2 + 2987456x,\n"
                           " Q(x) = -21.98x^3 - 21.98x^2 - 21.98x\n"
                           "Замечание: значение X, введенное пользователем, должно находиться\n"
                           "в диапазоне [-1.7E40, 1.7E40], деление на 0 не обрабатывается\n"
                           "При выводе текста в консоль используется библиотека <iostream>";

void printFloat(double number)
{
    bool negative = number < 0;
    std::cout << (negative ? "-" : " ");
    std::cout << std::setw(30) << std::setprecision(15)
              << std::setfill('0') << std::setiosflags(std::ios::right | std::ios::fixed)
              << (negative ? -number : number);
}
}

int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;

    double tmp, x, res, x_pow5;
    const double f7 = -3.01, f2 = 4324249, f1 = 2987456;
    const double q3 = -21.98, q2 = -21.98, q1 = -21.98;

    std::cout << credits << std::endl;
    std::cout << description << std::endl << std::endl;

    std::cout << "Введите x:";
    std::cin >> x;

    tmp = x*x;
    tmp *= x;
    tmp *= x;
    tmp *= x;
    x_pow5 = tmp;

    std::cout << "Промежуточные вычисления:\nx^5=";
    printFloat(x_pow5);

    tmp = f7*x_pow5 + f2;
    std::cout << "\nf1(x)=";
    printFloat(tmp);

    tmp = tmp * x + f1;
    std::cout << "\nf2(x)=";
    printFloat(tmp);

    tmp = tmp * x;
    std::cout << "\nf3(x)=";
    printFloat(tmp);
    res = tmp;

    tmp = q3*x + q2;
    std::cout << "\nq1(x)=";
    printFloat(tmp);

    tmp = tmp * x + q1;
    std::cout << "\nq2(x)=";
    printFloat(tmp);

    tmp *= x;
    std::cout << "\nq3(x)=";
    printFloat(tmp);

    res /= tmp;

    std::cout << "\nРезультат:\nR(x)=";
    printFloat(res);
    std::cout << "\nЗавершение программы" << std::endl;

    return 0;
}
