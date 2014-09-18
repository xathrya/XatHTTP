#ifndef XATHTTP_REQUEST_H_
#define XATHTTP_REQUEST_H_

#include <string>
#include "xathttp.hpp"
#include "httpcode.hpp"

#define DATA_TYPE_HTML          1
#define DATA_TYPE_PLAINTEXT     2
#define DATA_TYPE_BINARY        4

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
    const int           &   getDataType();

private:
    Request();

    int                 _dataType;
    std::string         _method;
    std::string         _resourcePath;
    std::string         _httpVersion;
};  /* Request Class */

}   /* XatHTTP Namespace */

#endif  /* XATHTTP_REQUEST_H_ */