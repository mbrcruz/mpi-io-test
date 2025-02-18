       _|  (     /   Amazon Linux AMI
      ___|\___|___|

https://aws.amazon.com/amazon-linux-ami/2018.03-release-notes/
136 package(s) needed for security, out of 176 available
Run "sudo yum update" to apply all updates.
[root@sydney ~]# screen -dR -S Marcelo
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define VECTOR_SIZE 1000000000  // Tamanho total do vetor

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_File file;
    MPI_Status status;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determinar tamanho da parte de cada processo
    int local_size = VECTOR_SIZE / size;
    if (rank == size - 1) {
        local_size += VECTOR_SIZE % size;  // Último processo recebe o resto
    }

    // Alocar e inicializar vetor local
    int *local_vector = (int *)malloc(local_size * sizeof(int));
    for (int i = 0; i < local_size; i++) {
        local_vector[i] = rank * 1000 + i;  // Preenchendo com valores distintos
    }

    // Abrir arquivo para escrita
    MPI_File_open(MPI_COMM_WORLD, "output.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    // Definir deslocamento baseado na posição do processo
    offset = rank * (VECTOR_SIZE / size) * sizeof(int);

    if(rank != 2){
        // Escrever os dados em paralelo
        printf("Starting Rank=%d , offset=%d\n",  rank , offset);
        MPI_File_write_all(file, local_vector, local_size, MPI_INT, &status);
        printf("Finish Rank=%d , offset=%d\n",  rank , offset);
    }
    // Fechar arquivo
    MPI_File_close(&file);

    // Liberar memória
    free(local_vector);

    MPI_Finalize();
    return 0;
}