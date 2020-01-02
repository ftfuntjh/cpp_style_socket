#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <system_error>
#include <netinet/ip.h>
#include <netinet/ip6.h>

namespace network {
    namespace {
        struct ReadableOption {

        };

        struct WriteableOption {

        };

        struct ReadWriteOption : public ReadableOption, WriteableOption {

        };

        template<int Level, int OptionName, typename OptionValue, typename OptionState>
        struct SocketOption : public OptionState {
            int level = Level;
            int option = OptionName;
            OptionValue value;
            unsigned int size;

            SocketOption() : OptionState{}, value{}, size{sizeof(OptionValue)} {

            }


            explicit SocketOption(OptionValue &&value_) : OptionState{},
                                                          value(std::forward<OptionValue>(value_)),
                                                          size{sizeof(OptionValue)} {

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

        void read() &;

        void write() &;

        template<typename OptionStatus, typename = typename std::enable_if<std::is_convertible<OptionStatus, ReadableOption>::value, OptionStatus>::type>
        void getOption(OptionStatus &option);

        template<typename OptionStatus, typename = typename std::enable_if<std::is_convertible<OptionStatus, ReadableOption>::value, OptionStatus>::type>
        void setOption(const OptionStatus &option);

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

    template<typename OptionStatus, typename>
    void Socket::getOption(OptionStatus &option) {
        int result = ::getsockopt(fd_, option.level, option.option, static_cast<void *>(&option.value),
                                  static_cast<unsigned int *>(&option.size));
        if (result != 0) {
            throw std::system_error(errno, std::generic_category());
        }
    }

    template<typename OptionStatus, typename>
    void Socket::setOption(
            const OptionStatus &option) {
        int result = ::setsockopt(fd_, option.level, option.option, static_cast<const void *>( &option.value),
                                  option.size);
        if (result != 0) {
            throw std::system_error(errno, std::generic_category());
        }
    }

    Socket::~Socket() {
        if (fd_ >= 0) {
            ::close(fd_);
        }
    }


    namespace option {
        /**
         * SOL_SOCKET Option
         */
        using SocketBoardCast = SocketOption<SOL_SOCKET, SO_BROADCAST, int, ReadWriteOption>;
        using SocketDebug =  SocketOption<SOL_SOCKET, SO_DEBUG, int, ReadWriteOption>;
        using SocketDontRoute = SocketOption<SOL_SOCKET, SO_DONTROUTE, int, ReadWriteOption>;
        using SocketKeepAlive = SocketOption<SOL_SOCKET, SO_KEEPALIVE, int, ReadWriteOption>;
        using SocketLinger = SocketOption<SOL_SOCKET, SO_LINGER, struct ::linger, ReadWriteOption>;
        using SocketOObinLine = SocketOption<SOL_SOCKET, SO_OOBINLINE, int, ReadWriteOption>;
        using SocketReciveBuffer = SocketOption<SOL_SOCKET, SO_RCVBUF, int, ReadWriteOption>;
        using SocketSendBuf = SocketOption<SOL_SOCKET, SO_SNDBUF, int, ReadWriteOption>;
        using SocketReciveLowat = SocketOption<SOL_SOCKET, SO_RCVLOWAT, int, ReadWriteOption>;
        using SocketSendLowat = SocketOption<SOL_SOCKET, SO_SNDLOWAT, int, ReadWriteOption>;
        using SocketReciveTimeOut =SocketOption<SOL_SOCKET, SO_RCVTIMEO, struct ::timeval, ReadWriteOption>;
        using SocketSendTimeOut = SocketOption<SOL_SOCKET, SO_SNDTIMEO, struct ::timeval, ReadWriteOption>;
        using SocketReuseAddress =SocketOption<SOL_SOCKET, SO_REUSEADDR, int, ReadWriteOption>;
        using SocketReusePort = SocketOption<SOL_SOCKET, SO_REUSEADDR, int, ReadWriteOption>;
        using SocketType = SocketOption<SOL_SOCKET, SO_TYPE, int, ReadWriteOption>;
        using SocketAcceptConnection = SocketOption<SOL_SOCKET, SO_ACCEPTCONN, int, ReadWriteOption>;
        using SocketUseLoopBack = SocketOption<SOL_SOCKET, SO_USELOOPBACK, int, ReadWriteOption>;

        /**
         * IPPROTO_IP Option
         */
        using IpHeaderInclude = SocketOption<IPPROTO_IP, IP_HDRINCL, int, ReadWriteOption>;
        using IpOptions = SocketOption<IPPROTO_IP, IP_OPTIONS, void *, ReadWriteOption>;
        using IpReceiveDestAddress = SocketOption<IPPROTO_IP, IP_RECVDSTADDR, int, ReadWriteOption>;
        using IpReceiveInterface = SocketOption<IPPROTO_IP, IP_RECVIF, int, ReadWriteOption>;
        using IpTOS = SocketOption<IPPROTO_IP, IP_TOS, int, ReadWriteOption>;
        using IpTTL = SocketOption<IPPROTO_IP, IP_TTL, int, ReadWriteOption>;
        using IpMulticastInterface = SocketOption<IPPROTO_IP, IP_MULTICAST_IF, struct in_addr, ReadWriteOption>;
        using IpMulticastTTL = SocketOption<IPPROTO_IP, IP_MULTICAST_TTL, u_char, ReadWriteOption>;
        using IpMulticastLoop = SocketOption<IPPROTO_IP, IP_MULTICAST_LOOP, u_char, ReadWriteOption>;
        using IpAddMemberShip = SocketOption<IPPROTO_IP, IP_ADD_MEMBERSHIP, struct ::ip_mreq, ReadWriteOption>;
        using IpDropMemberShip = SocketOption<IPPROTO_IP, IP_DROP_MEMBERSHIP, struct ::ip_mreq, ReadWriteOption>;
        using IpBlockSource = SocketOption<IPPROTO_IP, IP_BLOCK_SOURCE, struct ::ip_mreq_source, ReadWriteOption>;
    }
}

