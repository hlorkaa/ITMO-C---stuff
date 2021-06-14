#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

thread philosopher[5]; // Пять философов
mutex fork[5]; // Пять вилок

void eating(int i) { // Процесс приема пищи философом
    while (true) {

        std::lock(fork[i], fork[(i + 1) % 5]); // Философ берет вилки и "ест"

        cout << " philosopher " << i << " takes forks " << i << " and " << (i + 1) % 5 << endl;
        cout << " philosopher " << i << " is eating " << endl;

        fork[(i + 1) % 5].unlock(); fork[i].unlock(); // Философ кладет вилки

        cout << "philosopher " << i << " puts forks " << i << " and " << (i + 1) % 5 << endl;
    }
}

int main() {

    for (int i = 0; i < 5; ++i) {
        philosopher[i] = (thread(eating, i));
    }
    for (int i = 0; i < 5; i++) {
        philosopher[i].join();
    }
}

// P.S. Вам не кажется, что использование одной и той же вилки двумя людьми единовременно несколько негигиенично..?