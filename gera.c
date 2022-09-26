#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

#define DNA_LEN 3000

int main(int argc, char* argv[])
{
    MPI_File fh;
    MPI_Status status;
    MPI_Offset fileSize;
    MPI_Offset disp;
    MPI_Request reqst;
    MPI_Info info;
    MPI_Datatype etype, filetype;
    int rank, size, i, tag = 100;
    double tempoTotal = 0, tempoPorRank = 0;
    char *pares_nucleobases[4] = { "AT", "CG", "TA", "GC" };

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sizes[] = { DNA_LEN };
    int sub_sizes[] = { DNA_LEN / size };
    int start_idxs[] = { 0 };

    MPI_Type_create_subarray(1, sizes, sub_sizes, start_idxs, MPI_ORDER_C, MPI_CHAR, &filetype);
    MPI_Type_commit(&filetype);

    etype = MPI_CHAR;

    int fracao = DNA_LEN / (size * 2);
    fileSize = DNA_LEN * sizeof(etype);
    disp = rank * (DNA_LEN / size); 

    char block_size[15];
    sprintf(block_size, "%lld", fileSize);
    char buffer_size[15];
    sprintf(buffer_size, "%lld", fileSize / size);

    MPI_Info_create(&info);
    MPI_Info_set(info, "cb_nodes", "3");
    MPI_Info_set(info, "collective_buffering", "true");
    MPI_Info_set(info, "cb_block_size", block_size);
    MPI_Info_set(info, "cb_buffer_size", buffer_size);
    MPI_Info_set(info, "striping_factor", "3");
    MPI_File_open(MPI_COMM_SELF, "dna.txt", MPI_MODE_WRONLY | MPI_MODE_CREATE, info, &fh);
    MPI_File_set_view(fh, disp, etype, filetype, "native", MPI_INFO_NULL);
    tempoTotal = MPI_Wtime();
    tempoPorRank = MPI_Wtime();
    srand(time(NULL) + rank);
    for(i = 0; i < fracao; i++)
        MPI_File_write_all(fh, pares_nucleobases[rand() % 4], 2, MPI_CHAR, &status);
    tempoPorRank = MPI_Wtime() - tempoPorRank;
    printf("Tempo de geracao do rank %d: %f segundos\n", rank, tempoPorRank);
    if(rank != 0)
    {
        bool sucesso = true;
        MPI_Send(&sucesso, 1, MPI_C_BOOL, 0, tag, MPI_COMM_WORLD);
    } else {
        bool sucessos[3] = { true, false, false };
        MPI_Recv(&sucessos[1], 1, MPI_C_BOOL, 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&sucessos[2], 1, MPI_C_BOOL, 2, tag, MPI_COMM_WORLD, &status);
        tempoTotal = MPI_Wtime() - tempoTotal;
        printf("DNA com %d nucleobases gerado em %f segundos\n", DNA_LEN, tempoTotal);
    }

    MPI_Info_free(&info);
    MPI_File_close(&fh);
    MPI_Finalize();
    return 0;
}