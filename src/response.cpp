#include "response.hpp"

namespace XatHTTP
{

Response::Response()
{

}

Response::~Response()
{

}

Response Response::create(const char *status)
{
    Response resp;
    resp._status = status;

    return resp;
}

Response Response::create(const char *status, byte *body, size_t length, bool copyBody)
{
    Response resp;
    resp._status = status;

    if (copyBody) {
        resp._bodyCopy.resize(length);
        memcpy(resp._bodyCopy.data(), body, length);
        resp._body = resp._bodyCopy.data();
    } else {
        resp._body = body;
    }

    resp._length = length;
    resp._header[HEADER_CONTENT_LENGTH] = std::to_string(length);

    return resp;
}

const std::string & Response::getStatus()
{
    return _status;
}

const byte * Response::getBody()
{
    return _body;
}

const size_t Response::getLength()
{
    return _length;
}

const std::string & Response::getCompiledHeader()
{
    return _headerCompiled;
}

std::map<std::string, std::string> & Response::header()
{
    return _header;
}

const std::string & Response::compileHeader()
{
    std::string &hc = _headerCompiled;
    hc.reserve(4096);

    hc += HTTP1_0" " + _status + CRLF;

    for (auto it = _header.begin(); it != _header.end(); it++) {
        hc += it->first + ": " + it->second + CRLF;
    }

    hc += HEADER_CONNECTION ": " CONNECTION_CLOSE CRLF;
    hc += CRLF;

    return _headerCompiled;
}

void Response::setStatus(const char *status)
{
    _status = status;
}

void Response::setContentType(const char *contentType)
{
    _header[HEADER_CONTENT_TYPE] = contentType;
}

void Response::setBody(byte *body, size_t length, bool copyBody)
{
    if (copyBody) {
        _bodyCopy.resize(length);
        memcpy(_bodyCopy.data(), body, length);
        _body = _bodyCopy.data();
    } else {
        _body = body;
    }

    _length = length;
}

}