#include "stdafx.h"
#include "afxsock.h"
#include "file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    signal(SIGINT, signalHandler);
    int nRetCode = 0;

    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
        _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        nRetCode = 1;
    }
    else {
        if (AfxSocketInit() == FALSE) {
            cout << "Khong the khoi tao thu vien socket" << endl;
            return 1;
        }

        vector<file> filelist = docdanhsachfile("FileList.txt");

        CSocket serverSocket;
        if (serverSocket.Create(1234) == 0) {
            cout << "Khong the khoi tao server " << endl;
            cout << serverSocket.GetLastError() << endl;
            return 1;
        }
        else {
            cout << "Khoi tao server thanh cong !" << endl;

            if (serverSocket.Listen(5) == FALSE) {
                cout << "Khong the lang nghe tren port nay " << endl;
                serverSocket.Close();
                return 1;
            }
        }

        while (true) {
            CSocket clientSocket;
            if (serverSocket.Accept(clientSocket)) {
                cout << "Client ket noi thanh cong " << endl;

                sendFileList(clientSocket, filelist);

                char filename[BUFFER_SIZE];
                while (true) {
                    memset(filename, 0, BUFFER_SIZE);
                    int bytesReceived = clientSocket.Receive(filename, BUFFER_SIZE - 1);
                    if (bytesReceived <= 0) break;

                    filename[bytesReceived] = '\0'; 

                    cout << "Client yeu cau tai file: " << filename << endl;

                    
                    bool fileFound = false;
                    for (const auto& fileInfo : filelist) {
                        if (fileInfo.name == filename) {
                            fileFound = true;
                            break;
                        }
                    }

                    if (fileFound) {
                        sendFile(clientSocket, filename);
                    }
                    else {
                        cerr << "File not found in list: " << filename << endl;
                        clientSocket.Send("ERROR", 5);
                    }
                }

                clientSocket.Close();
                cout << "File da duoc gui, dong ket noi" << endl;
            }
            else {
                cout << "Client ket noi chua thanh cong" << endl;
            }
        }

        serverSocket.Close();
    }
    return nRetCode;
}
