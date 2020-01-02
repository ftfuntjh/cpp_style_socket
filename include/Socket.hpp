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

        void read();

        void write();

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
    void Socket::getOption(const SocketOption<Level, OptionName, OptionValue> &option) {
        int result = ::getsockopt(fd_, option.level, option.option, &option.value, sizeof(option.value));
        if (result != 0) {
            throw std::system_error(errno, std::generic_category());
        }
    }

    template<int Level, int OptionName, typename OptionValue>
    void Socket::setOption(SocketOption<Level, OptionName, OptionValue> &option) {
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
        /**
         * SOL_SOCKET Option
         */
        using SocketBoardCast = SocketOption<SOL_SOCKET, SO_BROADCAST, int>;
        using SocketDebug =  SocketOption<SOL_SOCKET, SO_DEBUG, int>;
        using SocketDontRoute = SocketOption<SOL_SOCKET, SO_DONTROUTE, int>;
        using SocketKeepAlive = SocketOption<SOL_SOCKET, SO_KEEPALIVE, int>;
        using SocketLinger = SocketOption<SOL_SOCKET, SO_LINGER, struct ::linger>;
        using SocketOObinLine = SocketOption<SOL_SOCKET, SO_OOBINLINE, int>;
        using SocketReciveBuffer = SocketOption<SOL_SOCKET, SO_REVBUF, int>;
        using SocketSendBuf = SocketOption<SOL_SOCKET, SO_SNDBUF, int>;
        using SocketReciveLowat = SocketOption<SOL_SOCKET, SO_RECLOWAT, int>;
        using SocketSendLowat = SocketOption<SOL_SOCKET, SO_SNDLOWAT, int>;
        using SocketReciveTimeOut =SocketOption<SOL_SOCKET, SO_RCVTIMEO, struct ::timeval>;
        using SocketSendTimeOut = SocketOption<SOL_SOCKET, SO_SNDTIMEO, struct ::timeval>;
        using SocketReuseAddress =SocketOption<SOL_SOCKET, SO_REUSEADDR, int>;
        using SocketReusePort = SocketOption<SOL_SOCKET, SO_REUSEADDR, int>;
        using SocketType = SocketOption<SOL_SOCKET, SO_TYPE, int>;
        using SocketAcceptConnection = SocketOption<SOL_SOCKET, SO_ACCEPTCONN, int>;
        using SocketUseLoopBack = SocketOption<SOL_SOCKET, SO_USELOOPBACK, int>;

        /**
         * IPPROTO_IP Option
         */
        using IpHeaderInclude = SocketOption<IPPROTO_IP, IP_HDRINCL, int>;
        using IpOptions = SocketOption<IPPROTO_IP, IP_OPTIONS, void *>;
        using IpReceiveDestAddress = SocketOption<IPPROTO_IP, IP_RECVDSTADDR, int>;
        using IpReceiveInterface = SocketOption<IPPROTO_IP, IP_RECVIF, int>;
        using IpTOS = SocketOption<IPPROTO_IP, IP_TOS, int>;
        using IpTTL = SocketOption<IPPROTO_IP, IP_TTL, int>;
        using IpMulticastInterface = SocketOption<IPPROTO_IP, IP_MULTICAST_IF, struct in_addr>;
        using IpMulticastTTL = SocketOption<IPPROTO_IP, IP_MULTICAST_TTL, u_char>;
        using IpMulticastLoop = SocketOption<IPPROTO_IP, IP_MULTICAST_LOOP, u_char>;
        using IpAddMemberShip = SocketOption<IPPROTO_IP, IP_ADD_MEMBSHIP, struct ::ip_mreq>;
        using IpDropMemberShip = SocketOption<IPPROTO_IP, IP_DROP_MEMBSHIP, struct ::ip_mreq>;
        using IpBlockSource = SocketOption<IPPROTO_IP, IP_BLOCK_SOURCE, struct ::ip_mreq_source>;
    }
}

