#include "Build Settings.h"

#define MAIN 0
#define MANAGER 1
#define GENERATOR 2
#define EXTRACTOR 3
#define POLICY 4
#define ASSIGNMENT 5
#define PRINTER 6 // for the swag i guess, only works on the first 6 boards

#define PROCESS_COUNT 7

#define QUEUE_SIZE 6 	//min of 6, max of 255


#define M_WORKING_BOARDS 5003
#define M_WORKING_MUTATIONS 5004
#define M_INPUT_POLICY 5005
#define M_OUTPUT_POLICY 5006
#define M_INPUT_ASSIGNMENT 5007
#define M_OUTPUT_ASSIGNMENT 5008
#define M_COUNT 5009

#define STATUS_DONE 455

void SetBool(bool a[SIZE], char v) {
    for(int i = 0; i < SIZE; i++)
        a[i] = false;
    if(v)
        a[v-1] = true;
}

char _processID = MAIN;
int _write = STDOUT_FILENO;
int _read = STDIN_FILENO;
int _readFrom[PROCESS_COUNT];
int _writeTo[PROCESS_COUNT];
int _pid[PROCESS_COUNT];

const char* Name(int id = _processID)
{
	const char* array[] = { "MAIN", "MANAGER", "GENERATOR", "EXTRACTOR", "POLICY", "ASSIGNMENT", "PRINTER" };
	return array[id];
}

void SendTo(int to, void* val, size_t len)
{
	if(_writeTo[to] == -1)
	{
		std::cerr << "Can't write from " << Name() 
		<< " to " << Name(to) << std::endl
		<< "Hint: did you mean Send?" << std::endl;
		return;
	}
	
	write(_writeTo[to], val, len);
}

void Send(void* val, size_t len)
{
	write(_write, val, len);
}

void RecieveFrom(int from, void* buf, size_t len)
{
	if(_readFrom[from] == -1)
	{
		std::cerr << "Can't read from " << Name(from) 
		<< " in " << Name() << std::endl
		<< "Hint: did you mean Recieve?" << std::endl;
		return;
	}
	
	read(_readFrom[from], buf, len);
}

void Recieve(void* buf, size_t len)
{
	read(_read, buf, len);
}

int Open()
{
	for(int i = 0; i < PROCESS_COUNT; i++)
	{
		_readFrom[i] = -1;
		_writeTo[i] = -1;
	}
	
	int child_fd[2];
	int parent_fd[2];
	
	pipe(child_fd);
	pipe(parent_fd);
	
	if((_pid[MANAGER] = fork()) == 0)
	{
		_processID = MANAGER;
		_read = child_fd[0];
		_write = parent_fd[1];
		close(parent_fd[0]);
		close(child_fd[1]);
		
		int child_fds[5][2];
		int parent_fds[5][2];
		
		pipe(child_fds[0]);
		pipe(parent_fds[0]);
		
		if((_pid[GENERATOR] = fork()) == 0)
		{
			_processID = GENERATOR;
			_read = child_fds[0][0];
			_write = parent_fds[0][1];
			close(parent_fds[0][0]);
			close(child_fds[0][1]);
			
			return 0;
		}
		
		close(child_fds[0][0]);
		close(parent_fds[0][1]);
		
		pipe(child_fds[1]);
		pipe(parent_fds[1]);
		
		if((_pid[POLICY] = fork()) == 0)
		{
			_processID = POLICY;
			_read = child_fds[1][0];
			_write = parent_fds[1][1];
			close(parent_fds[1][0]);
			close(child_fds[1][1]);
			
			return 0;
		}
		
		close(child_fds[1][0]);
		close(parent_fds[1][1]);
		
		pipe(child_fds[2]);
		pipe(parent_fds[2]);
		
		if((_pid[ASSIGNMENT] = fork()) == 0)
		{
			_processID = ASSIGNMENT;
			_read = child_fds[2][0];
			_write = parent_fds[2][1];
			close(parent_fds[2][0]);
			close(child_fds[2][1]);
			
			
			return 0;
		}
		
		close(child_fds[2][0]);
		close(parent_fds[2][1]);
		
		pipe(child_fds[3]);
		pipe(parent_fds[3]);
		
		if((_pid[PRINTER] = fork()) == 0)
		{
			_processID = PRINTER;
			_read = child_fds[3][0];
			_write = parent_fds[3][1];
			close(parent_fds[3][0]);
			close(child_fds[3][1]);
			
			
			return 0;
		}
		
		close(child_fds[3][0]);
		close(parent_fds[3][1]);
		
		pipe(child_fds[4]);
		pipe(parent_fds[4]);
		
		if((_pid[EXTRACTOR] = fork()) == 0)
		{
			_processID = EXTRACTOR;
			_read = child_fds[4][0];
			_write = parent_fds[4][1];
			close(parent_fds[4][0]);
			close(child_fds[4][1]);
			
			
			return 0;
		}
		
		close(child_fds[4][0]);
		close(parent_fds[4][1]);
		
		_readFrom[GENERATOR] = parent_fds[0][0];
		_writeTo[GENERATOR] = child_fds[0][1];
		
		_readFrom[POLICY] = parent_fds[1][0];
		_writeTo[POLICY] = child_fds[1][1];
		
		_readFrom[ASSIGNMENT] = parent_fds[2][0];
		_writeTo[ASSIGNMENT] = child_fds[2][1];
		
		_readFrom[PRINTER] = parent_fds[3][0];
		_writeTo[PRINTER] = child_fds[3][1];
		
		_readFrom[EXTRACTOR] = parent_fds[4][0];
		_writeTo[EXTRACTOR] = child_fds[4][1];
		
		return 0;
	}
	
	close(child_fd[0]);
	close(parent_fd[1]);
	_readFrom[MANAGER] = parent_fd[0];
	_writeTo[MANAGER] = child_fd[1];
	
	return 0;
}

