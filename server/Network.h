#pragma once
#include<iostream>
#include"OverEx.h"

/*
Send Recv는 관련이 너무 많으니 여기로 빼자

Send 와 Recv는 Thrad Safe 하니 Pointer를 이용해서 처리하자.
https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
https://cboard.cprogramming.com/c-programming/150774-parallel-threads-socket-send-recv.html
*/
namespace NETWORK {

    //Recv
    void Recv(SOCKET& socket, OverEx& overEx) {
        DWORD flag = 0;
        int err_no = 0;

        err_no = WSARecv(socket, &overEx.dataBuffer_, 1, NULL, &flag, &(overEx.over_), 0);

        if (err_no != 0) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                std::cout << "Error - IO pending Failure\n";
                while (true);
            }
        }
        else {
            std::cout << "Non Overlapped Recv return.\n";
            while (true);
        }
    }

}