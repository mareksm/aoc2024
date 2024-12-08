#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>
#include <stdint.h>
#include <inttypes.h>

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

void combinations(int *arr, int n, int pos, int concat, void *ctx, void (*test)(void *, int *, int))
{
    if (pos == n)
    {
        test(ctx, arr, n);
        return;
    }

    arr[pos] = '+';
    combinations(arr, n, pos + 1, concat, ctx, test);

    arr[pos] = '*';
    combinations(arr, n, pos + 1, concat, ctx, test);

    if (concat)
    {
        arr[pos] = '|';
        combinations(arr, n, pos + 1, concat, ctx, test);
    }
}

void print_arr(int *v, int ln)
{
    printf("%d: ", ln);
    for (int i = 0; i < ln; i++)
        printf("%d ", v[i]);
    printf("\n");
}

void print_arr_char(int *v, int ln)
{
    printf(" ");
    for (int i = 0; i < ln; i++)
        printf("%c ", v[i]);
    printf("\n");
}

#define print_arr2d(_arr, _sz)           \
    for (int _i = 0; _i < _sz; _i++)     \
    {                                    \
        for (int _j = 0; _j < _sz; _j++) \
            printf("%c ", _arr[_i][_j]); \
        printf("\n");                    \
    }

#define copy_arr2d(_arr, _sz, _arrc, _pp) \
    for (int _i = 0; _i < _sz; _i++)      \
        for (int _j = 0; _j < _sz; _j++)  \
            _arrc[_i][_j] = _pp ? _pp : _arr[_i][_j];

int64_t combine_numbers(int64_t a, int64_t b)
{
    char r[128];
    snprintf(r, sizeof(r), "%" PRId64 "%" PRId64, a, b);
    return strtoll(r, 0, 10);
}

