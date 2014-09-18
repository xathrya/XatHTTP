#ifndef XATHTTP_HTTP_CODE_H_
#define XATHTTP_HTTP_CODE_H_

#define CRLF "\r\n"

/* HTTP Version */
#define HTTP1_0                 "HTTP/1.0"
#define HTTP1_1                 "HTTP/1.1"

/* HTTP Status Code */
#define HTTP_200_OK                     "200 OK"
#define HTTP_302_FOUND                  "302 Found"
#define HTTP_400_BAD_REQUEST            "400 Bad Request"
#define HTTP_401_UNAUTHORIZED           "401 Unauthorized"
#define HTTP_403_FORBIDDEN              "403 Forbidden"
#define HTTP_404_NOT_FOUND              "404 Not Found"
#define HTTP_500_INTERNAL_SERVER_ERROR  "500 Internal Server Error"
#define HTTP_501_NOT_IMPLEMENTED        "501 Not Implemented"

/* Header */
#define HEADER_CONTENT_LENGTH   "Content-Length"
#define HEADER_CONTENT_TYPE     "Content-Type"
#define HEADER_ACCEPT           "Accept"
#define HEADER_CONNECTION       "Connection"

#define CONTENT_HTML_UTF8       "text/html; charset=utf-8"
#define CONTENT_TEXT_PLAIN      "text/plain"
#define CONNECTION_CLOSE        "Close"


#endif /* XATHTTP_HTTP_CODE_H_ */