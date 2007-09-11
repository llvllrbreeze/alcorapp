#include "simple_server_t.h"
#include "simple_client_t.h"


int main()
{

  all::simple_server_t server;
  all::simple_client_t client;

  getchar();

  server.run_async();

  client.run_async();

  getchar();

  return 0;
};