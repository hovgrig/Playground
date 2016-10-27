#include <iostream>

#include "XmlRpc.h"

using namespace XmlRpc;

int main()
{
    const char* hostname = "10.99.200.149";
    int port = 8080;

    // Create a client and connect to the server at hostname:port
    XmlRpcClient c(hostname, port);

    XmlRpcValue noArgs, result;

    // Call the remote procedure Hello and print the result value
    if (c.execute("Hello", noArgs, result))
    {
        std::cout << result << std::endl;
    }
}
