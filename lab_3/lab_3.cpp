// Подключение необходимых библиотек
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// Глобальные переменные для использования в методе гомори
int a00, a10, a01, a11, b00, b11, c00, c11;

// Класс, реализующий метод симплекса 
class Simplex {
private:
    std::vector<std::vector<double>> tableau; // Матрица для симплекс-метода

    // Инициализация матрицы для симплекс-метода
    void initializeSimplexTable(const std::vector<std::vector<double>>& constraints, const std::vector<double>& objectiveFunction) {
        int m = constraints.size(); // Количество ограничений
        int n = constraints[0].size() - 1; // Количество переменных
        tableau.resize(m + 1, std::vector<double>(n + m + 1)); // Инициализация матрицы

        // Заполнение матрицы ограничениями и коэффициентами целевой функции
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                tableau[i][j] = constraints[i][j];
            }
            tableau[i][n + i] = 1;
            tableau[i][n + m] = constraints[i][n];
        }

        // Заполнение последней строки матрицы коэффициентами целевой функции с обратными знаками
        for (int j = 0; j < n; ++j) {
            tableau[m][j] = -objectiveFunction[j];
        }
    }

    // Поиск разрешающего столбца для симплекс-метода
    int findPivotColumn() {
        int n = tableau[0].size();
        int pivotColumn = 0;
        for (int j = 1; j < n - 1; ++j) {
            if (tableau[tableau.size() - 1][j] < tableau[tableau.size() - 1][pivotColumn]) {
                pivotColumn = j;
            }
        }
        return pivotColumn;
    }

    // Поиск разрешающей строки для симплекс-метода
    int findPivotRow(int pivotColumn) {
        int m = tableau.size() - 1;
        int pivotRow = 0;
        double minRatio = -1;

        for (int i = 0; i < m; ++i) {
            if (tableau[i][pivotColumn] > 0) {
                double ratio = tableau[i][tableau[0].size() - 1] / tableau[i][pivotColumn];
                if (minRatio == -1 || ratio < minRatio) {
                    minRatio = ratio;
                    pivotRow = i;
                }
            }
        }
        return pivotRow;
    }

    // Выполнение операции пересчета при симплекс-методе
    void performPivotOperation(int pivotRow, int pivotColumn) {
        int m = tableau.size();
        int n = tableau[0].size();

        double pivot = tableau[pivotRow][pivotColumn];
        for (int j = 0; j < n; ++j) {
            tableau[pivotRow][j] /= pivot;
        }

        for (int i = 0; i < m; ++i) {
            if (i != pivotRow) {
                double ratio = tableau[i][pivotColumn];
                for (int j = 0; j < n; ++j) {
                    tableau[i][j] -= ratio * tableau[pivotRow][j];
                }
            }
        }
    }

    // Проверка, является ли текущее решение оптимальным для симплекс-метода
    bool isOptimalSolution() {
        int n = tableau[0].size();
        for (int j = 0; j < n - 1; ++j) {
            if (tableau[tableau.size() - 1][j] < 0) {
                return false;
            }
        }
        return true;
    }

    bool isIntegerSolution(const std::vector<double>& solution) {
        for (double val : solution) {
            if (std::abs(val - std::round(val)) > 1e-6) {
                return false;
            }
        }
        return true;
    }

public:
    // Метод симплекс-метода для решения задачи линейного программирования
    std::vector<double> solve(std::vector<std::vector<double>> constraints, std::vector<double> objectiveFunction) {
        initializeSimplexTable(constraints, objectiveFunction);

        while (!isOptimalSolution()) {
            int pivotColumn = findPivotColumn();
            int pivotRow = findPivotRow(pivotColumn);
            performPivotOperation(pivotRow, pivotColumn);
        }

        // Формирование вектора решения из таблицы симплекс-метода
        std::vector<double> solution;
        int m = constraints.size();
        int n = constraints[0].size() - 1;

        for (int i = 0; i < m; ++i) {
            solution.push_back(tableau[i][n + m]);
        }

        return solution;
    }

    bool hasIntegerSolution(std::vector<std::vector<double>> constraints, std::vector<double> objectiveFunction) {
        std::vector<double> solution = solve(constraints, objectiveFunction);
        return isIntegerSolution(solution);
    }

    int calculateObjectiveValue(int x, int y, const std::vector<int>& c) {
        return c[0] * x + c[1] * y;
    }

};

