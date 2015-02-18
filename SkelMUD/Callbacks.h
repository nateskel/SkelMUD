#ifndef CALLBACKS_H
#define CALLBACKS_H

class CallBacks
{
public:
	virtual void recv_callback(char* data, int socket) = 0;
protected:
private:
};

#endif // CALLBACKS_H