
# request.cc

  Simple HTTP request lib backed by [libcurl](http://curl.haxx.se/libcurl/), inspired by [superagent](https://github.com/visionmedia/superagent).

## Example

```c++
#include <assert.h>
#include <iostream>
#include "request.h"

int
main(){
  littlstar::Request *req = new littlstar::Request;
  littlstar::Response *res = NULL;

  req->Get("https://littlstar.com/api/v1/videos");
  req->Query("foo", "bar");
  req->Set("X-ApiKey", "myapikey");
  req->Accept("application/json");

  res = req->End();

  assert(res->ok);
  std::cout << res->data << std::endl;

  delete req;
  delete res;

  return 0;
}
```

## License

  MIT
