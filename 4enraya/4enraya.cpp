// 4enraya.cpp: define el punto de entrada de la aplicación.
//

#include "4enraya.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

using namespace std;


string crear_fila(int fila, int columnas, bool final, vector<char> pieza) {
	string fila_string;

	for (int step = 0; step < 2; step++) {
		for (int i = 0; i < (columnas + 1); i++) {
			if (step == 0) {
				if (i == 0) {
					fila_string += "  ";
				}
				else {
					fila_string += "----";
				}
			}
			else {
				if (i == 0 && !final) {
					fila_string += to_string(fila) + " | ";
				}
				else if (i == 0 && final) {
					fila_string += "  | ";
				}
				else {
					if (final) {
						fila_string += to_string(i - 1) + " | ";
					}
					else if (pieza.size() > 0 && pieza[i - 1] != '\0') {
						fila_string += pieza[i - 1];
						fila_string += " | ";
					}
					else {
						fila_string += "  | ";
					}
				}
			}
		}

		fila_string += "\n";
	}

	return fila_string;
}

vector<string> crear_tablero(int filas, int columnas, vector<vector<char>> piezas) {
	vector<string> tablero;

	for (int i = 0; i < filas; i++) {
		string fila;

		if (!piezas[i].empty()) {
			fila = crear_fila(i, columnas, false, piezas[i]);
		}
		else {
			fila = crear_fila(i, columnas, false);
		}

		tablero.push_back(fila);
	}

	tablero.push_back(crear_fila(filas, columnas, true));

	return tablero;
}

void mostrar_tablero(vector<string> tablero) {
	for (string fila : tablero) {
		cout << fila;
	}
}

int genera_aleatorio(vector<char> fila) {
	int num = 0;
	int columnasDisponibles = COLUMNAS;
	bool generado = false;

	srand(time(NULL));
	num = rand() % (COLUMNAS - 1);

	while (!generado) {
		if (fila[num] != '\0') {
			columnasDisponibles--;
			int mitad = columnasDisponibles / 2;

			if (num > mitad) {
				num = mitad + rand() % (columnasDisponibles + 1) - mitad;
			}
			else {
				if (mitad == 0) {
					if (fila[0] == '\0') {
						num = 0;
					}
					else {
						num = COLUMNAS - 1;
					}
				}
				else {
					num = rand() % mitad;
				}
			}
		}
		else {
			generado = true;
		}
	}

	return num;
}

vector<vector<char>> tirada_jugador(vector<vector<char>> piezas, int filaActual) {
	int columna;
	bool is_columna_correcta;

	do {
		cout << "Selecciona una columna (0-6): ";
		cin >> columna;
		is_columna_correcta = columna >= 0 || columna < COLUMNAS;

		if (!is_columna_correcta) {
			cout << "Columna incorrecta, intenta de nuevo" << endl;
		}
		else if (piezas[filaActual][columna] != '\0') {
			cout << "Columna en uso, intentalo de nuevo" << endl;
			is_columna_correcta = false;
		}
	} while (!is_columna_correcta);

	piezas[filaActual][columna] = 'X';

	return piezas;
}

vector<vector<char>> tirada_maquina(vector<vector<char>> piezas, int filaActual) {
	int columna;
	bool is_columna_correcta;

	do {
		columna = genera_aleatorio(piezas[filaActual]);

		if (piezas[filaActual][columna] == '\0') {
			piezas[filaActual][columna] = 'O';
			is_columna_correcta = true;
		}
		else {
			is_columna_correcta = false;
		}
	} while (!is_columna_correcta);

	return piezas;
}

bool fila_llena(vector<char> piezas) {
	int notNulls = 0;

	for (int columna = 0; columna < piezas.size(); columna++) {
		if (piezas[columna] != '\0') {
			notNulls++;
		}
	}

	int totales = COLUMNAS;

	if (notNulls == totales) {
		return true;
	}

	return false;
}

bool tablero_lleno(vector<vector<char>> piezas) {
	int notNulls = 0;

	for (int fila = 0; fila < piezas.size(); fila++) {
		if (fila_llena(piezas[fila])) {
			notNulls++;
		}
	}

	if (notNulls == FILAS) {
		return true;
	}

	return false;
}

void limpiar_pantalla() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

