/*
** EPITECH PROJECT, 2025
** G-MUL-100-STG-1-1-myhunter-11
** File description:
** my_int_to_str
*/

static int int_to_str_rev(int nb, char *buf)
{
    int i = 0;

    while (nb > 0) {
        buf[i] = (nb % 10) + '0';
        i++;
        nb /= 10;
    }
    return i;
}

static int add_sign(int neg, char *buf, int i)
{
    if (neg) {
        buf[i] = '-';
        i++;
    }
    return i;
}

static void reverse_buf(char *buf, int len)
{
    char tmp;

    for (int j = 0; j < len / 2; j++) {
        tmp = buf[j];
        buf[j] = buf[len - 1 - j];
        buf[len - 1 - j] = tmp;
    }
}

void my_int_to_str(int nb, char *buf)
{
    int i = 0;
    int neg = 0;

    if (nb == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    if (nb < 0) {
        neg = 1;
        nb = -nb;
    }
    i = int_to_str_rev(nb, buf);
    i = add_sign(neg, buf, i);
    buf[i] = '\0';
    reverse_buf(buf, i);
}