void Alloc
(
	  Board*&    workingBoards
	, Mutation*& workingMutations
	, double*&    inputPols
	, double*&    outputPols
	, double*&    inputAsns
	, double*&    outputAsns
	, int*&		 counts
) 
{
	int m_boards_id = shmget
	(
		  M_WORKING_BOARDS
		, QUEUE_SIZE * sizeof(Board)
		, IPC_CREAT | 0666 
	);
	workingBoards = new (shmat(m_boards_id, NULL, 0)) Board[QUEUE_SIZE];
	
	int m_mutations_id = shmget
	(
		  M_WORKING_MUTATIONS
		, QUEUE_SIZE * BOARDSIZE * sizeof(Mutation)
		, IPC_CREAT | 0666
	);
	workingMutations = (Mutation*) shmat(m_mutations_id, NULL, 0);
	
	int m_input_policy = shmget
	(
		  M_INPUT_POLICY
		, QUEUE_SIZE * BOARDSIZE * BOARDSIZE * sizeof(double)
		, IPC_CREAT | 0666
	);
	inputPols = (double*) shmat(m_input_policy, NULL, 0);
	
	int m_output_policy = shmget
	(
		  M_OUTPUT_POLICY
		, QUEUE_SIZE * BOARDSIZE * BOARDSIZE * sizeof(double)
		, IPC_CREAT | 0666
	);
	outputPols = (double*) shmat(m_output_policy, NULL, 0);
	
	int m_input_assignment = shmget
	(
		  M_INPUT_ASSIGNMENT
		, QUEUE_SIZE * BOARDSIZE * 3 * (SIZE-1) * SIZE * sizeof(double)
		, IPC_CREAT | 0666
	);
	inputAsns = (double*) shmat(m_input_assignment, NULL, 0);
	
	int m_output_assignment = shmget
	(
		  M_OUTPUT_ASSIGNMENT
		, QUEUE_SIZE * BOARDSIZE * SIZE * sizeof(double)
		, IPC_CREAT | 0666
	);
	outputAsns = (double*) shmat(m_output_assignment, NULL, 0);
	
	int m_count = shmget
	(
		  M_COUNT
		, QUEUE_SIZE * sizeof(int)
		, IPC_CREAT | 0666
	);
	counts = (int*) shmat(m_count, NULL, 0);
} 

// Forward signal to children

void OnSignal(int i)
{
	for(int i = 0; i < 6; i++)
	{
		if(_pid[i] != 0)
			kill(_pid[i], i);
	}
	exit(1);
}

