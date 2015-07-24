#include "ApiForward.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "userdef_server.h"

// Remove this if not necessary.
#include "Shellapi.h"
#pragma comment(lib, "shell32.lib")

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::apiforward;

class ApiForwardHandler : virtual public ApiForwardIf {
    public:
    ApiForwardHandler() {}

    //
    // Add your hooking function here
    //

    int32_t ShellAboutW(const std::string& szApp) {
        printf("ShellAboutW()\n");
        return 0;
    }
};


bool start_server(int port) {
    shared_ptr<ApiForwardHandler> handler(new ApiForwardHandler());
    shared_ptr<TProcessor> processor(new ApiForwardProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return true;
}

