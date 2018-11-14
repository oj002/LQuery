#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common.c"

#include "caesar.c"

typedef enum person {
    SINGULAR = 0,
    PLURAL = 6,
} person_t;
typedef enum genus {
    AKTIV = 0,
    PASSIV = 12,
} genus_t;
typedef enum modi {
    INDIKATIV = 0,
    KONJUNKTIV = 24,
} modi_t;
// 2 * 6Pers * 2Genus * 2Modi
typedef enum tempus {
    PRAESENS        = 0,
    IMPERFEKT       = 24 * 2,
    FUTUR_I         = 24 * 4,
    PERFEKT         = 24 * 6,
    PLUSQUALPERFEKT = 24 * 8,
    FUTUR_II        = 24 * 10,
} tempus_t;

#define DIC_INDEX(t, m, g, n, p) ((t) + (m)+ (g) + (n) * 2 + (p))

void gen_verb_tabel(const char *infinitiv)
{
}

int main()
{
    printf("Please enter a verb in infinitive:\n");
    char verb[100];
    scanf("%s", verb); 
    store_dictionary(verb, "dict/");
    char path[256];
    strcpy(path, "dict/");
    strcat(path, verb);
    FILE *fin = fopen(path, "r");
    #define DIC_SIZE (FUTUR_II + IMPERFEKT)
    #define DIC_MAX_LEN (50)
    char dic[DIC_SIZE][DIC_MAX_LEN];
    
    char (*it)[50] = dic;

    for (; it != (dic + FUTUR_I + KONJUNKTIV); ++it)
    {
        if (fgets(*it, DIC_MAX_LEN, fin) == 0) puts("error"), exit(-1);
        char *p;
        if((p = strchr(*it, '\n')) != NULL) *p = '\0';
        // else input to long for buffer
    }
    for (; it != (dic + PERFEKT); ++it)
        memcpy(*it, "null", 5);
    for (; it != (dic + FUTUR_II + KONJUNKTIV); ++it)
    {
        if (fgets(*it, DIC_MAX_LEN, fin) == 0) puts("error"), exit(-1);
        char *p;
        if((p = strchr(*it, '\n')) != NULL) *p = '\0';
        // else input to long for buffer
    }
    for (; it != (dic + DIC_SIZE); ++it)
        memcpy(*it, "null", 5);
    int nump, tmp;
    person_t p;
    printf("Pleas enter the person (1-3): ");
    if (scanf("%d", &nump) != 1) puts("error"), exit(-1);
    nump--;

    printf("Singular or Plural (1 | 2): ");
    if (scanf("%d", &tmp) != 1) puts("error"), exit(-1);

    if (tmp == 1) p = SINGULAR;
    else if (tmp == 2) p = PLURAL;
    else exit(-1);
    for (size_t t = 0; t < 6; ++t) {
        printf("%25s|%25s|%25s|%25s\n",
                dic[DIC_INDEX(t * IMPERFEKT, INDIKATIV, AKTIV, nump, p)],
                dic[DIC_INDEX(t * IMPERFEKT, KONJUNKTIV, AKTIV, nump, p)],
                dic[DIC_INDEX(t * IMPERFEKT, INDIKATIV, PASSIV, nump, p)],
                dic[DIC_INDEX(t * IMPERFEKT, KONJUNKTIV, PASSIV, nump, p)]);
    }
    //puts(dic[DIC_INDEX(PRAESENS, KONJUNKTIV, PASSIV, nump, p)]);
    //(t, m, g, n, p) ((t) + (m)+ (g) + (n) * 2 + (p))
    return 0;
}