// Главная функция программы
int main() {
    setlocale(LC_ALL, "Rus"); // Установка локали для корректного отображения русского текста
    Simplex simplexSolver; // Создание экземпляра класса Simplex

    int numConstraints, numVariables;
    std::cout << "Введите количество ограничений: ";
    std::cin >> numConstraints;

    std::cout << "Введите количество переменных: ";
    std::cin >> numVariables;

    // Создание вектора для хранения коэффициентов ограничений
    std::vector<std::vector<double>> constraints(numConstraints, std::vector<double>(numVariables + 1));

    std::cout << "Введите коэффициенты для каждого ограничения:" << std::endl;
    // Ввод коэффициентов ограничений
    for (int i = 0; i < numConstraints; ++i) {
        std::cout << "Ограничение " << i + 1 << ": ";
        for (int j = 0; j <= numVariables; ++j) {
            std::cout << "Коэффициент " << j + 1 << ": ";
            std::cin >> constraints[i][j];
        }
    }

    // Создание вектора для хранения коэффициентов целевой функции
    std::vector<double> objectiveFunction(numVariables);
    std::cout << "Введите коэффициенты для целевой функции:" << std::endl;
    // Ввод коэффициентов целевой функции
    for (int i = 0; i < numVariables; ++i) {
        std::cout << "Коэффициент " << i + 1 << ": ";
        std::cin >> objectiveFunction[i];
    }

    // Решение задачи линейного программирования методом симплекса
    std::vector<double> solution = simplexSolver.solve(constraints, objectiveFunction);

    // Установка параметров для метода ветвей и границ
    a00 = constraints[0][0]; a01 = constraints[0][1];
    a10 = constraints[1][0]; a11 = constraints[1][1];
    b00 = constraints[0][2]; b11 = constraints[1][2];
    c00 = objectiveFunction[0]; c11 = objectiveFunction[1];

    if (simplexSolver.hasIntegerSolution(constraints, objectiveFunction)) {
        std::cout << "оптимизация не требуется" << std::endl;
    }
    else {
        vector<vector<int>> A = { {a00, a01}, {a10, a11} }; // Матрица коэффициентов 
        vector<int> b = { b00, b11 }; // Вектор ограничений
        vector<int> c = { c00, c11 }; // Коэффициенты целевой функции

        // Инициализация переменных для хранения оптимального решения
        int maxObjectiveValue = INT_MIN;
        int idealX = -1, idealY = -1;

        // Перебор всех возможных значений x и y, удовлетворяющих ограничениям
        for (int x = 0; x <= b00; ++x) {
            for (int y = 0; y <= b11; ++y) {
                bool isValid = true;

                // Проверка удовлетворения всех ограничений
                for (size_t i = 0; i < A.size(); ++i) {
                    if (x * A[i][0] + y * A[i][1] > b[i]) {
                        isValid = false;
                        break;
                    }
                }

                // Если решение удовлетворяет ограничениям, вычисляем значение целевой функции
                if (isValid) {
                    int objectiveValue = simplexSolver.calculateObjectiveValue(x, y, c);
                    // Обновление оптимального решения, если получено более высокое значение целевой функции
                    if (objectiveValue > maxObjectiveValue) {
                        maxObjectiveValue = objectiveValue;
                        idealX = x;
                        idealY = y;
                    }
                }
            }
        }

        // Вывод оптимального решения
        cout << "Optimal Objective Value: " << maxObjectiveValue << endl;
        cout << "x: " << idealX << endl;
        cout << "y: " << idealY << endl;

        return 0;
    }

    return 0;
}