
#define MAX_LOG_LENGTH		256
#define LOG_FATAL			0
#define LOG_IMPORTANT		1
#define LOG_NORMAL			2
#define LOG_IGNORE			3
#define LOG_DEBUG			4
#define LOG_ALL				5

#define LOG_JUST_DISPLAY	6

#define NUM_OF_LOG_TYPE		7

void MyLog( const int type, char *logmsg, ... );//020903 lsw
void InitMyLog( void );
void FreeMyLog( void );
