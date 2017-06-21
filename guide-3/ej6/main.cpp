#include <iostream>
#include <vector>
#include <chrono>  // for high_resolution_clock.
#include <math.h>
#include <sstream>


using namespace std;

using mat3 = vector<vector<vector<long double > > >;
using mat2 = vector<vector<long double > >;
using mat1 = vector<long double>;

void printMat(mat2 m) {
    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[0].size(); ++j) {
            cout << m[i][j] << " " ;
        }
        cout << endl;
    }
    cout << endl;
}

/* EJERCICIO 6 DE PRACTICA 3
    Donde x, y ∈ [0; 2] y t > 0 son las variables espacial y temporal respectivamente. u(x, y, t) y v(x, y, t) son los
    componentes x-y de la velocidad del fluido dependiente del espacio y el tiempo. Se desea resolver el Liddriven 
    cavity problem en el cual a un recipiente (2D) que contiene un determinado fluido se le desliza su tapa
    hacia uno de los lados, provocando cambios en el movimiento y la presi ́on del fluido. Para resolver el problema
    se cuenta con los siguiente datos: ν = 0.1. ρ=1. Condiciones iniciales u, v, p = 0 en todos lados. Condiciones
    de contorno u = 1 en y = 2 (la “tapa”). u, v = 0 en los demas bordes. ∂p/∂y = 0 en y = 0. p = 0 en y = 2. ∂p/∂x =0 en x = 0, 2
*/


