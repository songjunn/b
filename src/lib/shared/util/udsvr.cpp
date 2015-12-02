#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream> 
#include <stddef.h>
#include "udsvr.h"

using namespace std;

const int QLEN = 10;
const int BUFF_MAX = 4096;

CUnixDomainServer udsvr;

CUnixDomainServer::CUnixDomainServer():m_serverFd(-1), m_clientFd(-1)
{

}

CUnixDomainServer::~CUnixDomainServer()
{

}

void CUnixDomainServer::StartUp(const char* listenPath)
{
    int len;                                            
    struct sockaddr_un un;                                             
                                                                       
    /* creat a UNIX domain stream socket */                            
    if ((m_serverFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)                    
	{
		cerr<< "CUnixDomainServer : socket(AF_UNIX, SOCK_STREAM, 0) failed" << endl;
        return;                                                    
	}
                                                                       
    unlink(listenPath); /* in case it already exists */                      
	cout <<" create unix sock "<< listenPath<< endl;

	int val = fcntl(m_serverFd, F_GETFL, 0);
	if (val < 0){
		_CloseServer();
		cerr<< "CUnixDomainServer : fcntl failed "<< errno << ":" << strerror(errno)<< endl;
		return ;
	}
	val = fcntl(m_serverFd, F_SETFL, val | O_NONBLOCK) ;
	if (val < 0) {
		_CloseServer();
		cerr<< "CUnixDomainServer : fcntl failed "<< errno << ":" << strerror(errno)<< endl;
		return;
	}
                                                                       
    /* fill in socket address structure */                             
    memset(&un, 0, sizeof(un));                                        
    un.sun_family = AF_UNIX;                                           
    strcpy(un.sun_path, listenPath);                                         
    len = offsetof(struct sockaddr_un, sun_path) + strlen(listenPath);       
                                                                       
    /* bind the name to the descriptor */                              
    if (bind(m_serverFd, (struct sockaddr *)&un, len) < 0) {                   
		_CloseServer();
		cerr<< "CUnixDomainServer : bind failed "<< errno << ":" << strerror(errno)<< endl;
		return;
    }                                                                  
                                                                       
    if (listen(m_serverFd, QLEN) < 0) { /* tell kernel we're a server */       
		_CloseServer();
		cerr<< "CUnixDomainServer : listen failed "<< errno << ":" << strerror(errno)<< endl;
		return;
    }

	return;
}

bool CUnixDomainServer::IsConnected()
{
	if (m_clientFd >= 0) {
		return true;
	}

	bool ret = false;
	if (m_serverFd >= 0) {
		ret = _AcceptServer();
	}
	
	return ret;
}

void CUnixDomainServer::SendToMonitor(int line, const char *str)
{
	char buffer[BUFF_MAX] = "";	
	int len = sizeof(line) + strlen(str);
	int index = 0;

	memcpy(buffer + index, &len, sizeof(len));
	index += sizeof(len);
	memcpy(buffer + index, &line, sizeof(line));
	index += sizeof(line);
	strcpy(buffer + index, str);

	//cout<< "send start"<<endl;
	int ret = send(m_clientFd, buffer, len + sizeof(len), MSG_NOSIGNAL);	
	if (ret < 0) {
		if (errno == EAGAIN) {
			cout<< "send EAGAIN"<<endl;
			return ;
		} else {
			cout<< "send failed"<<endl;
			_CloseClient();
		}
	} 
}

bool CUnixDomainServer::_AcceptServer()
{
	fd_set fdread;

	FD_ZERO(&fdread);
	FD_SET(m_serverFd, &fdread);
	timeval	t = {0,0};
	int ret = select(m_serverFd+1, &fdread, NULL, NULL, &t);
	if (ret <= 0) {
		return false;
	}

    socklen_t len;                                                            
    struct sockaddr_un un;                                                    
    len = sizeof(un);                                                         
    if ((m_clientFd = accept(m_serverFd, (struct sockaddr *)&un, &len)) < 0)         
	{
		if (errno != EAGAIN) {
			cerr<< "CUnixDomainServer : accept failed fd = "<< m_serverFd 
				<< " error : " << errno << ":" << strerror(errno)<< endl;
		}
        return false;
	}

    return true;                                                           
}
