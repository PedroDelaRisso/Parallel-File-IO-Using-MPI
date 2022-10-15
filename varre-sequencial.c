#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define DNA_LEN 3000

#define AT pares[0]
#define GC pares[1]
#define CG pares[2]
#define TA pares[3]

typedef struct
{
    char par[2];
    long long int quantidade;
} Par_Nucleobases;

int main(int argc, char* argv[])
{
    FILE* file = fopen("dna.txt", "r");
    char* cadeia;
    long bytes;
    int i;

    Par_Nucleobases pares[4];
    strcpy(AT.par, "AT");
    strcpy(GC.par, "GC");
    strcpy(CG.par, "CG");
    strcpy(TA.par, "TA");
    AT.quantidade = 0;
    GC.quantidade = 0;
    CG.quantidade = 0;
    TA.quantidade = 0;

    fseek(file, 0L, SEEK_END);
    bytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    cadeia = (char*)calloc(bytes, sizeof(char));

    fread(cadeia, sizeof(char), bytes, file);
    fclose(file);

    for(i = 0; i < bytes; i += 2)
    {
        if(cadeia[i] == 'A') AT.quantidade++;
        if(cadeia[i] == 'G') GC.quantidade++;
        if(cadeia[i] == 'C') CG.quantidade++;
        if(cadeia[i] == 'T') TA.quantidade++;
    }

    free(cadeia);

    long long int total = (AT.quantidade + TA.quantidade + GC.quantidade + CG.quantidade) * 2;

    printf("TOTAL DE NUCLEOBASES: %lld\n\nTOTAL POR PARES:\n- AT: %lld\n- GC: %lld\n- CG: %lld\n- TA: %lld\n\nFIM\n", total, AT.quantidade, GC.quantidade, CG.quantidade, TA.quantidade);
    return 0;
}