bool isDiagonal(vector<vector<char>> tablero, char jugador, int columna, int fila) {
	int fichas = 0;
	int direccion = 0;
	int _columna = columna;

	for (int i = fila; i > 0; i--) {
		if (tablero[i][columna] != jugador) {
			if (direccion == 0) {
				if (columna < (COLUMNAS - 1) && tablero[i][columna + 1] == jugador) {
					direccion = 1;
				}
				else if (columna > 0 && tablero[i][columna - 1] == jugador) {
					direccion = -1;
				}
				else {
					return false;
				}

				columna += direccion;
			}
			else {
				return false;
			}

			if (tablero[i][columna] == jugador) {
				fichas++;
			}
		}
		else {
			if (fila > i && _columna == columna) {
				return false;
			}

			fichas++;
		}

		if (direccion != 0) {
			columna += direccion;
		}

		if (columna < 0 || columna >(COLUMNAS - 1)) {
			break;
		}
	}

	if (fichas >= 4) {
		return true;
	}

	return false;
}

bool isColumna(vector<vector<char>> tablero, char jugador, int columna, int fila) {
	int fichas = 0;
	char ficha_anterior = '\0';

	for (int i = fila; i > 0; i--) {
		if (tablero[i][columna] == jugador) {
			if (fichas > 0 && ficha_anterior != jugador) {
				return false;
			}

			fichas++;
		}

		ficha_anterior = tablero[i][columna];
	}

	if (fichas >= 4) {
		return true;
	}

	return false;
}

bool isFila(vector<vector<char>> tablero, char jugador, int fila) {
	int fichas = 0;
	char ficha_anterior = '\0';

	for (int i = 0; i < COLUMNAS; i++) {
		if (tablero[fila][i] == jugador) {
			if (fichas > 0 && ficha_anterior != jugador) {
				return false;
			}

			fichas++;
		}

		ficha_anterior = tablero[fila][i];
	}

	if (fichas >= 4) {
		return true;
	}

	return false;
}

int puntuacion(vector<vector<char>> tablero, char jugador) {
	int puntos = 0;

	for (int fila = (FILAS - 1); fila > 0; fila--) {
		for (int columna = 0; columna < COLUMNAS; columna++) {
			if (isDiagonal(tablero, jugador, columna, fila)) {
				puntos++;
			}
			else if (isColumna(tablero, jugador, columna, fila)) {
				puntos++;
			}
		}

		if (isFila(tablero, jugador, fila)) {
			puntos++;
		}
	}

	return puntos;
}

int jugar() {
	vector<string> tablero;
	vector<vector<char>> piezas;
	piezas.resize(FILAS, vector<char>(COLUMNAS, '\0'));
	int filaActual = FILAS - 1;

	tablero = crear_tablero(FILAS, COLUMNAS, piezas);
	mostrar_tablero(tablero);

	while (!tablero_lleno(piezas)) {
		while (!fila_llena(piezas[filaActual])) {
			// turno del jugador
			piezas = tirada_jugador(piezas, filaActual);

			limpiar_pantalla(); // limpia la pantalla

			tablero = crear_tablero(FILAS, COLUMNAS, piezas);
			mostrar_tablero(tablero);

			if (fila_llena(piezas[filaActual])) {
				filaActual--;
			}

			// turno de la maquina
			piezas = tirada_maquina(piezas, filaActual);

			limpiar_pantalla(); // limpia la pantalla

			tablero = crear_tablero(FILAS, COLUMNAS, piezas);
			mostrar_tablero(tablero);

			int puntuacion_jugador = puntuacion(piezas, 'X');
			int puntuacion_maquina = puntuacion(piezas, 'O');

			if (puntuacion_jugador > puntuacion_maquina) {
				return 1;
			}
			else if (puntuacion_jugador < puntuacion_maquina) {
				return -1;
			}
		}

		if (filaActual > 0) {
			filaActual--;
		}
	}

	return 0;
}

int main() {
	char opcion;

	do {
		int ganador = jugar();

		if (ganador == 1) {
			cout << "Has ganado! :D" << endl;
		}
		else if (ganador == -1) {
			cout << "La maquina ha ganado, has perdido :(" << endl;
		}
		else {
			cout << "La partida ha quedado en empate!" << endl;
		}

		cout << "Quieres volver a jugar? (Y/N)" << endl;
		cin >> opcion;

		if (opcion == 'N' || opcion == 'n') {
			cout << "Cerrando programa, hasta luego!" << endl;
			this_thread::sleep_for(chrono::milliseconds(1000));
		}
		else {
			limpiar_pantalla();
		}
	} while (opcion == 'Y' || opcion == 'y');

	return 0;
}