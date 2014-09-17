#ifndef XATHTTP_REQUEST_H_
#define XATHTTP_REQUEST_H_

#include <string>
#include "xathttp.hpp"
#include "httpcode.hpp"

namespace XatHTTP
{

class Request
{
public:
    ~Request();

    static Request parse(const byte *requestBytes, size_t length);

    const std::string   &   getMethod();
    const std::string   &   getResourcePath();
    const std::string   &   getHttpVersion();

private:
    Request();

    std::string         _method;
    std::string         _resourcePath;
    std::string         _httpVersion;
};  /* Request Class */

}   /* XatHTTP Namespace */

#endif  /* XATHTTP_REQUEST_H_ */