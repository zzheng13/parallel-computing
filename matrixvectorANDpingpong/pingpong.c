#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PING_PONG_LIMIT 1000000
int main(int argc, char** argv) {
 clock_t start_t, end_t, total_t, aver_t;
  int i,m;
  double start_time, end_time,elapsed_time, time;
  
  MPI_Init(&argc, &argv);
  
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

 
   if (world_size != 2) {
    
     MPI_Abort(MPI_COMM_WORLD, 1);
  }

  

  int ping_pong_count = 0;
  start_t = clock();
   for(m=1;m<PING_PONG_LIMIT;m++) {
  	
    if (world_rank == 0) {
     
      
      MPI_Send(&ping_pong_count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(&ping_pong_count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     
    } else {
      MPI_Recv(&ping_pong_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      MPI_Send(&ping_pong_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

  }
  end_t = clock();
   total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
   printf("Total time taken by clock: %ld\n", total_t  );
   printf("Average time taken by clock: %ld\n", total_t/PING_PONG_LIMIT);
   









   start_time = MPI_Wtime();
   for(m=1;m<PING_PONG_LIMIT;m++) {
    
    if (world_rank == 0) {
     
     
      MPI_Send(&ping_pong_count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(&ping_pong_count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     
    } else {
      MPI_Recv(&ping_pong_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      MPI_Send(&ping_pong_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
   
  end_time=MPI_Wtime();
  elapsed_time = end_time - start_time;
  time = elapsed_time/PING_PONG_LIMIT;
  printf("The time in total by wtime is %lf\n", elapsed_time);
  printf("The average time by wtime is %lf\n", time);
  printf("When PING_PONG_LIMIT=1000000, the time by clock is 0.");
  printf("When PING_PONG_LIMIT=10000000, the time by clock is 2.");

   

  MPI_Finalize();

   return(0);

}