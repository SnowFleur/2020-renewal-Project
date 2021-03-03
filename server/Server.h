#include<vector>
#include<thread>
#include<memory>
#include"WinSocketHeader.h"
#include"DataType.h"

constexpr int NUMBER_OF_THREAD = 6;


class CSector;
class CTimerThread;
class CDBThread;
struct EVENT_ST;
enum class ObjectState;
using Threads           = std::vector<std::thread>;
using Thread            = std::thread;
using sPtrSector        = std::shared_ptr<CSector>;
using sPtrTimerHandle   = std::shared_ptr<CTimerThread>;

class CServer {
private:
    HANDLE              iocp_;              //Iopc วุต้
    SOCKET              listenSocket_;      // listenSocket;
    SOCKET              dbSocket_;
    sPtrSector          sector_;            //Sector
    sPtrTimerHandle     timerThread_;       //TimerThread
    void                WorkThread();
    bool                AcceptDBServer();
    void                ProcessPacket(int id, char* packet);
public:
    CServer() = default;
    ~CServer() = default;

    CServer(const CServer&) = delete;
    CServer(CServer&&) = delete;
    CServer& operator=(const CServer&) = delete;
    CServer&& operator=(CServer&&) = delete;

    void Run();
    
};