int64_t solve(int *n, int *o, int l, int prec, int concat)
{
    int64_t r = 0;

    if (prec)
    {
        for (int i = 0; i < l - 1; i++)
        {
            if (o[i] == '*')
            {
                n[i + 1] = n[i] * n[i + 1];
                n[i] = 0;
            }
        }

        for (int i = 0; i < l; i++)
        {
            r += n[i];
        }
    }
    else if (concat)
    {
        r = n[0];
        for (int i = 0; i < l - 1; i++)
        {
            if (o[i] == '+')
            {
                r += n[i + 1];
            }
            else if (o[i] == '*')
            {
                r *= n[i + 1];
            }
            else if (o[i] == '|')
            {
                r = combine_numbers(r, n[i + 1]);
            }
        }
    }
    else
    {
        r = n[0];
        for (int i = 0; i < l - 1; i++)
        {
            if (o[i] == '+')
            {
                r += n[i + 1];
            }
            else if (o[i] == '*')
            {
                r *= n[i + 1];
            }
        }
    }

    return r;
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

    /* day 5 */
    {
        char line[256];
        char *p;
#define por_len 1176
        struct day5
        {
            int page[2];
        } por[por_len];
        int por_ln = 0;
        int day_5_a = 0, day_5_b = 0;

        int in_rule_forw(int a, int b)
        {
            for (int j = 0; j < por_ln; j++)
                if (por[j].page[0] == a && por[j].page[1] == b)
                    return 1;
            return 0;
        };

        int in_rule_back(int a, int b)
        {
            for (int j = 0; j < por_ln; j++)
                if (por[j].page[1] == a && por[j].page[0] == b)
                    return 1;
            return 0;
        };

        int in_rule(int *pages, int pages_ln)
        {
            int forw_err = 0, back_err = 0;
            if (pages_ln == 0)
                return -1;

            for (int i = 0; i < pages_ln; i++)
            {
                for (int j = i + 1; j < pages_ln; j++)
                {
                    if (!in_rule_forw(pages[i], pages[j]))
                    {
                        forw_err++;
                        break;
                    }
                }

                for (int j = i - 1; j >= 0; --j)
                {
                    if (!in_rule_back(pages[i], pages[j]))
                    {
                        back_err++;
                        break;
                    }
                }
            }

            if (!forw_err && !back_err)
            {
                return pages[pages_ln / 2];
            }
            return -1;
        };

        file = fopen("input_d5.txt", "r");
        while (fgets(line, sizeof(line), file))
        {
            if ((p = strchr(line, '|')))
            {
                *p++ = 0;
                por[por_ln].page[0] = strtol(line, 0, 10);
                por[por_ln++].page[1] = strtol(p, 0, 10);
            }
            else if (strchr(line, ','))
            {
                char *end;
                int pages[32];
                int pages_ln = 0;
                p = line;
                while (*p)
                {
                    int num = strtol(p, &end, 10);
                    if (p != end)
                    {
                        pages[pages_ln++] = num;
                        p = end;
                    }
                    else
                        p++;
                }

                int in = in_rule(pages, pages_ln);
                if (in != -1)
                {
                    day_5_a += in;
                }
                else
                {
                    int offs = pages_ln;

                    do
                    {
                        for (int i = 0; i < pages_ln - 1; i++)
                        {
                            for (int j = i + 1; j < pages_ln; j++)
                            {
                                if (!in_rule_forw(pages[i], pages[j]))
                                {
                                    int t = pages[i];
                                    pages[i] = pages[i + 1];
                                    pages[i + 1] = t;
                                    break;
                                }
                            }
                        }

                    } while (--offs > 0);

                    in = in_rule(pages, pages_ln);
                    if (in != -1)
                    {
                        day_5_b += in;
                    }
                }
            }
        }
        fclose(file);

        printf("DAY 5 (a): %d\n", day_5_a);
        printf("DAY 5 (b): %d\n", day_5_b);
    }

    /* day 6 */
    {
        char line[256], *p;
        int i = 0, obstcount = 0;
#define guard_map_sz 130
        char mx[guard_map_sz][guard_map_sz];
        int x = 0, y = 0; /* col , row */
        file = fopen("input_d6t.txt", "r");
        while (fgets(line, sizeof(line), file))
        {
            p = strchr(line, '^');
            if (p)
            {
                x = p - line;
                y = i;
            }
            memcpy(mx[i++], line, strlen(line));

            for (int j = 0; line[j] != 0; j++)
                if (line[j] == '#')
                    obstcount++;
        }
        fclose(file);

        int max_x = i, max_y = i;

        struct visited
        {
            int x;
            int y;
        } vis[max_x * max_y];

        int vis_ln = 0;

        void add_pos(int xx, int yy)
        {
            for (int k = 0; k < vis_ln; k++)
            {
                if (vis[k].x == xx && vis[k].y == yy)
                    return;
            }

            vis[vis_ln].x = xx;
            vis[vis_ln].y = yy;
            vis_ln++;
        };

        int walk(char(*arr)[guard_map_sz][guard_map_sz], int max_xx, int max_yy, int xx, int yy)
        {
            char cont = 1, dir = 0; /* up */
            int steps = 0;
            int max_steps = max_xx * max_yy - 2 - obstcount;

            memset(vis, 0, sizeof(vis));
            vis_ln = 0;

            do
            {
                if (steps++ > max_steps)
                    return -1;

                switch (dir)
                {
                case 0: /* up */
                    if (yy < 0)
                    {
                        cont = 0;
                    }
                    if ((*arr)[yy][xx] == '#')
                    {
                        dir = 1;
                        yy++;
                    }
                    else
                    {
                        add_pos(xx, yy);
                        yy--;
                    }
                    break;
                case 1: /* right */
                    if (xx == max_xx)
                    {
                        cont = 0;
                    }
                    else if ((*arr)[yy][xx] == '#')
                    {
                        dir = 2;
                        xx--;
                    }
                    else
                    {
                        add_pos(xx, yy);
                        xx++;
                    }
                    break;
                case 2: /* down */
                    if (yy == max_yy)
                    {
                        cont = 0;
                    }
                    else if ((*arr)[yy][xx] == '#')
                    {
                        dir = 3;
                        yy--;
                    }
                    else
                    {
                        add_pos(xx, yy);
                        yy++;
                    }
                    break;
                case 3: /* left */
                    if (xx < 0)
                    {
                        cont = 0;
                    }
                    else if ((*arr)[yy][xx] == '#')
                    {
                        dir = 0;
                        xx++;
                    }
                    else
                    {
                        add_pos(xx, yy);
                        xx--;
                    }
                    break;
                }

            } while (cont);

            return 0;
        };

        int x_start = x, y_start = y;

        add_pos(x_start, y_start);

        walk(&mx, max_x, max_y, x_start, y_start);

        printf("DAY 6 (a): %d\n", vis_ln);

        int day_6_b = 0;

#pragma omp parallel for firstprivate(mx) reduction(+ : day_6_b)
        for (int r = 0; r < max_y; r++)
        {
            for (int c = 0; c < max_x; c++)
            {
                char o = mx[r][c];
                if (o == '#' || (r == y_start && c == x_start))
                    continue;

                mx[r][c] = '#';

                int v = walk(&mx, max_x, max_y, x_start, y_start);
                if (v == -1)
                    day_6_b += 1;

                mx[r][c] = o;
            }
        }

        printf("DAY 6 (b): %d\n", day_6_b);
    }

    /* day 7 */
    {
        char line[256], *p, *e;
        int arr[32], c = 0;
        int64_t day_7_a = 0, day_7_b = 0;

        struct day7
        {
            int64_t s;
            int *a;
            int al;

            int set;
            int concat;
        };

        void test(void *ctx, int *a, int al)
        {
            struct day7 *c = ctx;

            int a_copy[c->al];
            memcpy(a_copy, c->a, c->al * sizeof(int));

            int64_t r = solve(a_copy, a, c->al, 0, c->concat);

            if (r == c->s && !c->set)
                c->set = 1;
        };

        file = fopen("input_d7t.txt", "r");
        while (fgets(line, sizeof(line), file))
        {
            int64_t s = strtoll(line, &e, 10);
            c = numbers(e + 2, arr, array_len(arr));

            int op[c - 1];
            struct day7 ctx = {.s = s, .a = arr, .al = c, .set = 0, .concat = 0};
            combinations(op, c - 1, 0, ctx.concat, &ctx, test);

            if (ctx.set)
            {
                day_7_a += s;
            }
            else
            {
                struct day7 ctx_b = {.s = s, .a = arr, .al = c, .set = 0, .concat = 1};
                combinations(op, c - 1, 0, ctx_b.concat, &ctx_b, test);

                if (ctx_b.set)
                    day_7_b += s;
            }
        }
        fclose(file);

        printf("DAY 7 (a): %" PRId64 "\n", day_7_a);
        printf("DAY 7 (b): %" PRId64 "\n", day_7_a + day_7_b);
    }

    /* day 8 */
    {
#define day8asz 50
        int mx[day8asz][day8asz], mx_sz = 0, mxc = 0, c;
        file = fopen("input_d8t.txt", "r");
        while ((c = fgetc(file)) != EOF)
        {
            if (c == '\n' || c == '\r')
            {
                mx_sz++;
                mxc = 0;
            }
            else
            {
                int bytes;
                if ((c & 0x80) == 0)
                {
                    bytes = 1;
                }
                else if ((c & 0xE0) == 0xC0)
                {
                    bytes = 2;
                }
                else if ((c & 0xF0) == 0xE0)
                {
                    bytes = 3;
                }
                else if ((c & 0xF8) == 0xF0)
                {
                    bytes = 4;
                }
                else
                {
                    continue;
                }

                int ch = c;
                for (int i = 1; i < bytes; i++)
                {
                    c = fgetc(file);
                    if (c == EOF)
                        break;
                    ch = (ch << 8) | c;
                }

                mx[mx_sz][mxc++] = ch;
            }
        }

        if (mx_sz)
            mx_sz++;

        fclose(file);

        int mxres[day8asz][day8asz];
        copy_arr2d(mx, mx_sz, mxres, '.');

        int find(int(*arr)[day8asz][day8asz], int asz, int cr, int cc, int sym)
        {
            for (int ii = cr; ii < asz; ii++)
            {
                for (int jj = 0; jj < asz; jj++)
                {
                    int dys = cr - ii;
                    int dy = abs(dys);
                    int dxs = cc - jj;
                    int dx = abs(dxs);

                    if (!(dx < 1 && dy < 1) && (*arr)[ii][jj] == sym)
                    {
                        int a_x_1 = jj + dx * (dxs < 0 ? -2 : 2);
                        int a_y_1 = ii + dy * (dys < 0 ? -2 : 2);

                        int a_x_2 = jj - dx * (dxs < 0 ? -1 : 1);
                        int a_y_2 = ii - dy * (dys < 0 ? -1 : 1);

                        if (a_x_1 >= 0 && a_x_1 < asz && a_y_1 >= 0 && a_y_1 < asz)
                        {
                            mxres[a_y_1][a_x_1] = '#';
                        }
                        if (a_x_2 >= 0 && a_x_2 < asz && a_y_2 >= 0 && a_y_2 < asz)
                        {
                            mxres[a_y_2][a_x_2] = '#';
                        }
                    }
                }
            }
        };

        for (int i = 0; i < mx_sz; i++)
            for (int j = 0; j < mx_sz; j++)
                if (mx[i][j] != '.')
                    find(&mx, mx_sz, i, j, mx[i][j]);

        print_arr2d(mxres, mx_sz);

        int day_8_a = 0;
        for (int i = 0; i < mx_sz; i++)
            for (int j = 0; j < mx_sz; j++)
                if (mxres[i][j] == '#')
                    day_8_a++;

        printf("DAY 8 (a): %d\n", day_8_a);

        int mxres2[day8asz][day8asz];
        copy_arr2d(mx, mx_sz, mxres2, '.');

        int find2(int(*arr)[day8asz][day8asz], int asz, int cr, int cc, int sym)
        {
            for (int ii = cr; ii < asz; ii++)
            {
                for (int jj = 0; jj < asz; jj++)
                {
                    int dys = cr - ii;
                    int dy = abs(dys);
                    int dxs = cc - jj;
                    int dx = abs(dxs);

                    if (!(dx < 1 && dy < 1) && (*arr)[ii][jj] == sym)
                    {
                        for (int k = -asz; k < asz; k++)
                        {
                            int a_x_2 = jj + dxs * k;
                            int a_y_2 = ii + dys * k;

                            if (a_x_2 >= 0 && a_x_2 < asz && a_y_2 >= 0 && a_y_2 < asz)
                            {
                                mxres2[a_y_2][a_x_2] = '#';
                            }
                        }
                    }
                }
            }
        };

        for (int i = 0; i < mx_sz; i++)
            for (int j = 0; j < mx_sz; j++)
                if (mx[i][j] != '.')
                    find2(&mx, mx_sz, i, j, mx[i][j]);

        print_arr2d(mxres2, mx_sz);

        int day_8_b = 0;
        for (int i = 0; i < mx_sz; i++)
            for (int j = 0; j < mx_sz; j++)
                if (mxres2[i][j] == '#')
                    day_8_b++;

        printf("DAY 8 (b): %d\n", day_8_b);
    }

    return 0;
}

// cc -g -o t main.c -fopenmp -std=gnu2x