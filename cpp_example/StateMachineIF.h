#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

class StateMachineIF
{
public:
	virtual ~StateMachineIF() {}
	virtual void run() = 0;
	virtual void stop() = 0;
};

#endif
