#include "thriftlink_client.h"


boost::shared_ptr<TTransport> _socket;
boost::shared_ptr<TTransport> _transport;
boost::shared_ptr<TProtocol> _protocol;
boost::shared_ptr<ApiForwardClient> _api;

bool thrift_connect(const char* ipaddr, int port) {
  boost::shared_ptr<TTransport> socket(new TSocket(ipaddr, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  boost::shared_ptr<ApiForwardClient> api(new ApiForwardClient(protocol));

  _socket = socket;
  _transport = transport;
  _protocol = protocol;
  _api = api;

  try {
    _transport->open();
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
      return false;
  }

  return true;
}

void thrift_close() {
 
    try {
        if (_transport.get() != NULL) {
            _transport->close();
        }
    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
        return;
    }

    return;
}