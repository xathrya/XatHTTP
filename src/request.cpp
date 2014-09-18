#include "request.hpp"
#include "httpparser.hpp"

namespace XatHTTP
{

Request::Request()
{

}

Request::~Request()
{

}

Request Request::parse(const byte *requestBytes, size_t length)
{
    Request req;

    // XatHTTP::HTTPParser::parse(requestBytes, length);

    req._method = "GET";
    req._resourcePath       = "/index.html";
    req._httpVersion        = "HTTP/1.0";
    req._dataType           = DATA_TYPE_HTML;

    return req;
}

const std::string & Request::getMethod()
{
    return _method;
}

const std::string & Request::getResourcePath()
{
    return _resourcePath;
}

const std::string & Request::getHttpVersion()
{
    return _httpVersion;
}

const int & Request::getDataType()
{
    return _dataType;
}

}