#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>
#include "Eigen/Dense"
#include <chrono>

using namespace Eigen;
using namespace std;


void regression( vector<double>& x_vals,  vector<double>& y_vals, double& slope, double& intercept) {
    
    // Transformar el vector<double> en VectorXd
    Map<const VectorXd> x(x_vals.data(), x_vals.size());
    Map<const VectorXd> y(y_vals.data(), y_vals.size());
    
    int n = x.size();

    // Construir la matriz X y el vector Y
    MatrixXd X(n, 2);
    X.col(0) = VectorXd::Ones(n);
    X.col(1) = x;

    // Calcular la regresión lineal usando la fórmula cerrada (X^T*X)^-1*X^T*Y
    Vector2d theta = (X.transpose() * X).ldlt().solve(X.transpose() * y);

    // Asignar los valores de la pendiente (slope) e intercepto
    intercept = theta(0);
    slope = theta(1);
}

vector<int> roundToInteger(const vector<double>& values) {
    vector<int> roundedValues;
    
    for (const double& value : values) {
        roundedValues.push_back(static_cast<int>(round(value)));
    }
    
    return roundedValues;
}

// Función para calcular el percentil de un valor en un vector
double calcularPercentil(const std::vector<double>& valores, double valor) {
    // Copia el vector para no alterar el orden original
    std::vector<double> copiaValores = valores;

    // Ordena el vector de valores
    std::sort(copiaValores.begin(), copiaValores.end());

    // Encuentra la posición del valor en el vector ordenado
    auto it = std::lower_bound(copiaValores.begin(), copiaValores.end(), valor);

    // Calcula el percentil usando la fórmula de la distribución empírica de probabilidad
    double percentil = (it - copiaValores.begin()) / static_cast<double>(copiaValores.size()) * 100.0;

    return percentil;
}

vector<double> calcularPercentilVector(const std::vector<double>& dist, vector<double>& valores) {
    // Copia el vector para no alterar el orden original
    vector<double> percentils;

    std::vector<double> copiaValores = dist;

    // Ordena el vector de dist
    std::sort(copiaValores.begin(), copiaValores.end());

    for(double valor : valores) {
        // Encuentra la posición del valor en el vector ordenado
        auto it = std::lower_bound(copiaValores.begin(), copiaValores.end(), valor);

        // Calcula el percentil usando la fórmula de la distribución empírica de probabilidad
        double percentil = (it - copiaValores.begin()) / static_cast<double>(copiaValores.size()) * 100.0;
        percentils.push_back(percentil);
    }
    

    return percentils;
}



