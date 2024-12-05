#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define array_len(a) ((&a)[1] - a)

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

int numbers(char *line, int *v, int arrsz)
{
    char *p = line, *e;
    int c = 0;

    while (*p)
    {
        v[c] = strtol(p, &e, 10);
        if (p == e)
            break;
        p = e;
        c++;
        if (c > arrsz)
            return 0;
    }
    return c;
}

void print_arr(int *v, int ln)
{
    printf("%d: ", ln);
    for (int i = 0; i < ln; i++)
        printf("%d ", v[i]);
    printf("\n");
}

int unsafe(int *v, int arrsz)
{
    if (arrsz < 2)
        return -1;

    int order = v[0] - v[1];

    if (order == 0)
        return 0;

    for (int i = 1; i < arrsz; i++)
    {
        int curr_order = v[i - 1] - v[i];
        if (curr_order == 0 || (order > 0 && curr_order < 0) || (order < 0 && curr_order > 0) || abs(curr_order) > 3)
        {
            return i - 1;
        }
    }

    return -1;
}

int search_pattern2(const char *a, int asz, const char *pt, int ptsz)
{
    int count = 0;
    const char *ptr = a;
    const char *end = ptr + asz;

    if (asz < ptsz)
        return 0;

    while (ptr < end)
    {
        const char *found = memmem(ptr, end - ptr, pt, ptsz);
        if (!found)
            break;
        count++;
        ptr = found + ptsz;
    }

    return count;
}

int search_pattern(const char *a, int asz, const char *pt, int ptsz)
{
    int count = 0;
    const char *ptr = a;
    const char *end = ptr + asz;

    while ((ptr = strstr(ptr, pt)))
    {
        count++;
        ptr += ptsz;
    }

    return count;
}