int main() {

    long double xMax = 2.0;
    long double yMax = 2.0;
    long double tMax = 1.0;
    long double nu = 0.1; //viscosidad
    long double rho = 1;   //densidad
    long double dx = 0.2;
    long double dy = 0.2;
    long double dt = 0.005;
    int nX = round(xMax / dx) + 1;
    int nY = round(yMax / dy) + 1;
    int nT = round(tMax / dt) + 1;
    long double al = 1.0;
    //0 es tiempo n-1, 1 es tiempo n, 2 es tiempo n+1
    //3 es simplemente las matrices auxiliares para no perder datos de el tiempo 2
    mat2 U0(nX, vector<long double>(nY));
    mat2 V0(nX, vector<long double>(nY));
    mat2 P0(nX, vector<long double>(nY));
    mat2 U1(nX, vector<long double>(nY));
    mat2 V1(nX, vector<long double>(nY));
    mat2 P1(nX, vector<long double>(nY));
    mat2 U2(nX, vector<long double>(nY));
    mat2 V2(nX, vector<long double>(nY));
    mat2 P2(nX, vector<long double>(nY));
    mat2 U3(nX, vector<long double>(nY));
    mat2 V3(nX, vector<long double>(nY));
    mat2 P3(nX, vector<long double>(nY));

    for (int i = 0; i < nX; ++i) {
        fill(U0[i].begin(), U0[i].end(), 0);
        fill(V0[i].begin(), V0[i].end(), 0);
        fill(P0[i].begin(), P0[i].end(), 0);
        fill(U1[i].begin(), U1[i].end(), 0);
        fill(V1[i].begin(), V1[i].end(), 0);
        fill(P1[i].begin(), P1[i].end(), 0);
        fill(U2[i].begin(), U2[i].end(), 0);
        fill(V2[i].begin(), V2[i].end(), 0);
        fill(P2[i].begin(), P2[i].end(), 0);
        fill(U3[i].begin(), U3[i].end(), 0);
        fill(V3[i].begin(), V3[i].end(), 0);
        fill(P3[i].begin(), P3[i].end(), 0);
    }

    //condicion borde u=1 en y=2
    for (int i = 0; i < nX; ++i) {
        U0[i][nY - 1] = 1.0;
        U1[i][nY - 1] = 1.0;
        U2[i][nY - 1] = 1.0;
        U3[i][nY - 1] = 1.0;
    }

    for (long double t = 0.0; t < tMax; t = t + dt) {
        printMat(U0);
        printMat(V0);
        printMat(P0);

        for (int i = 1; i < nX - 1; ++i) {
            for (int j = 1; j < nY - 1; ++j) { //las condiciones borde en Y=2 se respetan aca
                for (int k = 0; true; ++k) {
                    //long double oldU = U2[i][j];
                    //long double oldV = V2[i][j];

                    long double U1x = (U1[i + 1][j] - U1[i - 1][j]) / (2* dx);
                    long double U2x = (U2[i + 1][j] - U2[i - 1][j]) / (2* dx);
                    long double U1y = (U1[i][j + 1] - U1[i][j - 1]) / (2* dy);
                    long double U2y = (U2[i][j + 1] - U2[i][j - 1]) / (2* dy);
                    long double U1xx = (U1[i + 1][j] - 2* U1[i][j] + U1[i - 1][j]) / (dx* dx);
                    long double U2xx = (U2[i + 1][j] - 2* U2[i][j] + U2[i - 1][j]) / (dx* dx);
                    long double U1yy = (U1[i][j + 1] - 2* U1[i][j] + U1[i][j - 1]) / (dy* dy);
                    long double U2yy = (U2[i][j + 1] - 2* U2[i][j] + U2[i][j - 1]) / (dy* dy);
                    long double P1x = (P1[i + 1][j] - P1[i - 1][j]) / (2* dx);
                    long double P2x = (P2[i + 1][j] - P2[i - 1][j]) / (2* dx);
                    long double P1y = (P1[i][j + 1] - P1[i][j - 1]) / (2* dy);
                    long double P2y = (P2[i][j + 1] - P2[i][j - 1]) / (2* dy);
                    long double V1x = (V1[i + 1][j] - V1[i - 1][j]) / (2* dx);
                    long double V2x = (V2[i + 1][j] - V2[i - 1][j]) / (2* dx);
                    long double V1y = (V1[i][j + 1] - V1[i][j - 1]) / (2* dy);
                    long double V2y = (V2[i][j + 1] - V2[i][j - 1]) / (2* dy);
                    long double V1xx = (V1[i + 1][j] - 2* V1[i][j] + V1[i - 1][j]) / (dx* dx);
                    long double V2xx = (V2[i + 1][j] - 2* V2[i][j] + V2[i - 1][j]) / (dx* dx);
                    long double V1yy = (V1[i][j + 1] - 2* V1[i][j] + V1[i][j - 1]) / (dy* dy);
                    long double V2yy = (V2[i][j + 1] - 2* V2[i][j] + V2[i][j - 1]) / (dy* dy);
                    long double P1xx = (P1[i + 1][j] - 2* P1[i][j] + P1[i - 1][j]) / (dx* dx);
                    long double P1yy = (P1[i][j + 1] - 2* P1[i][j] + P1[i][j - 1]) / (dy* dy);
                    long double P2yy = (P2[i][j + 1] - 2* P2[i][j] + P2[i][j - 1]) / (dy* dy);

                    //TODO: chequear despejes de ecuación 3
                    U3[i][j] = U0[i][j] + 2* dt* (-U1[i][j]* (al* U1x  + (1 - al)* U2x ) - V1[i][j]* (al* U1y + (1 - al)* U2y)
                                                   - (1 / rho)* (al* P1x + (1 - al)* P2x) + nu* (al* U1xx + (1 - al)* U2xx + al* U1yy + (1 - al)* U2yy));
                    V3[i][j] = V0[i][j] + 2* dt* (-U1[i][j]* (al* V1x  + (1 - al)* V2x ) - V1[i][j]* (al* V1y + (1 - al)* V2y)
                                                   - (1 / rho)* (al* P1y + (1 - al)* P2y) + nu* (al* V1xx + (1 - al)* V2xx + al* V1yy + (1 - al)* V2yy));
                    P3[i][j] = (P2[i - 1][j] / 2)  + (P2[i + 1][j] / 2) + (dx* dx) / (2* al - 2)* (-al* P1xx - al* P1yy - (1 - al)* P2yy
                              - rho*( pow((al* U1x + (1 - al)* U2x), 2) + 2* (al* U1y + (1 - al)* U2y)* (al* V1x + (1 - al)* V2x) + pow((al* V1y + (1 - al)* V2y), 2)));


                    long double diff = sqrt(pow(U3[i][j] - U2[i][j], 2) + pow(V3[i][j] - V2[i][j], 2));
                    if (diff < 0.01){
                        break;
                    } else if (k > 5) {
                        //cerr << "ERROR: unstable." << endl;
                        break;
                    }
                }
            }
        }
        U0 = U1;
        U1 = U2;
        U2 = U3;

        V0 = V1;
        V1 = V2;
        V2 = V3;

        P0 = P1;
        P1 = P2;
        P2 = P3;
    }
}



