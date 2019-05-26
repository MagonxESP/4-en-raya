// 4enraya.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos de un proyecto.

#pragma once

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// TODO: Haga referencia aquí a los encabezados adicionales que el programa requiere.
const int FILAS = 6;
const int COLUMNAS = 7;

string crear_fila(int fila, int columnas, bool final, vector<char> pieza = vector<char>());
vector<string> crear_tablero(int filas, int columnas, vector< vector<char> > piezas);
void mostrar_tablero(vector<string> tablero);
int genera_aleatorio(vector<char> fila);
vector< vector<char> > tirada_jugador(vector< vector<char> > piezas, int filaActual);
vector< vector<char> > tirada_maquina(vector< vector<char> > piezas, int filaActual);
bool fila_llena(vector<char> piezas);
bool tablero_lleno(vector< vector<char> > piezas);
void limpiar_pantalla();
bool isDiagonal(vector< vector<char> > tablero, char jugador, int columna, int fila);
bool isColumna(vector< vector<char> > tablero, char jugador, int columna, int fila);
bool isFila(vector< vector<char> > tablero, char jugador, int fila);
int puntuacion(vector< vector<char> > tablero, char jugador);
int jugar();
