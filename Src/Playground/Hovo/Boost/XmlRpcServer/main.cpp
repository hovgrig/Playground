#include "XmlRpc.h"
using namespace XmlRpc;


class Hello : public XmlRpcServerMethod
{
public:
    Hello(XmlRpcServer* s) : XmlRpcServerMethod("Hello", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        result = "Hello";
    }

};

int main()
{
    XmlRpcServer s;
    Hello h(&s);

    // Create the server socket on the specified port
    s.bindAndListen(8080);

    // Wait for requests and process indefinitely (Ctrl-C to exit)
    s.work(-1.0);
}
