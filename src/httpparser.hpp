#ifndef XATHTTP_HTTP_PARSER_H_
#define XATHTTP_HTTP_PARSER_H_


namespace XatHTTP
{

typedef int (*http_data_cb) (http_parser*, const char *at, size_t length);
typedef int (*http_cb) (http_parser*);

/* Request Method */
enum class HTTTP_Method
{
    DELETE, GET, HEAD, POST, PUT,

    /* */
    CONNECT, OPTIONS, TRACE,

    /* WebDAV */
    COPY, LOCK, MKCOL, MOVE, PROPFIND, 
    PROPPATCH, SEARCH, UNLOCK,

    /* Subversion */
    REPORT, MKACTIVITY, CHECKOUT, MERGE,

    /* upnp */
    MSEARCH, NOTIFY, SUBSCRIBE, UNSUBSCRIBE,

    /* RFC-5789 */
    PATCH, PURGE
};

/* Parser Type */
enum class HTTP_Parser_Type
{
    HTTP_REQUEST,
    HTTP_RESPONSE,
    HTTP_BOTH
};

class HTTPParser
{
public:


private:
    unsigned char type;
    unsigned char flags;
    unsigned char state;
    unsigned char header_state;
    unsigned char index;
};

enum class URL_Fields : char
{
    UF_SCHEMA   = 0,
    UF_HOST     = 1,
    UF_PORT     = 2,
    UF_PATH     = 3,
    UF_QUERY    = 4,
    UF_FRAGMENT = 5,
    UF_MAX      = 6
};

}   /* XatHTTP Namespace */

#endif /* XATHTTP_HTTP_PARSER_H_ */