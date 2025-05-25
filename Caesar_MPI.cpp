#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

char shift_char(char c, int key) {
    if (isupper(c)) return 'A' + (c - 'A' + key) % 26;
    if (islower(c)) return 'a' + (c - 'a' + key) % 26;
    return c;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, key = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
        cout << "Length\tTime (s)" << endl;

    for (int pow = 1; pow <= 24; pow++) {
        int length = 1 << pow; // 2^pow
        int chunk = length / size;

        string text(length, 'a');
        vector<char> local(chunk);
        vector<char> result(length);

        // початок таймера
        double start_time = 0;
        if (rank == 0)
            start_time = MPI_Wtime();

        // передаємо фрагменти
        MPI_Scatter(text.data(), chunk, MPI_CHAR, local.data(), chunk, MPI_CHAR, 0, MPI_COMM_WORLD);

        for (int i = 0; i < chunk; i++) {
            local[i] = shift_char(local[i], key);
        }

        // збираємо результат
        MPI_Gather(local.data(), chunk, MPI_CHAR, result.data(), chunk, MPI_CHAR, 0, MPI_COMM_WORLD);

        // вивід часу на rank 0
        if (rank == 0) {
            double end_time = MPI_Wtime();
            double elapsed = end_time - start_time;
            cout << length << "\t" << fixed << setprecision(6) << elapsed << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
