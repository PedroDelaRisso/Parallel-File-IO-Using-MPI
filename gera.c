#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

#define DNA_LEN 100

int main(int argc, char* argv[])
{
    MPI_File fh;
    MPI_Request reqst;
    MPI_Offset offset;
    MPI_Status status;
    int rank, size, i, tag = 100;
    double tempo = 0;
    char *pares_nucleobases[4] = { "AT", "CG", "TA", "GC" };

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int fracoes[size];
    for(i = 0; i < size; i++)
        fracoes[i] = (DNA_LEN / size);

    if(DNA_LEN % size != 0)
        fracoes[(sizeof(fracoes) / sizeof(int)) - 1]++;

    offset = rank * (DNA_LEN * 2 / size);
    printf("[RANK %d]:\t%lld\t|\tFracao:\t%d\n", rank, offset, fracoes[rank]);

    MPI_File_open(MPI_COMM_WORLD, "dna.txt", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, offset, MPI_SEEK_SET);

    srand(time(NULL) + rank);
    for(i = 0; i < fracoes[rank]*2; i++)
    {
        int aleatorio = rand() % 4;
        char *par_aleatorio = pares_nucleobases[aleatorio];
        /**
         * ! Sem o printf abaixo não funciona no meu computador.
         * ? Eu acho que no cluster da PUC deve rodar corretamente se atualizarem o Open MPI.
         * TODO: descobrir se é isso mesmo.
         * * printf("|\tRANK %d\t|\tCICLO %d\t|\t%s\t|\n", rank, i, par_aleatorio);
        */
        MPI_File_iwrite_all(fh, par_aleatorio, 2, MPI_CHAR, &reqst);
    }

    MPI_File_close(&fh);
    MPI_Finalize();
    return 0;
}