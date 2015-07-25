# ApiForward
API Forwarding template by MinHook and Apache Thrift

## Forward your own APIs to another machine
If you want to forward your own APIs, write it to following files
+ extension/userdefined.thrift
+ extension/userdef_server.h
+ extension/userdef_client.cpp

### Skeleton
There are example skeleton files in 'extension' directory.
It shows how to forward 'shell32!ShellAbout' API to another machine.

## Generating Apache Thrift files
"'
> thrift-0.9.2.exe -r -gen cpp apiforward.thrift


This will generate gen-cpp folders what Visual Studio project needs.
It also generate following helpful files.
+ gen-cpp/SelfService_server.skeleton.cpp -> extension/userdef_server.h

## Build library on Windows

VS2010
