#include "ApiForward.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "userdef_client.h"
#include "minhooklink.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace apiforward;


// Remove if you aren't hook ShellAboutW
#pragma comment(lib, "shell32.lib")


typedef int (WINAPI *SHELLABOUTW)(HWND, LPCWSTR, LPCWSTR, HICON);
SHELLABOUTW fpShellAboutW= NULL;
int WINAPI DetourShellAboutW(HWND hWnd, LPCWSTR szApp, LPCWSTR szOtherStuff, HICON hIcon)
{
    printf("DetourShellAboutW()\n");
    return fpShellAboutW(hWnd, szApp, szOtherStuff, hIcon);
}


HOOK_ITEM _hook_items[] = {
    {L"shell32", "ShellAboutW", (LPVOID)&DetourShellAboutW, (LPVOID*)&fpShellAboutW},
    {NULL, NULL, NULL, NULL}
};




bool start_client(const char* ipaddr, int port) {
  boost::shared_ptr<TTransport> socket(new TSocket(ipaddr, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  ApiForwardClient api(protocol);

  try {
    transport->open();



    transport->close();
  } catch (TException& tx) {
//    cout << "ERROR: " << tx.what() << endl;
      return false;
  }

  return true;
}
