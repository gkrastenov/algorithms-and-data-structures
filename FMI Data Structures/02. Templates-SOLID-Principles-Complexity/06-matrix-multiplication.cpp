#include <iostream>

/**
 * Напишете функция за произведението на две матрици.
 */

int main()
{
    uint16_t M, N, K;
    std::cin >> M >> N >> K;

    // creating matrix
    float** matrix1 = new float* [M];
    for (int i = 0; i < M; i++)
        matrix1[i] = new float[N];

    float** matrix2 = new float* [N];
    for (int i = 0; i < N; i++)
        matrix2[i] = new float[K];

    for (size_t i = 0; i < M; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            std::cin >> matrix1[i][j];
        }
    }

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < K; j++)
        {
            std::cin >> matrix2[i][j];
        }
    }

    // multy
    for (uint16_t i = 0; i < M; i++)
    {
        for (uint16_t j = 0; j < K; j++)
        {
            float   res = 0;
            for (uint16_t acc = 0; acc < N; acc++)
                res += matrix1[i][acc] * matrix2[acc][j];

            std::cout << res <<"\t";
        }
        std::cout << "\n";
    }

    // deleting matrix
    for (int i = 0; i < M; ++i)
        delete[] matrix1[i];
    delete[] matrix1;

    for (int i = 0; i < N; ++i)
        delete[] matrix2[i];
    delete[] matrix2;

    return 0;
}