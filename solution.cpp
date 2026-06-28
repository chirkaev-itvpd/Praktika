#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE 10000

long long cocktailSort(int arr[], int n)
{
    int left = 0;
    int right = n - 1;
    int swapped = 1;
    long long swap_count = 0;

    while (swapped)
    {
        swapped = 0;

        for (int i = left; i < right; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
                swap_count++;
            }
        }

        if (!swapped)
            break;

        right--;
        swapped = 0;

        for (int i = right; i > left; i--)
        {
            if (arr[i - 1] > arr[i])
            {
                int temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                swapped = 1;
                swap_count++;
            }
        }

        left++;
    }
    return swap_count;
}

int loadCSV(const char* filename, int arr[])
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return -1;
    }

    char line[65536];

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return -2;
    }

    fclose(file);

    int count = 0;
    char* token = strtok(line, ",");

    while (token != NULL)
    {
        if (count >= MAX_SIZE) break;

        char* endptr;
        long value = strtol(token, &endptr, 10);

        while (*endptr == ' ' || *endptr == '\n' || *endptr == '\r' || *endptr == '\t')
        {
            endptr++;
        }

        if (*endptr != '\0')
        {
            return -3;
        }

        arr[count++] = (int)value;
        token = strtok(NULL, ",");
    }

    return count;
}

int saveCSV(const char* filename, int arr[], int n)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
        return 0;

    for (int i = 0; i < n; i++)
    {
        fprintf(file, "%d", arr[i]);
        if (i < n - 1)
            fprintf(file, ",");
    }

    fclose(file);
    return 1;
}

void printArray(int arr[], int n)
{
    if (n > 20) {
        for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
        printf("... ");
        for (int i = n - 10; i < n; i++) printf("%d ", arr[i]);
        printf("\n(Всего элементов: %d. Вывод ограничен)\n", n);
    }
    else {
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

void generateRandomArray(int arr[], int n, int min_val, int max_val)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = min_val + rand() % (max_val - min_val + 1);
    }
}

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    srand((unsigned int)time(NULL));

    int arr[MAX_SIZE];
    int n = 0;
    int choice;

    char input_filename[256] = "input.csv";
    char output_filename[256] = "output.csv";

    while (1)
    {
        printf("\n================ МЕНЮ ================\n");
        printf("1. Загрузить массив из CSV файла\n");
        printf("2. Сгенерировать массив случайных чисел\n");
        printf("3. Выйти из программы\n");
        printf("Выберите действие (1-3): ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода чисел! Выход.\n");
            break;
        }

        if (choice == 3) {
            printf("Программа завершена.\n");
            break;
        }

        if (choice == 1) {
            printf("Введите имя исходного файла (например, input.csv): ");
            scanf("%255s", input_filename);

            n = loadCSV(input_filename, arr);

            if (n == -1) {
                printf("Ошибка: не удалось открыть файл %s\n", input_filename);
                continue;
            }
            if (n == -2) {
                printf("Ошибка: файл %s пуст.\n", input_filename);
                continue;
            }
            if (n == -3) {
                printf("Ошибка: некорректные данные в файле %s.\n", input_filename);
                continue;
            }
            printf("Массив успешно загружен из файла!\n");
        }
        else if (choice == 2) {
            int min_val, max_val;
            printf("Введите количество элементов массива (макс. %d): ", MAX_SIZE);
            scanf("%d", &n);
            if (n <= 0 || n > MAX_SIZE) {
                printf("Некорректный размер массива. Возврат в меню.\n");
                continue;
            }

            printf("Введите минимальное значение диапазона: ");
            scanf("%d", &min_val);
            printf("Введите максимальное значение диапазона: ");
            scanf("%d", &max_val);

            if (min_val > max_val) {
                int t = min_val; min_val = max_val; max_val = t;
            }

            generateRandomArray(arr, n, min_val, max_val);
            printf("Массив случайных чисел успешно сгенерирован!\n");
        }
        else {
            printf("Неверный пункт меню. Попробуйте снова.\n");
            continue;
        }

        printf("\nИсходный массив:\n");
        printArray(arr, n);

        printf("\nСортировка алгоритмом Cocktail Sort...\n");

        clock_t start_time = clock();
        long long total_swaps = cocktailSort(arr, n);
        clock_t end_time = clock();

        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        printf("Отсортированный массив:\n");
        printArray(arr, n);

        printf("\n-------------------------------------\n");
        printf("Время работы алгоритма: %.6f секунд.\n", time_taken);
        printf("Количество перестановок: %lld\n", total_swaps);
        printf("-------------------------------------\n");

        printf("Введите название результирующего файла для сохранения (например, output.csv): ");
        scanf("%255s", output_filename);

        if (saveCSV(output_filename, arr, n)) {
            printf("Результат успешно сохранен в файл %s\n", output_filename);
        }
        else {
            printf("Ошибка сохранения в файл %s\n", output_filename);
        }
    }

    return 0;
}