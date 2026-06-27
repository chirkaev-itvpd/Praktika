#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_SIZE 1000

int step = 0;

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void cocktailSort(int arr[], int n)
{
    int left = 0;
    int right = n - 1;
    int swapped = 1;
    step = 0;

    printf("\n=== ПРОЦЕСС СОРТИРОВКИ ===\n");

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
            }
        }

        if (!swapped)
            break;

        right--;
        step++;
        printf("Шаг %d (→): ", step);
        printArray(arr, n);

        swapped = 0;

        for (int i = right; i > left; i--)
        {
            if (arr[i - 1] > arr[i])
            {
                int temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                swapped = 1;
            }
        }

        left++;
        step++;
        printf("Шаг %d (←): ", step);
        printArray(arr, n);
    }

    printf("=== СОРТИРОВКА ЗАВЕРШЕНА ===\n\n");
}

int loadCSV(const char* filename, int arr[])
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return -1;

    char line[4096];
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return -1;
    }
    fclose(file);

    int count = 0;
    char* token = strtok(line, ",");

    while (token != NULL)
    {
        while (*token == ' ' || *token == '\t')
            token++;

        char* endptr;
        long value = strtol(token, &endptr, 10);

        while (*endptr == ' ' || *endptr == '\n' ||
            *endptr == '\r' || *endptr == '\t')
        {
            endptr++;
        }

        if (*endptr != '\0' && *endptr != ',')
        {
            return -2;
        }

        if (value > 2147483647 || value < -2147483648)
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

void saveReport(const char* filename, int arr[], int n, double time_ms, const char* inputFile, const char* outputFile)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Ошибка создания отчёта!\n");
        return;
    }

    fprintf(file, "========== ОТЧЁТ ПО ШЕЙКЕРНОЙ СОРТИРОВКЕ ==========\n\n");
    fprintf(file, "Исходный файл: %s\n", inputFile);
    fprintf(file, "Выходной файл: %s\n", outputFile);
    fprintf(file, "Размер массива: %d элементов\n", n);
    fprintf(file, "Время выполнения: %.3f мс\n", time_ms);
    fprintf(file, "\nИсходный массив: ");
    for (int i = 0; i < n; i++)
        fprintf(file, "%d ", arr[i]);
    fprintf(file, "\n\nОтсортированный массив: ");
    for (int i = 0; i < n; i++)
        fprintf(file, "%d ", arr[i]);

    fprintf(file, "\n\n========== КОНЕЦ ОТЧЁТА ==========\n");

    fclose(file);
    printf("✓ Отчёт сохранён в файл %s\n", filename);
}

void generateRandomArray(int arr[], int n)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100;
    }
}

int manualInput(int arr[])
{
    int n;
    printf("Введите количество элементов (не более %d): ", MAX_SIZE);
    scanf("%d", &n);

    if (n <= 0 || n > MAX_SIZE)
    {
        printf("Некорректное количество!\n");
        return 0;
    }

    printf("Введите %d чисел через пробел: ", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    return n;
}

int isSorted(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
            return 0;
    }
    return 1;
}

void showMenu()
{
    printf("\n====================================\n");
    printf("    ШЕЙКЕРНАЯ СОРТИРОВКА\n");
    printf("====================================\n");
    printf("1 - Загрузить массив из CSV файла\n");
    printf("2 - Ввести массив вручную\n");
    printf("3 - Сгенерировать случайный массив\n");
    printf("4 - Выйти\n");
    printf("====================================\n");
    printf("Выберите действие: ");
}

int main(void)
{
    setlocale(LC_ALL, "Russian");

    int arr[MAX_SIZE];
    int n = 0;
    int choice;
    double time_ms;
    char inputFilename[256];
    char outputFilename[256];

    while (1)
    {
        showMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            printf("Введите имя файла для загрузки (например, input.csv): ");
            scanf("%s", inputFilename);

            n = loadCSV(inputFilename, arr);
            if (n == -1)
            {
                printf("✗ Ошибка: не удалось открыть файл %s\n", inputFilename);
                continue;
            }
            else if (n == -2)
            {
                printf("✗ Ошибка: некорректные данные в файле (допустимы только числа)\n");
                continue;
            }
            else if (n == -3)
            {
                printf("✗ Ошибка: число выходит за пределы допустимого диапазона (int)\n");
                continue;
            }
            else if (n == 0)
            {
                printf("✗ Файл пуст или содержит только разделители\n");
                continue;
            }
            printf("✓ Загружено %d элементов из %s\n", n, inputFilename);
            break;

        case 2:
            n = manualInput(arr);
            if (n == 0)
                continue;
            strcpy(inputFilename, "ручной ввод");
            break;

        case 3:
            printf("Введите размер массива (не более %d): ", MAX_SIZE);
            scanf("%d", &n);
            if (n <= 0 || n > MAX_SIZE)
            {
                printf("✗ Некорректный размер!\n");
                continue;
            }
            generateRandomArray(arr, n);
            printf("✓ Сгенерирован случайный массив из %d элементов\n", n);
            strcpy(inputFilename, "случайная генерация");
            break;

        case 4:
            printf("До свидания!\n");
            return 0;

        default:
            printf("✗ Неверный выбор!\n");
            continue;
        }

        printf("\nИсходный массив (%d элементов):\n", n);
        printArray(arr, n);

        if (isSorted(arr, n))
        {
            printf("\n⚠ Массив уже отсортирован!\n");
        }

        clock_t start = clock();
        cocktailSort(arr, n);
        clock_t end = clock();
        time_ms = (double)(end - start) * 1000 / CLOCKS_PER_SEC;

        printf("Отсортированный массив:\n");
        printArray(arr, n);

        printf("\n========== СТАТИСТИКА ==========\n");
        printf("Размер массива: %d элементов\n", n);
        printf("Время:         %.3f мс\n", time_ms);

        printf("Введите имя файла для сохранения (например, output.csv): ");
        scanf("%s", outputFilename);

        if (saveCSV(outputFilename, arr, n))
        {
            printf("\n✓ Результат сохранён в %s\n", outputFilename);
        }
        else
        {
            printf("\n✗ Ошибка сохранения %s\n", outputFilename);
        }

        saveReport("report.txt", arr, n, time_ms, inputFilename, outputFilename);

        printf("\nНажмите Enter для продолжения...");
        getchar();
    }

    return 0;
}