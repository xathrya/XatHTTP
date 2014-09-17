#ifndef XATHTTP_RESPONSE_H_
#define XATHTTP_RESPONSE_H_

#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "xathttp.hpp"
#include "httpcode.hpp"

namespace XatHTTP
{

class Response
{
public:
    ~Response();

    static Response create(const char *status);
    static Response create(const char *status, byte *body, size_t length, bool copyBody = true);

    const std::string   &   getStatus();
    const byte          *   getBody();
    const size_t            getLength();
    const std::string   &   getCompiledHeader();

    std::map<std::string, std::string>  &   header();

    const std::string   &   compileHeader();
    
    void setStatus(const char *status);
    void setContentType(const char *contentType);
    void setBody(byte *body, size_t length, bool copyBody = true);

private:
    Response();

    std::string                         _status;
    std::map<std::string, std::string>  _header;
    std::string                         _headerCompiled;
    std::vector<byte>                   _bodyCopy;

    byte                *   _body   = nullptr;
    size_t                  _length = 0;
};  /* Response Class */

}   /* XatHTTP Namespace */

#endif  /* XATHTTP_RESPONSE_H_ */