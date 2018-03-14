#include <iostream>
#include <cstring>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "abathur/io_loop.hpp"
#include "abathur/net/socket.hpp"
#include "abathur/net/inet_address.hpp"
#include "abathur/net/socket_server.hpp"
#include "abathur/net/socket_handler.hpp"

#define LISTEN_PORT 8012
#define LISTEN_ADDR "0.0.0.0"



class EchoHandler : public abathur::net::SocketHandler{
    private:
        const static int BUFFER_SIZE = 200;
        char buffer_[BUFFER_SIZE];
    public:
        EchoHandler(std::shared_ptr<abathur::net::Socket> socket) : abathur::net::SocketHandler(socket) {};
        virtual void Process(const char* data, size_t size) override {
            time_t te = time(NULL);
            strftime(buffer_, BUFFER_SIZE, "%m-%d-%Y %H:%M:%S  hi.", gmtime(&te));
            std::string s(buffer_);
            s+=std::string(data, size);
            s+="\n";
            std::cout<<s<<endl;
        }
};

int main() {

    char buf[50];
    getcwd(buf, sizeof(buf));
    std::cout<<buf<< std::endl;

    std::shared_ptr<abathur::net::InetAddress> addr(
            new abathur::net::InetAddress(LISTEN_ADDR, LISTEN_PORT)
    );
    std::cout << addr->getAddrString() << addr->getPort() << std::endl;

    std::shared_ptr<abathur::net::SocketServer> tcp_server = std::make_shared<abathur::net::SocketServer>(
            addr
    );
    tcp_server->Init();
    tcp_server->SetHandler<EchoHandler>();
    abathur::IOLoop::Current()->Start();
    return 0;
}
