# ApiForward
API Forwarding template by MinHook and Apache Thrift

Forward API call to another machine.
If you want to forward your own APIs, write it to
'userdefined.thrift', 'userdef_server.h' and 'userdef_client.cpp' in 'extension' directory.
There are example skeleton files in 'extension' directory. The skeleton files show how to forward shell32!ShellAbout API to another machine.
