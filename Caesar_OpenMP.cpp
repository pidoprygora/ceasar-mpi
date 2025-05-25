#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

// ���������� 1 �������
char shift_char(char c, int key) {
    if (isupper(c)) return 'A' + (c - 'A' + key) % 26;
    if (islower(c)) return 'a' + (c - 'a' + key) % 26;
    return c;
}

int main() {
    int key = 3;

    // ������� ����������
    cout << "Length\tTime (s)" << endl;

    for (int pow = 1; pow <= 24; pow++) { // 2^1 �� 2^24 (2 -> 16 ��� �������)
        int len = 1 << pow;               // ����� len = 2^pow
        string text(len, 'a');            // ��������� ����� �� ������� 'a'

        auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for
        for (int i = 0; i < len; i++) {
            text[i] = shift_char(text[i], key);
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        cout << len << "\t" << fixed << setprecision(6) << duration.count() << endl;
    }

    return 0;
}
