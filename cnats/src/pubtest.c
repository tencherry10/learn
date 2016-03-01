#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "cnats/nats.h"

#define error_cleanup(go_label, msg) do {  \
  fprintf(stderr, "Error: %s\n", msg);     \
  goto go_label;                           \
} while(0)

int main(int argc, char **argv) {
  const char      *url    = NATS_DEFAULT_URL;
  natsConnection  *conn   = NULL;
  natsOptions     *opts   = NULL;
  int64_t         last    = 0;
  natsStatus      s       = NATS_OK;
  
  if(3 != argc)
    error_cleanup(cleanup, "invalid argc (should be 3)");
  
  if(natsOptions_Create(&opts) != NATS_OK)
    error_cleanup(cleanup, "nats opt create memory failed");
  
  if(natsOptions_SetSecure(opts, false) != NATS_OK)
    error_cleanup(cleanup, "natsOptions_SetSecure failed");
  
  if(natsOptions_SetServers(opts, &url, 1) != NATS_OK)
    error_cleanup(cleanup, "natsOptions_SetServers failed");
  
  if(natsConnection_Connect(&conn, opts) != NATS_OK) 
    error_cleanup(cleanup, "natsConnection_Connect failed");
  
  if(natsConnection_PublishString(conn, argv[1], argv[2]) != NATS_OK)
    error_cleanup(cleanup, "natsConnection_PublishString failed");

  if(natsConnection_FlushTimeout(conn, 1000) != NATS_OK)
    error_cleanup(cleanup, "natsConnection_FlushTimeout failed");
  
cleanup:
  natsConnection_Destroy(conn);
  natsOptions_Destroy(opts);
  nats_Close();
  return 0;
}
