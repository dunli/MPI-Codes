#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	//INIT MPI World
	MPI_Init(NULL, NULL);

	int rank, world_size;
	//get process rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//get world size
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if(world_size < 2)
	{
		printf("must use greater than r equal to two processors");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	const int PING_PONG_LIMIT = 10;
	int pingPongs = 0, numberOfPings = 0;
	bool sendPing = (rank == 0? true: false); // set initialization where process 0 sends and process 1 recieves.

	while(pingPongs < PING_PONG_LIMIT)
	{
		if(sendPing)
		{
			numberOfPings = pingPongs;
			printf("process with rank: %d sending number = %d \n", rank, numberOfPings);
			MPI_Send(&numberOfPings, 1, MPI_INT, (rank + 1)%2, 0, MPI_COMM_WORLD);
		} 
		else
		{
			MPI_Recv(&numberOfPings, 1, MPI_INT, (rank + 1)%2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("process with rank: %d recieved number = %d \n", rank, numberOfPings);
		}

		//Increment PingPongs after each successful send/recieve
		pingPongs++;
		//change case of send and recieve
		sendPing = !sendPing;
	}

	//Finalize MPI World
	MPI::Finalize();
}
