#include<vector>
#include<thread>
#include<memory>
#include"WinSocketHeader.h"
#include"DataType.h"
#include"TimerThread.h"

constexpr int NUMBER_OF_THREAD = 6;


class CSector;
struct EVENT_ST;
enum class ObjectState;
using Threads           = std::vector<std::thread >;
using Thread            = std::thread;
using uPtrSector        = std::shared_ptr<CSector>;


class CServer {
private:
    HANDLE          iocp_;              //Iopc วุต้
    SOCKET          listenSocket_;      // listenSocket;
    uPtrSector      sector_;            //Sector
    CTimerThread    timerThread_;       //TimerThread

    void            ProcessPacket(int id, char* packet);
    void            WorkThread();
public:
    CServer() = default;
    ~CServer() = default;

    CServer(const CServer&) = delete;
    CServer(CServer&&) = delete;
    CServer& operator=(const CServer&) = delete;
    CServer&& operator=(CServer&&) = delete;

    void Run();
    
};

