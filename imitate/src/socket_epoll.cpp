#include "../include/socket_epoll.h"
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

epoller::epoller(bool et)
: epoller(64, et) {
}

epoller::epoller(int size, bool et)
: _fd(-1)
, _et(et)
, _nfd(-1)
, _size(0) {
#if __linux__
    _fd = ::epoll_create(10240);
#elif __APPLE__
    _fd = kqueue();
#endif
    if (_fd == -1) {
        cout << "epoller::epoller: create epoller error." << endl;
    }
    _evs.resize(size);
}

epoller::~epoller() {
    if (_fd > 0) {
        ::close(_fd);
    }
    if (_nfd > 0) {
        ::close(_nfd);
    }
}

#if __APPLE__

int epoller::ctrl(int fd, uint64_t data, int events, int op) noexcept {
    struct kevent64_s kev[2];
    int changes = sizeof(kev) / sizeof(struct kevent64_s);
    int n = 0;

    if (op == EPOLL_CTL_ADD) {
        op = _et ? (EV_ADD | EV_CLEAR) : EV_ADD;
        EV_SET64(&kev[n++], fd, EVFILT_READ, op | (events & EPOLLIN ? EV_ENABLE : EV_DISABLE), 0, 0, data, 0, 0);
        EV_SET64(&kev[n++], fd, EVFILT_WRITE, op | (events & EPOLLOUT ? EV_ENABLE : EV_DISABLE), 0, 0, data, 0, 0);
    } else if (op == EPOLL_CTL_DEL) {
        EV_SET64(&kev[n++], fd, EVFILT_READ, EV_DELETE , 0, 0, data, 0, 0);
        EV_SET64(&kev[n++], fd, EVFILT_WRITE, EV_DELETE , 0, 0, data, 0, 0);
    } else if (op == EPOLL_CTL_MOD) {
        op = _et ? EV_CLEAR : 0; 
        EV_SET64(&kev[n++], fd, EVFILT_READ, op | (events & EPOLLIN) ? EV_ENABLE : EV_DISABLE, 0, 0, data, 0, 0);
        EV_SET64(&kev[n++], fd, EVFILT_WRITE, op | (events & EPOLLOUT) ? EV_ENABLE : EV_DISABLE, 0, 0, data, 0, 0);
    } else {
        return -1;
    }

    return kevent64(_fd, kev, changes, NULL, 0, 0, NULL);
}

#else
int epoller::ctrl(int fd, uint64_t data, int events, int op) noexcept {
    struct epoll_event ev;
    ev.data.u64 = data;
    ev.events = events;

    if (_et) {
        ev.events |= EPOLLET;
    }

    return ::epoll_ctl(_fd, op, fd, &ev);
}

#endif

int epoller::add(int fd, uint64_t data, int events) noexcept {
    if (++_size >= _evs.size()) {
        _evs.resize(_evs.size() * 2);
    }

    return ctrl(fd, data, events, EPOLL_CTL_ADD);
}

int epoller::mod(int fd, uint64_t data, int events) noexcept {
    return ctrl(fd, data, events, EPOLL_CTL_MOD);
}

int epoller::del(int fd, uint64_t data, int events) noexcept {
    -- _size;
    return ctrl(fd, data, events, EPOLL_CTL_DEL);
}

int epoller::wait(int millsecond) noexcept {
    int ret = 0;
#if __APPLE__
    struct timespec ts;
    ts.tv_sec = millsecond / 1000;
    ts.tv_nsec = (millsecond % 1000) * 1000 * 1000;
    ret = kevent64(_fd, nullptr, 0, (struct kevent64_s*)&_evs[0], (int)_evs.size(), 0, &ts);
#else
    ret = ::epoll_wait(_fd, &_evs[0], (int)_evs.size(), millsecond);
    if (ret < 0 && errno == EINTR) {
        return 0;
    }
#endif

    return ret;
}

void epoller::wake_up() {
    if (_nfd < 0 ) {
        _nfd = ::socket(SOCK_DGRAM, AF_INET, 0);
        add(_nfd, _nfd, EPOLLIN | EPOLLOUT);
    } else {
        mod(_nfd, _nfd, EPOLLIN | EPOLLOUT);
    }
}

const struct epoll_event& epoller::get(size_t i) const noexcept {
#if __APPLE__
    struct kevent64_s& kev = _evs[i].kev;
    struct epoll_event& ev = _evs[i].ev;

    ev.data.u64 = kev.udata;
    if (kev.filter == EVFILT_READ) {
        ev.events |= EPOLLIN;
    }
    if (kev.filter == EVFILT_WRITE) {
        ev.events |= EPOLLOUT;
    }
    if (kev.filter == EVFILT_EXCEPT || kev.filter == EVFILT_EXCEPT) {
        ev.events |= EPOLLERR;
    }
    if (kev.flags & EV_ERROR) {
        ev.events |= EPOLLERR;
    }
    if (kev.flags & EV_EOF) {
        ev.events |= EPOLLIN;
    }
    return ev;
#else
    return _evs[i];
#endif
}