int main() 
{
	signal (SIGINT,OnSignal);
	signal (SIGQUIT,OnSignal);
	signal (SIGKILL,OnSignal);
	
	Board*    workingBoards;
	Mutation* workingMutations;
	double*    inputPols;
	double*    outputPols;
	double*    inputAsns;
	double*    outputAsns;
	int* 	  counts;
	
	Alloc (
		  workingBoards
		, workingMutations
		, inputPols
		, outputPols
		, inputAsns
		, outputAsns
		, counts
	);
	
	Open();
	
	if(_processID == GENERATOR)
	{
		std::cout << Name() << ": " << getpid() << std::endl;
		while(1)
		{
			char bIndex;
			Recieve(&bIndex, 1);
			
			if(bIndex == -1)
				return 0;
			
			std::cout << Name() << " got " << (int)bIndex << std::endl;
			
			PuzzleGenerator::GenerateMinimum(
				  workingBoards[bIndex]
				, &workingMutations[bIndex*BOARDSIZE]
			);

			std::cout << Name() << " finished " << (int)bIndex << std::endl; 
			 
			Send(&bIndex, 1);
		}
	}
	if(_processID == EXTRACTOR)
	{
		std::cout << Name() << ": " << getpid() << std::endl;
		while(1)
		{
			char bIndex;
			Recieve(&bIndex, 1);
			
			if(bIndex == -1)
				return 0;
			
			std::cout << Name() << " got " << (int)bIndex << std::endl;
			
			counts[bIndex] = Extractor::Extract(
				  workingBoards[bIndex]
				, &workingMutations[bIndex*BOARDSIZE]
				, &inputPols[bIndex*BOARDSIZE*BOARDSIZE]
				, &outputPols[bIndex*BOARDSIZE*BOARDSIZE]
				, &inputAsns[bIndex*BOARDSIZE*3*(SIZE-1)*SIZE]
				, &outputAsns[bIndex*BOARDSIZE*SIZE]
			);
			
			std::cout << Name() << " finished " << (int)bIndex << std::endl; 
			 
			Send(&bIndex, 1);
		}
	}
	if(_processID == POLICY)
	{
		std::cout << Name() << ": " << getpid() << std::endl;
		NeuralNet* policy = new NeuralNet(BOARDSIZE, A_HIDDEN, BOARDSIZE, P_EPS, P_ETA);
		while(1)
		{
			char bIndex;
			Recieve(&bIndex, 1);
			
			if(bIndex == -1)
			{
				std::ofstream s("PolicyNetwork.txt");
				policy->Save(s);
				s.close();
				return 0;
			}
			
			std::cout << Name() << " got " << (int)bIndex << std::endl;
			
			policy->Train
			(
				  &inputPols[bIndex*BOARDSIZE*BOARDSIZE]
				, &outputPols[bIndex*BOARDSIZE*BOARDSIZE]
				, counts[bIndex]
			);

			std::cout << Name() << " finished " << (int)bIndex << std::endl; 
			
			Send(&bIndex, 1);
		}
	}
	if(_processID == ASSIGNMENT)
	{
		std::cout << Name() << ": " << getpid() << std::endl;
		NeuralNet* assignment = new NeuralNet(3*(SIZE-1)*SIZE, P_HIDDEN, SIZE, A_EPS, A_ETA);
		while(1)
		{
			char bIndex;
			Recieve(&bIndex, 1);
			
			if(bIndex == -1)
			{
				std::ofstream s("AssignmentNetwork.txt");
				assignment->Save(s);
				s.close();
				return 0;
			}
			
			std::cout << Name() << " got " << (int)bIndex << std::endl;
			
			assignment->Train
			(
				  &inputAsns[bIndex*BOARDSIZE*3*(SIZE-1)*SIZE]
				, &outputAsns[bIndex*BOARDSIZE*SIZE]
				, counts[bIndex]
			);
			
			std::cout << Name() << " finished " << (int)bIndex << std::endl; 
			
			Send(&bIndex, 1);	
		}
	}
	if(_processID == PRINTER)
	{
		/*
		using namespace std;
		cout << clear_screen;
		timespec t, tawake;
		t.tv_sec = 3;
		t.tv_nsec = 83333333; //approx 12 frames a sec
	
		nanosleep(&t, &tawake);
		while(1)
		{
			cout << move_to(BOARD1Y, BOARD1X);
			workingBoards[0].printBoard();
			
			cout << move_to(BOARD1Y, BOARD2X);
			workingBoards[1].printBoard();
			
			cout << move_to(BOARD2Y, BOARD1X);
			workingBoards[2].printBoard();
			
			cout << move_to(BOARD2Y, BOARD2X);
			workingBoards[3].printBoard();
			
			cout << move_to(BOARD3Y, BOARD1X);
			workingBoards[4].printBoard();
			
			cout << move_to(BOARD3Y, BOARD2X);
			workingBoards[5].printBoard();
			
			t.tv_sec = 0;
			t.tv_nsec = 83333333; //approx 12 frames a sec
		
			nanosleep(&t, &tawake);
		}
		*/
	}
	if(_processID == MANAGER)
	{
		std::cout << Name() << ": " << getpid() << std::endl;
		bool stop = false;
		char waiting = 0;
		fd_set readfds;
		
		for(char bIndex = 0; bIndex < QUEUE_SIZE; bIndex++)
		{
			SendTo(GENERATOR, &bIndex, 1);
		}
		char bIndex;
		
		char sent[QUEUE_SIZE] = { 0 };
		
		while(1)
		{
			FD_ZERO(&readfds);
			FD_SET(_readFrom[GENERATOR],  &readfds);
			FD_SET(_readFrom[EXTRACTOR],  &readfds);
			FD_SET(_readFrom[POLICY],     &readfds);
			FD_SET(_readFrom[ASSIGNMENT], &readfds);
			FD_SET(_read, 				  &readfds); 
			select (FD_SETSIZE, &readfds, NULL, NULL, NULL);
			if(FD_ISSET(_readFrom[GENERATOR], &readfds))
			{
				char bIndex;
				RecieveFrom(GENERATOR, &bIndex, 1); 
				SendTo(EXTRACTOR, &bIndex, 1);
			}	
			if(FD_ISSET(_readFrom[EXTRACTOR], &readfds))
			{
				char bIndex;
				RecieveFrom(EXTRACTOR, &bIndex, 1); 
				sent[bIndex] = 1;
				SendTo(POLICY, &bIndex, 1);
				//SendTo(ASSIGNMENT, &bIndex, 1);
			}				
			if(FD_ISSET(_readFrom[POLICY], &readfds))
			{
				char bIndex;
				RecieveFrom(POLICY, &bIndex, 1); 
				sent[bIndex] --;
				if(sent[bIndex] == 0)
				{
					if(stop)
						waiting--;
					else
						SendTo(GENERATOR, &bIndex, 1);
				}
			}
			if(FD_ISSET(_readFrom[ASSIGNMENT], &readfds))
			{
				char bIndex;
				RecieveFrom(ASSIGNMENT, &bIndex, 1); 
				sent[bIndex] --;
				if(sent[bIndex] == 0)
				{
					if(stop)
						waiting--;
					else
						SendTo(GENERATOR, &bIndex, 1);
				}
			}
			if(FD_ISSET(_read, &readfds))
			{
				std::cout << Name() << " Got kill" << std::endl;
				
				char q;
				
				Recieve(&q, 1);
				
				waiting = QUEUE_SIZE;
				stop = true;
			}
			if(stop && waiting == 0)
			{
				char finish = -1;
				SendTo(ASSIGNMENT, &finish, 1);
				SendTo(POLICY, &finish, 1);
				SendTo(EXTRACTOR, &finish, 1);
				SendTo(GENERATOR, &finish, 1);
				kill(_pid[PRINTER], SIGQUIT);
				return STATUS_DONE;
			}
		}
	}
	if(_processID == MAIN)
	{
		std::cout << Name() << ": " << getpid() << std::endl;
		while(1)
		{
			std::string input;
			std::getline(std::cin, input);
			if(input == "quit")
			{
				// quit routine
				int status;
				char q = 'q';
				SendTo(MANAGER, &q, 1);
				waitpid(_pid[MANAGER], &status, 0);
				std::cout << clear_screen << move_to(3, 3);
				std::cout << "Killed children" << std::endl;
				
				return 0;
			}
		}
	}
}

