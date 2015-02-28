
#ifndef REQUEST_H
#define REQUEST_H 1

#include <string>
#include <map>

/**
 * Header map.
 */

typedef std::map<std::string, std::string> HeaderMap;

/**
 * Response.
 */

class Response {
public:
  Response();
  ~Response();

  /**
   * "OK" shorthand flag.
   */

  int ok;

  /**
   * Response status code.
   */

  int status;

  /**
   * Response body.
   */

  std::string data;

  /**
   * Headers.
   */

  HeaderMap headers;
};

/**
 * Supported request methods.
 */

typedef enum {
    REQUEST_GET
  , REQUEST_POST
  , REQUEST_PUT
  , REQUEST_DELETE
} RequestMethod;

/**
 * Request.
 */

class Request {
public:

  Request();
  ~Request();

  /**
   * Set request method to GET using `url`.
   */

  void
  Get(const std::string &url);

  /**
   * Set request method to PUT using `url`.
   */

  void
  Put(const std::string &url);

  /**
   * Set request method to POST using `url`.
   */

  void
  Post(const std::string &url);

  /**
   * Set request method to DELETE using `url`.
   */

  void
  Delete(const std::string &url);

  /**
   * Set the user agent.
   */

  void
  UserAgent(const std::string &user_agent);

  /**
   * Set follow redirects flag to `val`.
   */

  void
  FollowRedirects(bool val);

  /**
   * Set header `field` to `val`.
   */

  void
  Set(const std::string &field, const std::string &val);

  /**
   * Set content-type to `type`.
   *
   * TODO: map stuff like "json" => "application/json"
   */

  void
  Type(const std::string &type);

  /**
   * Set Accept to `type`
   *
   * TODO: map for ease of use (json => application/json, etc.)
   */

  void
  Accept(const std::string &type);

  /**
   * Set Authorization field value with `user` and `pass`.
   */

  void
  Auth(const std::string &user, const std::string &pass);

  /**
   * Send the given `data` along with the request.
   */

  void
  Send(void);

  /**
   * Get all headers.
   */

  HeaderMap
  Headers();

  /**
   * Add query string `val`.
   */

  void
  Query(const std::string &val);

  /**
   * Add query string `field=val`.
   */

  void
  Query(const std::string &field, const std::string &val);

  /**
   * Get request `url`.
   */

  std::string
  Url();

  /**
   * Initiate request.
   */

  Response *
  End();

private:

  /**
   * Should we follow 30x redirects?
   */

  bool follow_redirects;

  /**
   * User agent.
   */

  std::string user_agent;

  /**
   * Request URL.
   */

  std::string url;

  /**
   * Auth username.
   */

  std::string auth_username;

  /**
   * Auth password.
   */

  std::string auth_password;

  /**
   * Request method.
   */

  RequestMethod method;

  /**
   * Request headers.
   */

  HeaderMap headers;

  /**
   * Query string map.
   */

  std::map<std::string, std::string> query;

  /**
   * Has auth been set?
   */

  bool
  HasAuth();

  std::string
  QueryString();

  /**
   * Write data callback.
   */

  static size_t
  WriteCallback(
      void *data
    , size_t size
    , size_t nmemb
    , void *userdata
  );

  /**
   * Header callback.
   */

  static size_t
  HeaderCallback(
      void *data
    , size_t size
    , size_t nmemb
    , void *userdata
  );

}; // Request

#endif // REQUEST_H
