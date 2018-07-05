#include <memory>
#include <thread>
#include <chrono>

#include "gtest/gtest.h"
#include "mock_socketServer.h"
#include "MsgQueueImp.h"
#include "StateMachineImp.h"


using namespace ::testing;

class SmTest: public ::testing::Test {
protected:
    void SetUp() {
        std::cout<<"SetUp"<<std::endl;
    }
    void TearDown() {
        std::cout<<"TearDown"<<std::endl;
    }
    static void SetUpTestCase() {
        std::cout<<"SetUpTestCase"<<std::endl;
    }
    static void TearDownTestCase() {
        std::cout<<"TearDownTestCase"<<std::endl;
    }
};

TEST(SmTest, run)
{
    MockSocketServerIF socket;
    std::unique_ptr<MsgQueueIF> msgq(new MsgQueueImp());
    std::unique_ptr<StateMachineIF> sm(new StateMachineImp(msgq.get(), &socket));
    sm->run();
 
    EXPECT_CALL(socket,sendMsg(1,_)).Times(1);
    msgq->push(1, "");

    EXPECT_CALL(socket,closeSocket(1)).Times(1);
    msgq->push(1, "<close-session>");

    std::this_thread::sleep_for(chrono::seconds(2));
    sm->stop();
}

