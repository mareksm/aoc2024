#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b)
{
    return (*(int *) a - *(int *) b);
}

int count_occurances(int *arr, int size, int target)
{
    int *item = (int *) bsearch(&target, arr, size, sizeof(int), compare);
    if (!item)
        return 0;

    int index = item - arr;
    int count = 1;

    for (int i = index - 1; i >= 0 && arr[i] == target; i--)
        count++;

    for (int i = index + 1; i < size && arr[i] == target; i++)
        count++;

    return count;
}

int main(int argc, char **argv)
{
#define arr_len 1000
    int left[arr_len], right[arr_len];
    int i = 0;

    FILE *file = fopen("input.txt", "r");
    while (fscanf(file, "%d %d", &left[i], &right[i]) == 2)
    {
        i++;
    }
    fclose(file);

    qsort(left, arr_len, sizeof(int), compare);
    qsort(right, arr_len, sizeof(int), compare);

    int day_1_a = 0;

    for (int i = 0; i < arr_len; i++)
        day_1_a += abs(left[i] - right[i]);

    printf("DAY 1 (a): %d\n", day_1_a);

    int day_1_b = 0;

    for (int i = 0; i < arr_len; i++)
        day_1_b += left[i] * count_occurances(right, arr_len, left[i]);

    printf("DAY 1 (b): %d\n", day_1_b);

    return 0;
}