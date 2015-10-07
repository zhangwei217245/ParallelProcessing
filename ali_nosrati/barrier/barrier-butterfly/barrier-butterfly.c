#include "my_barrier.h"
#include <math.h>
#include "util.h"


int my_barrier(MPI_Comm comm){
    int world_size, world_rank;
    int i = 0, step = 0;
    
    MPI_Status status;
    
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the processor
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);	
    
    all_steps = (log10(world_size) / log10(2)) ;

    while(step <= log)



    
    int totalSteps = (int)lg((double) world_size);
    for (i = 1; i <= totalSteps; i++){
            int pow_2 = (int)pow((double)2, (double)i);
            int mid = world_size / pow_2;
            int addr = 0;
            if (world_rank % last_mid < mid) {
                    addr = world_rank + mid;
            } else {
                    addr = world_rank - mid;
            }
            int x;
            MPI_Send(&flag, 1, MPI_INT, addr, 0, MPI_COMM_WORLD);
            MPI_Recv(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD, &status);
            flag = flag + x;
            last_mid = mid;
    }
    printf("[%s]: Butterfly Barrier done for #%d.\n", getTimeString(), world_rank);
    while (1==1) {
            if (flag == world_size){
                    break;
            }
    }
    return 0;
}
