#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#define FILE_SIZE 100L * 1024L * 1024L * 1024L  // 100GB
#define BLOCK_SIZE 1024L * 1024L * 1024L       // 1GB por processo

int main(int argc, char *argv[]) {
    MPI_File fh;
    MPI_Status status;
    int rank, size;
    MPI_Offset offset;
    char *buf;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Buffer para dados a serem escritos
    buf = (char*)malloc(BLOCK_SIZE * sizeof(char));

    // Preencher o buffer com algum valor
    memset(buf, rank, BLOCK_SIZE);

    // Abrir o arquivo com MPI-IO
    MPI_File_open(MPI_COMM_WORLD, "output_file.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

    // Calcular o deslocamento para o início do bloco do processo atual
    offset = rank * BLOCK_SIZE;

    // Escrever no arquivo de forma paralela
    MPI_File_write_at(fh, offset, buf, BLOCK_SIZE, MPI_CHAR, &status);

    // Fechar o arquivo
    MPI_File_close(&fh);

    // Liberar o buffer
    free(buf);

    MPI_Finalize();
    return 0;
}
