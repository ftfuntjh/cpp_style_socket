#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <system_error>

namespace network {
    namespace {
        template<int Level, int OptionName, typename OptionValue>
        struct SocketOption {
            int level = Level;
            int option = OptionName;
            OptionValue value;

            SocketOption() : value{} {

            }

            explicit SocketOption(OptionValue &&value_) : value(std::forward<OptionValue>(value_)) {

            }
        };
    }

    class Socket {
    public:
        explicit Socket(int fd);

        ~Socket();

        void bind(const sockaddr &addr, socklen_t addrLen) &;

        void connect(const sockaddr &addr, socklen_t addrLen) &;

        void listen() &;

        template<int Level, int OptionName, typename OptionValue>
        void getOption(SocketOption<Level, OptionName, OptionValue> &&option);

        template<int Level, int OptionName, typename OptionValue>
        void setOption(SocketOption<Level, OptionName, OptionValue> &&option);

        static Socket create(int protoFamily, int type, int protocol);

    private:
        int fd_;
    };

    Socket::Socket(int fd) : fd_{fd} {

    }

    void Socket::bind(const sockaddr &addr, socklen_t addrLen) &{
        ::bind(fd_, &addr, addrLen);
    }

    Socket Socket::create(int protoFamily, int type, int protocol) {
        int fd = ::socket(protoFamily, type, protocol);
        if (fd < 0) {
            throw std::system_error(errno, std::generic_category());
        }
        return Socket(fd);
    }

    template<int Level, int OptionName, typename OptionValue>
    void Socket::getOption(SocketOption<Level, OptionName, OptionValue> &&option) {
        int result = ::getsockopt(fd_, option.level, option.option, &option.value, sizeof(option.value));
        if (result != 0) {
            throw std::system_error(errno, std::generic_category());
        }
    }

    template<int Level, int OptionName, typename OptionValue>
    void Socket::setOption(SocketOption<Level, OptionName, OptionValue> &&option) {
        int result = ::setsockopt(fd_, option.level, option.option, &option.value, sizeof(option.value));
        if (result != 0) {
            throw std::system_error(errno, std::generic_category());
        }
    }

    Socket::~Socket() {
        if (fd_ > 0) {
            ::close(fd_);
        }
    }


    namespace option {
        using Debug =  SocketOption<SOL_SOCKET, SO_DEBUG, int>;
        using AcceptConnection = SocketOption<SOL_SOCKET, SO_ACCEPTCONN, int>;
        using ReuseAddress =SocketOption<SOL_SOCKET, SO_REUSEADDR, int>;
        using ReusePort = SocketOption<SOL_SOCKET, SO_REUSEADDR, int>;
        using KeepAlive = SocketOption<SOL_SOCKET, SO_KEEPALIVE, int>;
        using DontRoute = SocketOption<SOL_SOCKET, SO_DONTROUTE, int>;
        using BoardCast = SocketOption<SOL_SOCKET, SO_BROADCAST, int>;
    }
}

