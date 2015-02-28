
/**
 * Copyright (c) 2015 -  Little Star Media, Inc
 */

#include <stdlib.h>
#include <curl/curl.h>
#include "trim/trim.h"
#include "uri/uri.h"
#include "request.h"

Response::Response(){
  this->ok = 0;
  this->status = -1;
}

Response::~Response(){
  this->data.clear();
  this->headers.clear();
}

Request::Request(){
  this->FollowRedirects(true);
}

Request::~Request(){
  this->headers.clear();
  this->query.clear();
}

void
Request::Get(const std::string &url){
  this->url = url;
  this->method = REQUEST_GET;
}

void
Request::Put(const std::string &url){
  this->url = url;
  this->method = REQUEST_PUT;
}

void
Request::Post(const std::string &url){
  this->url = url;
  this->method = REQUEST_POST;
}

void
Request::Delete(const std::string &url){
  this->url = url;
  this->method = REQUEST_DELETE;
}

void
Request::UserAgent(const std::string &user_agent){
  this->user_agent = user_agent;
}

void
Request::FollowRedirects(bool val){
  this->follow_redirects = val;
}

void
Request::Set(const std::string &field, const std::string &val){
  // TODO: lowercase fieldname
  this->headers[field] = val;
}

void
Request::Type(const std::string &type){
  this->Set("Content-Type", type);
}

void
Request::Accept(const std::string &type){
  this->Set("Accept", type);
}

void
Request::Auth(const std::string &user, const std::string &pass){
  this->auth_username = user;
  this->auth_password = pass;
}

void
Request::Send(void){
  // TODO
}

HeaderMap
Request::Headers(){
  HeaderMap headers;
  headers.insert(this->headers.begin(), this->headers.end());
  return headers;
}

void
Request::Query(const std::string &val){
  this->query[val] = "";
}

void
Request::Query(const std::string &field, const std::string &val){
  this->query[field] = val;
}

std::string
Request::QueryString(){
  std::map<std::string, std::string>::iterator it;
  std::string qs;
  int i = 0;

  if (this->query.empty()) return qs;

  for (it = this->query.begin(); it != this->query.end(); it++, i++) {
    if (0 == i) {
      qs.append("?");
    } else {
      qs.append("&");
    }

    std::string field = it->first;
    std::string value = it->second;

    if (value.length()) {
      char *encoded = uri_encode(value.c_str());
      qs.append(field + "=" + encoded);
      free(encoded);
    } else {
      qs.append(field);
    }
  }

  return qs;
}

std::string
Request::Url(){
  return this->url + this->QueryString();
}

Response *
Request::End(){
  Response *res = new Response;
  CURL *req = NULL;
  CURLcode c = CURLE_OK;
  struct curl_slist *headers = NULL;
  long status = 0;
  HeaderMap::iterator it;

  if (!(req = curl_easy_init())) return res;

  #define option(x, y) curl_easy_setopt(req, x, y);

  // set auth
  if (this->HasAuth()) {
    std::string auth = this->auth_username + ":" + this->auth_password;
    option(CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    option(CURLOPT_USERPWD, auth.c_str());
  }

  // follow 30xs
  if (this->follow_redirects) {
    option(CURLOPT_FOLLOWLOCATION, 1);
  }

  // set useragent
  option(CURLOPT_USERAGENT, this->user_agent.c_str());

  // build/set headers
  for (it = this->headers.begin(); it != this->headers.end(); it++) {
    std::string str = it->first + ": " + it->second;
    headers = curl_slist_append(headers, str.c_str());
  }

  // set headers
  c = option(CURLOPT_HTTPHEADER, headers);
  // TODO: check return code

  // set url
  option(CURLOPT_URL, this->Url().c_str());

  // set callbacks
  switch (this->method) {
    case REQUEST_GET:
      option(CURLOPT_WRITEFUNCTION, this->WriteCallback);
      option(CURLOPT_WRITEDATA, res);
      option(CURLOPT_HEADERFUNCTION, this->HeaderCallback);
      option(CURLOPT_HEADERDATA, res);
      break;

    // TODO
    case REQUEST_POST: break;
    case REQUEST_PUT: break;
    case REQUEST_DELETE: break;
  }

  #undef option

  // make request
  c = curl_easy_perform(req);

  // set status
  curl_easy_getinfo(req, CURLINFO_RESPONSE_CODE, &status);
  res->ok = (200 == status && CURLE_ABORTED_BY_CALLBACK != c)
    ? 1
    : 0;
  res->status = static_cast<int>(status);

  // cleanup
  curl_easy_cleanup(req);
  curl_global_cleanup();
  curl_slist_free_all(headers);

  return res;
}

bool
Request::HasAuth(){
  return this->auth_username.length()
      && this->auth_password.length();
}

size_t
Request::WriteCallback(
    void *data
  , size_t size
  , size_t nmemb
  , void *userdata
){
  Response* res;
  res = reinterpret_cast<Response *>(userdata);
  res->data.append(reinterpret_cast<char *>(data), size * nmemb);
  return size * nmemb;
}

size_t
Request::HeaderCallback(
    void *data
  , size_t size
  , size_t nmemb
  , void *userdata
){
  Response* res;
  res = reinterpret_cast<Response *>(userdata);
  std::string header(reinterpret_cast<char *>(data), size * nmemb);
  size_t seperator = header.find_first_of(":");

  if (std::string::npos != seperator) {
    std::string key = header.substr(0, seperator);
    std::string value = header.substr(seperator + 1);
    res->headers[Trim(key)] = Trim(value);
  }

  return size * nmemb;
}
