
#include <assert.h>
#include "request.h"

#include <stdio.h>

void
TestUserAgent(){
  Request *req = new Request;
  Response *res = NULL;

  req->UserAgent("foobar");
  req->Get("http://httpbin.org/get");
  res = req->End();
  assert(200 == res->status);
  assert(std::string::npos != res->data.find("\"User-Agent\": \"foobar\""));
  delete req;
  delete res;
}

void
TestRedirects(){
  Request *req = new Request;
  Response *res = NULL;

  req->Get("http://google.com");
  req->FollowRedirects(false);
  res = req->End();
  assert(301 == res->status);
  delete req;
  delete res;

  req = new Request;
  req->Get("http://google.com");
  req->FollowRedirects(true);
  res = req->End();
  assert(200 == res->status);
  delete req;
  delete res;
}

void
TestHeaders(){
  Request *req = new Request;
  Response *res = NULL;
  HeaderMap headers;

  req->Set("foo", "bar");
  req->Set("Content-Type", "application/something");
  req->Accept("application/json");
  req->Type("bananas");

  headers = req->Headers();

  assert("bar" == headers["foo"]);
  assert("bananas" == headers["Content-Type"]);
  assert("application/json" == headers["Accept"]);

  req->Get("http://httpbin.org/get");
  res = req->End();
  assert(200 == res->status);
  assert(std::string::npos != res->data.find("\"Accept\": \"application/json\""));
  assert(std::string::npos != res->data.find("\"Content-Type\": \"bananas\""));
  assert(std::string::npos != res->data.find("\"Foo\": \"bar\""));
  delete req;
  delete res;
}

void
TestAccept(){
  Request *req = new Request;
  Response *res = NULL;

  req->Accept("tacos");
  req->Get("http://httpbin.org/get");
  res = req->End();
  assert(200 == res->status);
  assert(std::string::npos != res->data.find("\"Accept\": \"tacos\""));
  delete req;
  delete res;
}

void
TestType(){
  Request *req = new Request;
  Response *res = NULL;

  req->Type("tacos");
  req->Get("http://httpbin.org/get");
  res = req->End();
  assert(200 == res->status);
  assert(std::string::npos != res->data.find("\"Content-Type\": \"tacos\""));
  delete req;
  delete res;
}

void
TestAuth(){
  Request *req = new Request;
  Response *res = NULL;

  req->Auth("username", "password");
  req->Get("http://httpbin.org/basic-auth/username/password");
  res = req->End();
  assert(200 == res->status);
  delete req;
  delete res;
}

void
TestGet(){
  Request *req = NULL;
  Response *res = NULL;

  // success
  req = new Request;
  req->Get("https://github.com/");
  res = req->End();
  assert(200 == res->status);
  assert(1 == res->ok);
  assert("text/html; charset=utf-8" == res->headers["Content-Type"]);
  assert(std::string::npos != res->data.find("<title>GitHub"));
  delete req;
  delete res;

  // 404
  req = new Request;
  req->Get("https://google.com/404");
  res = req->End();
  assert(0 == res->ok);
  assert(404 == res->status);
  delete req;
  delete res;

  // failure
  req = new Request;
  req->Get("http://imnotreal");
  res = req->End();
  assert(0 == res->status);
  assert(0 == res->ok);
  delete req;
  delete res;
}

void
TestPost(){
  Request *req = NULL;
  Response *res = NULL;

  req = new Request;
  req->Post("http://httpbin.org/post");
  req->Send("data goes here");
  req->Type("application/json");
  res = req->End();

  assert(200 == res->status);
  assert(std::string::npos != res->data.find("\"data\": \"data goes here\""));

  delete req;
  delete res;
}

void
TestPut(){
  Request *req = NULL;
  Response *res = NULL;

  req = new Request;
  req->Put("http://httpbin.org/put");
  req->Send("data goes here");
  res = req->End();

  assert(200 == res->status);
  assert(std::string::npos != res->data.find("\"data\": \"data goes here\""));

  delete req;
  delete res;
}

void
TestDelete(){
  Request *req = NULL;
  Response *res = NULL;

  req = new Request;
  req->Delete("http://httpbin.org/delete");
  res = req->End();

  assert(res->ok);
  assert(200 == res->status);

  delete req;
  delete res;
}

int
main(){
  TestUserAgent();
  TestRedirects();
  TestHeaders();
  TestAccept();
  TestType();
  TestAuth();
  TestGet();
  TestPost();
  TestPut();
  TestDelete();
  return 0;
}
