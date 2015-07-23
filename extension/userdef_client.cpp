#include "ApiForward.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "userdef_client.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace apiforward;


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
