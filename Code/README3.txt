README FOR Q3 (POLLING BOOTH)

To run :
	- make
	- ./3

Implementaion :

	- struct for booth which contains basic info like its id,mutex locks,conditional variables and other variables.
	- For every booth a thread for every EVM and every robot in that booth.
	- a thread for each EVM and every robot in that booth.
	- For Synchronising EVM's within the booth we used cond. var and a lock.
	- For Synchonising  Robots in the Booth we used cond. var and a lock.
	- Each EVM waits for signal by others for its voting turn.
	- Each Robot also waits for EVM on cond. var until it is signaled.
	- Once the EVM is signaled, All the users on it gives votes.
	- Once a robot is signaled an EVM os assigned to it for voting.
	- Program ends when all robots are done with voting.
