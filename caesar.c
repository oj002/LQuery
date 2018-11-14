#include <curl/curl.h>

#define CASSERT_OK(x) \
{ \
    CURLcode res = (x); \
    if (res != CURLE_OK) { \
        fprintf(stderr, "%s\n", curl_easy_strerror(res)); \
        exit(1); \
    } \
}
#define CASSERT_SETOPT(...) \
CASSERT_OK(curl_easy_setopt(__VA_ARGS__));

size_t curl_write_to_string(void *ptr, size_t size, size_t nmemb, string_t *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (!s->ptr) printf("realloc() failed\n"), exit(-1);
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

#define CAESAR_URL_PREF "https://www.frag-caesar.de/lateinwoerterbuch/"
#define CAESAR_URL_SUFF "-uebersetzung.html"
string_t convert_to_url(const char *infinitiv)
{
    string_t str;
    str.len = strlen(infinitiv) + sizeof(CAESAR_URL_PREF) + sizeof(CAESAR_URL_SUFF);
    str.ptr = (char*)malloc(str.len);
    strcpy(str.ptr, CAESAR_URL_PREF);
    strcat(str.ptr, infinitiv);
    strcat(str.ptr, CAESAR_URL_SUFF);
    return str;
}

void store_dictionary(const char *infinitiv, const char *path)
{
    CURL *curl = curl_easy_init();  
    if (!curl) 
        fprintf(stderr, "Curl init failed\n"), exit(128);


    string_t file_str;
    string_init(&file_str); 
    string_t name = convert_to_url(infinitiv);
    CASSERT_SETOPT(curl, CURLOPT_URL, name.ptr);
    // CASSERT_SETOPT(curl, CURLOPT_FOLLOWLOCATION, 1L);
    CASSERT_SETOPT(curl, CURLOPT_WRITEFUNCTION, curl_write_to_string);
    CASSERT_SETOPT(curl, CURLOPT_WRITEDATA, &file_str);
    CASSERT_OK(curl_easy_perform(curl));

    curl_easy_cleanup(curl);
    free(name.ptr);


    string_t data;
    string_init(&data);
    char *it = strstr(file_str.ptr, "Aufstellung aller Formen");
    char *end = strstr(file_str.ptr, "Supina");
    while (it != end)
    {
        switch (*it)
        {
        case '\t': case '\r': ++it; break;
        case '\n':
            string_push(&data, '\n');
            do { ++it; }
            while (*it == '\n' || *it == '\r' || *it == ' ' || *it == '\t');
            break;
        case '<':
            do { ++it; } while (*it != '>');
            ++it; break;
        case '&':
            if (it[2] == 'u' && it[3] == 'm' && it[4] == 'l')
            {
                int uml = 'ä';
                switch (it[1])
                {
                case 'o': uml = 'ö'; break;
                case 'u': uml = 'ü'; break;
                }
                string_push(&data, (char)(uml >> 8));
                string_push(&data, (char)uml);
                it += 5;
            }
            else while (*it != ';') it++;
            ++it; break;
        default: string_push(&data, *it++); break;
        }
    }
    
    it = strstr(data.ptr, "Präsens Indikativ");
    end = data.ptr + data.len;
    #define TO_NEXT_LINE() \
        while (*it != '\n') ++it; \
        while (*it == ' ' \
            || *it == '\n' \
            || *it == '\t' \
            || *it == '\r') \
        { ++it; }
    char *str = malloc(strlen(infinitiv) + strlen(path));
    strcpy(str, path);
    strcat(str, infinitiv);
    FILE *fout = fopen("dict/esse", "w");
    free(str);

    for (size_t i = 0; i < 10; ++i)
    {
        TO_NEXT_LINE();
        for (size_t j = 0; j < 2; ++j)
        {
            TO_NEXT_LINE();
            for (size_t k = 0; k < 2*3; ++k)
            {
                TO_NEXT_LINE();
                char *old = it;
                it = strchr(it, '\n') + 1;
                it = strchr(it, '\n') + 1;
                fprintf(fout, "%.*s", it - old, old);
                TO_NEXT_LINE();
            }
        }
    }
    #undef TO_NEXT_LINE
    free(data.ptr);
    fclose(fout);
}
