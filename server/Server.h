#include<vector>
#include<thread>
#include<memory>
#include"WinSocketHeader.h"
#include"DataType.h"

constexpr int NUMBER_OF_THREAD = 6;
constexpr PositionType PRIMARY_SPAWN_POSITION_X = 10;
constexpr PositionType PRIMARY_SPAWN_POSITION_Y = 10;



class CSector;

using Threads           = std::vector<std::thread >;
using Thread            = std::thread;
using uPtrSector        = std::shared_ptr<CSector>;


class CServer {
private:
    HANDLE          iocp_;              //Iopc วุต้
    SOCKET          listenSocket_;      // listenSocket;
    uPtrSector      sector_;            //Sector
    void            ProcessPacket(int id, char* packet);
    void            WorkThread();
    void            TimerThread();
public:
    CServer() = default;
    ~CServer() = default;

    CServer(const CServer&) = delete;
    CServer(CServer&&) = delete;
    CServer& operator=(const CServer&) = delete;
    CServer&& operator=(CServer&&) = delete;

    void Run();
};

