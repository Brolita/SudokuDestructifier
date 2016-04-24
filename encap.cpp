#include "Build Settings.h"

#define MAIN 0
#define CREATE 1
#define FULL 2
#define MIN 3
#define SOLVE 4
#define NN 5

int _processID = MAIN;
int _write = STDOUT_FILENO;
int _read = STDIN_FILENO;
int _readFrom[6];
int _writeTo[6];
int _pid[6];

const char* Name(int id = _processID)
{
	const char* array[] = { "MAIN", "CREATE", "FULL", 
							"MIN", "SOLVE", "NN" };
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
	int fds[2][2];
	
	pipe(fds[0]);
	
	if((_pid[CREATE] = fork()) == 0)
	{
		_processID = CREATE;
		_read = fds[0][0];
		_write = fds[0][1];
		
		int moreFds[3][2];
		
		for(int i = 0; i < 3; i++)
		{
			pipe(moreFds[i]);
			if((_pid[FULL + i] = fork()) == 0)
			{
				for(int j = 0; j < 3; j++)
				{
					_readFrom[FULL + j] = -1;
					_writeTo[FULL + j] = -1;
				}
				_processID = FULL + i;
				_read = moreFds[i][0];
				_write = moreFds[i][1];
				return 0;
			}
			_readFrom[FULL + i] = moreFds[i][0];
			_writeTo[FULL + i] = moreFds[i][1];
		}
		return 0;
	}
	_readFrom[CREATE] = fds[0][0];
	_writeTo[CREATE] = fds[0][1];
	
	pipe(fds[1]);
	if((_pid[NN] = fork()) == 0)
	{
		_processID = NN;
		_read = fds[1][0];
		_write = fds[1][1];
		_readFrom[CREATE] = -1;
		_writeTo[CREATE] = -1;
		return 0;
	}
	_readFrom[NN] = fds[0][0];
	_writeTo[NN] = fds[0][1];
	
	return 0;
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
	Open();
	
	if(_processID == MIN)
	{
		Send((void*)Name(), 4);  
		// since MIN is owned by CREATE
		// Send goes directly to CREATE
	}
	if(_processID == CREATE)
	{
		char buf[128];
		RecieveFrom(MIN, buf, 128);
		SendTo(FULL,(void*)buf, 4);
		// we know its 4, but normally you'd have 
		// to figure out its SIZE 
		RecieveFrom(FULL, buf, 128);
		Send(buf, 5);
		// CREATE is owned by MAIN
	}
	if(_processID == FULL)
	{
		char buf[128];
		Recieve(buf, 128);
		printf(buf);
		Send((void*)" done",6);
	}
	if(_processID == MAIN)
	{
		char buf[128];
		RecieveFrom(CREATE, buf, 128);
		printf(buf);
	}
}

