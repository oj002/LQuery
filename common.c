#define STRING_BLOCK_SIZE 1024

typedef struct string {
    char *ptr;
    size_t len, allocated;
} string_t;
void string_init(string_t *s)
{
    s->ptr = malloc(1);
    s->ptr[0] = '\0';
    s->len = 0;
    s->allocated = 0;
}
void string_push(string_t *s, char c)
{
    if (s->allocated)
    {
        if (s->len >= s->allocated) {
            s->allocated += STRING_BLOCK_SIZE;
            s->ptr = (char*)realloc(s->ptr, s->allocated);
        }
        s->ptr[s->len++] = c;
        s->ptr[s->len] = '\0';
    }
    else
    {
        s->ptr = (char*)malloc(STRING_BLOCK_SIZE);
        s->len = 0;
        s->allocated = STRING_BLOCK_SIZE;
        s->ptr[0] = '\0';
    }
}
