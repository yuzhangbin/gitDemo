#ifndef __SOCKET_EPOLL_H__
#define __SOCKET_EPOLL_H__

#include <vector>

#if __APPLE__
#define EPOLLIN      (1U <<  0)
#define EPOLLPRI     (1U <<  1)
#define EPOLLOUT     (1U <<  2)
#define EPOLLERR     (1U <<  3)
#define EPOLLHUP     (1U <<  4)
#define EPOLLRDNORM  (1U <<  6)
#define EPOLLRDBAND  (1U <<  7)
#define EPOLLWRNORM  (1U <<  8)
#define EPOLLWRBAND  (1U <<  9)
#define EPOLLMSG     (1U << 10)
#define EPOLLRDHUP   (1U << 13)
#define EPOLLET      (1U << 30)
#define EPOLLONESHOT (1U << 31)

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_MOD 2
#define EPOLL_CTL_DEL 3

typedef union epoll_data {
    void* ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t events;
    epoll_data_t data;
};

#include <sys/event.h>
struct kepoll_event {
    struct kevent64_s kev;
    struct epoll_event ev;
};

#endif

#if __linux__
#include <sys/epoll.h>
#endif

class epoller final {
public:
    epoller(bool et = true);
    epoller(int size, bool et);
    ~epoller();
    
    epoller(const epoller&) = delete;
    epoller& operator=(const epoller&) = delete;

    int fd() const { return _fd; }
    bool valid() const { return _fd > 0; }

    int ctrl(int fd, uint64_t data, int events, int op) noexcept;
    int add(int fd, uint64_t data, int events) noexcept;
    int mod(int fd, uint64_t data, int events) noexcept;
    int del(int fd, uint64_t data, int events) noexcept;
    int wait(int millsecond) noexcept;

    int nfd() const { return _nfd; }
    void wake_up();

    const struct epoll_event& get(size_t i) const noexcept;
private:
    int _fd;
    bool _et;
    int _nfd;
    size_t _size;
#if __APPLE__
    mutable std::vector<struct kepoll_event> _evs;
#else
    std::vector<struct epoll_event> _evs;
#endif
};

#endif