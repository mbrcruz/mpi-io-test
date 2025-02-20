#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#define TOTAL_FILE_SIZE 10L * 1024L * 1024L * 1024L  // 100GB
#define BLOCK_SIZE 1024L * 1024L * 1024L       // Tamanho do bloco por processo (1GB máximo por processo)
#define FILE_NAME "output_file.dat"

int main(int argc, char *argv[]) {
    MPI_File fh;
    MPI_Status status;
    int rank, size;
    MPI_Offset offset;
    long total_blocks_per_process;
    char *buf;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calcular o número total de blocos a serem escritos no arquivo
    total_blocks_per_process = TOTAL_FILE_SIZE / size;

    // Alocar o buffer de dados a ser escrito
    buf = (char*)malloc(total_blocks_per_process * sizeof(char));

    // Preencher o buffer com dados baseados no rank para distinção entre os processos
    memset(buf, rank, total_blocks_per_process);

    // Abrir o arquivo com MPI-IO
    MPI_File_open(MPI_COMM_WORLD, FILE_NAME, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

    // Calcular o deslocamento do início do bloco para o processo atual
    offset = rank * total_blocks_per_process;

    // Escrever no arquivo de forma paralela
    MPI_File_write_all(fh, buf, total_blocks_per_process, MPI_CHAR, &status);

    // Fechar o arquivo
    MPI_File_close(&fh);

    // Liberar o buffer
    free(buf);

    MPI_Finalize();
    return 0;
}
