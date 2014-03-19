
#include <string.h>
#include <stdarg.h>
// #include "mini-printf.h"
#include "cbuf.h"

extern CircularBuffer g_LogCB;

static unsigned int
mini_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

static unsigned int
mini_itoa(int value, unsigned int radix, unsigned int uppercase,
        char *buffer, unsigned int zero_pad)
{
    char	*pbuffer = buffer;
    int	negative = 0;
    unsigned int	i, len;

    /* No support for unusual radixes. */
    if (radix > 16)
        return 0;

    if (value < 0) {
        negative = 1;
        value = -value;
    }

    /* This builds the string back to front ... */
    do {
        int digit = value % radix;
        *(pbuffer++) = (digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10);
        value /= radix;
    } while (value > 0);

    for (i = (pbuffer - buffer); i < zero_pad; i++)
        *(pbuffer++) = '0';

    if (negative)
        *(pbuffer++) = '-';

    *(pbuffer) = '\0';

    /* ... now we reverse it (could do it recursively but will
     * conserve the stack space) */
    len = (pbuffer - buffer);
    for (i = 0; i < len / 2; i++) {
        char j = buffer[i];
        buffer[i] = buffer[len-i-1];
        buffer[len-i-1] = j;
    }

    return len;
}

int
mini_vsnprintf(char *fmt, va_list va)
{
    char bf[24];
    char ch;

    int _putc(char ch)
    {
        cbWrite(&g_LogCB,  &ch);
        return 1;
    }

    int _puts(char *s, unsigned int len)
    {
        unsigned int i;

        for (i = 0; i < len; i++)
            cbWrite(&g_LogCB, &s[i]);

        return len;
    }

    while ((ch=*(fmt++))) {
        if (ch!='%')
            _putc(ch);
        else {
            char zero_pad = 0;
            char *ptr;
            unsigned int len;

            ch=*(fmt++);

            /* Zero padding requested */
            if (ch=='0') {
                ch=*(fmt++);
                if (ch == '\0')
                    goto end;
                if (ch >= '0' && ch <= '9')
                    zero_pad = ch - '0';
                ch=*(fmt++);
            }

            switch (ch) {
                case 0:
                    goto end;

                case 'u':
                case 'd':
                    len = mini_itoa(va_arg(va, unsigned int), 10, 0, bf, zero_pad);
                    _puts(bf, len);
                    break;

                case 'x':
                case 'X':
                    len = mini_itoa(va_arg(va, unsigned int), 16, (ch=='X'), bf, zero_pad);
                    _puts(bf, len);
                    break;

                case 'c' :
                    _putc((char)(va_arg(va, int)));
                    break;

                case 's' :
                    ptr = va_arg(va, char*);
                    _puts(ptr, mini_strlen(ptr));
                    break;

                default:
                    _putc(ch);
                    break;
            }
        }
    }
end:
    return 0;
}


int
mini_snprintf(char *fmt, ...)
{
    int ret;
    va_list va;

    va_start(va, fmt);
    ret = mini_vsnprintf(fmt, va);
    va_end(va);

    return ret;
}