int search_pattern3(int row, int col, int n, char c[][n])
{
    char x[3][3];

    for (int i = row; i < row + 3; i++)
    {
        for (int j = col; j < col + 3; j++)
        {
            if (i < n && j < n)
            {
                x[i - row][j - col] = c[i][j];
            }
        }
    }

    if (x[1][1] != 'A')
        return 0;

    if (((x[0][0] == 'M' && x[2][2] == 'S') || (x[0][0] == 'S' && x[2][2] == 'M')) &&
        ((x[0][2] == 'M' && x[2][0] == 'S') || (x[0][2] == 'S' && x[2][0] == 'M')))
    {
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    /* day 1 */

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

    /* day 2 */

    char line[256];
    int safe_level = 0;

    file = fopen("input_d2.txt", "r");
    while (fgets(line, sizeof(line), file))
    {
        int v[9], c = numbers(line, v, array_len(v));

        if (unsafe(v, c) == -1)
            safe_level++;
    }
    fclose(file);

    printf("DAY 2 (a): %d\n", safe_level);

    safe_level = 0;
    file = fopen("input_d2.txt", "r");
    while (fgets(line, sizeof(line), file))
    {
        int v[9], c = numbers(line, v, array_len(v));

        int uidx = unsafe(v, c);
        if (uidx != -1)
        {

            int m = 0;
            do
            {
                int vd[c - 1], j = 0;
                for (int i = 0; i < c; i++)
                {
                    if (i != m)
                        vd[j++] = v[i];
                }
                if (unsafe(vd, c - 1) == -1)
                {
                    safe_level++;
                    break;
                }

            } while (++m < c);
        }
        else
            safe_level++;
    }
    fclose(file);

    printf("DAY 2 (b): %d\n", safe_level);

    /* day 3 */

    {
        int inmul = 0, diga = 0, digb = 0;
        int c, *digp = &diga;
        int day_3_a = 0;
        int mul_state = 0;

        file = fopen("input_d3.txt", "r");
        while ((c = fgetc(file)) != EOF)
        {
            if (inmul)
            {
                if (isdigit(c) || c == ',' || c == ')')
                {
                    if (c == ',')
                    {
                        digp = &digb;
                    }
                    else if (c == ')')
                    {
                        day_3_a += diga * digb;
                        inmul = 0;
                        diga = digb = 0;
                        digp = &diga;
                    }
                    else
                    {
                        *digp = *digp * 10 + (c - '0');
                    }
                    continue;
                }
                else
                {
                    inmul = 0;
                    diga = digb = 0;
                    digp = &diga;
                }
            }

            if (mul_state == 0 && c == 'm')
            {
                mul_state = 1;
            }
            else if (mul_state == 1 && c == 'u')
            {
                mul_state = 2;
            }
            else if (mul_state == 2 && c == 'l')
            {
                mul_state = 3;
            }
            else if (mul_state == 3 && c == '(')
            {
                inmul = 1;
                mul_state = 0;
            }
            else
            {
                mul_state = 0;
            }
        }

        fclose(file);
        printf("DAY 3 (a): %d\n", day_3_a);
    }

    {
        int inmul = 0, diga = 0, digb = 0, indo = -1, indont = 0;
        int c, *digp = &diga;
        int day_3_b = 0;
        int mul_state = 0, do_state = 0, dont_state = 0;

        file = fopen("input_d3.txt", "r");
        while ((c = fgetc(file)) != EOF)
        {
            if (inmul)
            {
                if (isdigit(c) || c == ',' || c == ')')
                {
                    if (c == ',')
                    {
                        digp = &digb;
                    }
                    else if (c == ')')
                    {
                        if (!indont || indo == -1)
                            day_3_b += diga * digb;

                        inmul = 0;
                        diga = digb = 0;
                        digp = &diga;
                    }
                    else
                    {
                        *digp = *digp * 10 + (c - '0');
                    }
                    continue;
                }
                else
                {
                    inmul = 0;
                    diga = digb = 0;
                    digp = &diga;
                }
            }

            if (mul_state == 0 && c == 'm')
            {
                mul_state = 1;
            }
            else if (mul_state == 1 && c == 'u')
            {
                mul_state = 2;
            }
            else if (mul_state == 2 && c == 'l')
            {
                mul_state = 3;
            }
            else if (mul_state == 3 && c == '(')
            {
                inmul = 1;
                mul_state = 0;
            }
            else
            {
                mul_state = 0;
            }

            if (do_state == 0 && c == 'd')
            {
                do_state = 1;
            }
            else if (do_state == 1 && c == 'o')
            {
                do_state = 2;
            }
            else if (do_state == 2 && c == '(')
            {
                do_state = 3;
            }
            else if (do_state == 3 && c == ')')
            {
                indo = 1;
                indont = 0;
                do_state = 0;
            }
            else
            {
                do_state = 0;
            }

            if (dont_state == 0 && c == 'd')
            {
                dont_state = 1;
            }
            else if (dont_state == 1 && c == 'o')
            {
                dont_state = 2;
            }
            else if (dont_state == 2 && c == 'n')
            {
                dont_state = 3;
            }
            else if (dont_state == 3 && c == '\'')
            {
                dont_state = 4;
            }
            else if (dont_state == 4 && c == 't')
            {
                dont_state = 5;
            }
            else if (dont_state == 5 && c == '(')
            {
                dont_state = 6;
            }
            else if (dont_state == 6 && c == ')')
            {
                indont = 1;
                indo = 0;
                dont_state = 0;
            }
            else
            {
                dont_state = 0;
            }
        }

        fclose(file);
        printf("DAY 3 (b): %d\n", day_3_b);
    }

    /* day 4 */

    {
        int day_4_a = 0, day_4_b = 0;
        int i = 0;
#define mx_arr_sz 140
        char line[mx_arr_sz + 2];
        char mx[mx_arr_sz][mx_arr_sz];
        file = fopen("input_d4.txt", "r");
        while (fgets(line, sizeof(line), file))
        {
            day_4_a += search_pattern(line, mx_arr_sz, "XMAS", 4);
            day_4_a += search_pattern(line, mx_arr_sz, "SAMX", 4);

            memcpy(mx[i++], line, mx_arr_sz);
        }
        fclose(file);

        // columns

        for (int j = 0; j < mx_arr_sz; j++)
        {
            char t[mx_arr_sz << 2];
            int tl = 0;
            for (int k = 0; k < mx_arr_sz; k++)
            {
                t[tl++] = mx[k][j];
            }
            t[tl] = 0;

            day_4_a += search_pattern(t, tl, "XMAS", 4);
            day_4_a += search_pattern(t, tl, "SAMX", 4);
        }

        // left-top->right-bottom

        for (int j = 0; j < mx_arr_sz; j++)
        {
            char t[mx_arr_sz << 2];
            int tl = 0;
            for (int k = 0; k <= j; k++)
            {
                t[tl++] = mx[k][j - k];
            }
            t[tl] = 0;

            day_4_a += search_pattern(t, tl, "XMAS", 4);
            day_4_a += search_pattern(t, tl, "SAMX", 4);
        }

        for (int j = 1; j < mx_arr_sz; j++)
        {
            char t[mx_arr_sz << 2];
            int tl = 0;
            for (int k = 0; k < mx_arr_sz - j; k++)
            {
                t[tl++] = mx[j + k][mx_arr_sz - 1 - k];
            }
            t[tl] = 0;

            day_4_a += search_pattern(t, tl, "XMAS", 4);
            day_4_a += search_pattern(t, tl, "SAMX", 4);
        }

        // right-top->left-bottom

        for (int j = 0; j < mx_arr_sz; j++)
        {
            char t[mx_arr_sz << 2];
            int tl = 0;
            for (int k = 0; k <= j; k++)
            {
                t[tl++] = mx[k][mx_arr_sz - 1 - j + k];
            }
            t[tl] = 0;

            day_4_a += search_pattern(t, tl, "XMAS", 4);
            day_4_a += search_pattern(t, tl, "SAMX", 4);
        }

        for (int j = 1; j < mx_arr_sz; j++)
        {
            char t[mx_arr_sz << 2];
            int tl = 0;
            for (int k = 0; k < mx_arr_sz - j; k++)
            {
                t[tl++] = mx[j + k][k];
            }
            t[tl] = 0;

            day_4_a += search_pattern(t, tl, "XMAS", 4);
            day_4_a += search_pattern(t, tl, "SAMX", 4);
        }

        printf("DAY 4 (a): %d\n", day_4_a);

        for (int j = 0; j <= mx_arr_sz - 3; j++)
        {
            for (int k = 0; k <= mx_arr_sz - 3; k++)
            {
                if (search_pattern3(j, k, mx_arr_sz, mx))
                {
                    day_4_b++;
                }
            }
        }

        printf("DAY 4 (b): %d\n", day_4_b);
    }

    return 0;
}