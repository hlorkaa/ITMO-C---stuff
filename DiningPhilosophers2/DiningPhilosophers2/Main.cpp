#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

thread philosopher[5]; // ���� ���������
mutex fork[5]; // ���� �����

void eating(int i) { // ������� ������ ���� ���������
    while (true) {

        std::lock(fork[i], fork[(i + 1) % 5]); // ������� ����� ����� � "���"

        cout << " philosopher " << i << " takes forks " << i << " and " << (i + 1) % 5 << endl;
        cout << " philosopher " << i << " is eating " << endl;

        fork[(i + 1) % 5].unlock(); fork[i].unlock(); // ������� ������ �����

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

// P.S. ��� �� �������, ��� ������������� ����� � ��� �� ����� ����� ������ ������������� ��������� ������������..?