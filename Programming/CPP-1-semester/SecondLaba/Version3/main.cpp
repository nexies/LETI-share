#include <iostream>
#include <iomanip>

namespace {
const char credits [] = "===========================================================\n"
                       "Лабораторная работа №2 по дисциплине \"Программирование\"\n"
                       "Автор: Иванов Григорий Денисович\n"
                       "Группа: 4335 (подгруппа 1)\n"
                       "Версия: 3\n"
                       "Период выполнения работы: 16.01.2025 - 17.01.2025\n"
                       "===========================================================";

const char description [] = "Описание:\n"
                           "В этой программе будет проведен расчёт частичной суммы\n"
                           "S_n(x) = SUM (u_1(x), u_2(x), ... u_n(x)), номер последнего слагаемого\n"
                           "определяется в соответствии со значением числа e>0.\n"
                           "Формула для слагаемого u_i(x):\n"
                           "    u_i(x)=(-1)^i * 2^(2i-1) * (i-1)! * i! * x^(2i+1) / (2i+1)!\n"
                           "Упрощенная формула (формула с переходным коэффициентом):\n"
                           "    u_i+1(x) = (-2x^2*i) / (2i+3) * ui(x)\n"
                           "При вычислении используются инструменты для динамического\n"
			   "выделения необходимой памяти\n";
}

int main()
{
    double x, tmp, ep;
    double * tmps { nullptr }, * sums { nullptr };
    unsigned int i {1}, steps;

    std::cout << credits << std::endl;
    std::cout << description << std::endl;

    do
    {
        std::cout << "Введите X из диапазона (-1, 1):";
        std::cin >> x;
    }while(x <= -1 || x >= 1);

    do
    {
        std::cout << "Введите e из диапазона (0, 1):";
        std::cin >> ep;
    }while(ep <= 0 || ep >= 1);

    tmp = -x*x*x / 3;
    for(;;i++)
    {
        tmp *= -2 * x * x * i;
        tmp /= 2 * i + 3;

        if(i >= 1000 || (tmp < 0? -tmp : tmp) < ep)
            break;
    }
    steps = i;
    tmps = new double[steps + 1];
    sums = new double[steps + 1];

    *tmps = -x*x*x / 3;
    *sums = *tmps;

    for(i = 1; i <= steps; i++)
    {
        *(tmps + i) = *(tmps + i - 1) * -2 * x * x * i;
        *(tmps + i) /= 2 * i + 3;
        *(sums + i) = *(sums + i - 1) + *(tmps + i);
    }


    std::cout << "|" << std::setw(10) << "i"      //"Шаг"
              << "|" << std::setw(20) << "u_i(x)" //"Слагаемое"
              << "|" << std::setw(20) << "s_i(x)" //"Сумма"
              << "|" << std::endl;

    for(i = 0; i < steps; i++)
    {
        std::cout << "|" << std::setw(10) << i + 1
                  << "|" << std::setw(20) << std::setprecision(10) << *(tmps + i)
                  << "|" << std::setw(20) << std::setprecision(10) << *(sums + i)
                  << "|" << std::endl;
    }

    std::cout << "Результат: " << *(sums + steps - 1) << std::endl;

    delete [] tmps;
    delete [] sums;

    return 0;
}
