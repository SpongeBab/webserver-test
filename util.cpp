#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include  <string.h>
#include  <memory.h>
const int MAX_BUFF = 4096;

//用reann封装系统调用read（），返回 读文件描述符 个数
///*从socket读N个字节,成功返回n>=0 不成功-1*/
ssize_t readn(int fd, void *buff, size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char *ptr = (char *)buff;
    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0) // read 函数从fd读取字节到buffer缓冲区，错误返回-1
        {
            if (errno == EINTR)
                nread = 0;
            else if (errno == EAGAIN)
            {
                return readSum;
            }
            else
            {
                return -1;
            }
        }
        else if (nread == 0)
        {
            break;
        }
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSum;
}

//重載
ssize_t readn(int fd, std::string &inBuffer)
{
    ssize_t nread = 0;
    ssize_t readsum = 0;
    while (true)
    {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF) < 0))
        {
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN)
            {
                return readsum;
            }
            else
            {
                perror("read error /n");
                return -1;
            }
        }
        else if (nread == 0)
        {
            break;
        }
        readsum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readsum;
}

ssize_t writen(int fd, void *buff, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    char *ptr = (char *)buff;
    while (nleft > 0)
    {
        /* code */
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0)
            {
                if (errno == EINTR)
                {
                    nwritten = 0;
                    continue;
                }
                else if (errno == EAGAIN)
                {
                    return writeSum;
                }
                else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writeSum;
}

ssize_t writen(int fd, std::string &outBuffer)
{
    ssize_t n_left = outBuffer.size();
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    const char *ptr = outBuffer.c_str();
    while (true)
    {
        /* code */
        char buff[MAX_BUFF];
        if ((nwritten == write(fd, buff, MAX_BUFF)) <= 0)
        {
            if (nwritten < 0)
            {
                if (errno == EINTR)
                {
                    nwritten = 0;
                    continue;
                }
                else if (errno == EAGAIN)
                {
                    break;
                }
                else
                {
                    perror("write error /n");
                    return -1;
                }
            }        }
        writeSum += nwritten;
        n_left -= nwritten;
        ptr += nwritten;
    }
    if (writeSum == outBuffer.size())
    {
        outBuffer.clear();
    }
    else
        outBuffer = outBuffer.substr(writeSum);
    return writeSum;
}

void handle_for_sigpipe()
{
    struct sigaction sa; //信号，描述当一个信号到达
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL))
        return;
}

int setSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1)
        return -1;
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1)
        return -1;
    return 0;
}