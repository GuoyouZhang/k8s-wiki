#ifndef _MOCK_SOCKET_S
#define _MOCK_SOCKET_S

#include "gmock/gmock.h"
#include "SocketServerIF.h"

class MockSocketServerIF : public SocketServerIF {
 public:
  MOCK_METHOD3(setup,
      int(MsgQueueIF* q, const string& ip, int port));
  MOCK_METHOD2(sendMsg,
      int(int sock,const string& msg));
  MOCK_METHOD1(closeSocket,
      void(int sock));
  MOCK_METHOD0(run,
      void());
};


#endif
