#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "cnats/nats.h"

#define error_cleanup(go_label, msg) do {  \
  fprintf(stderr, "Error: %s\n", msg);     \
  goto go_label;                           \
} while(0)


int main(int argc, char **argv) {
  const char        *url  = NATS_DEFAULT_URL;
  natsConnection    *conn = NULL;
  natsSubscription  *sub  = NULL;
  natsOptions       *opts = NULL;
  natsMsg           *msg  = NULL;
  int64_t           last  = 0;
  natsStatus        s     = NATS_OK;
  
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
  
  if(natsConnection_SubscribeSync(&sub, conn, argv[1]) != NATS_OK)
    error_cleanup(cleanup, "natsConnection_SubscribeSync failed");
  
  if(natsSubscription_AutoUnsubscribe(sub, 1) != NATS_OK)
    error_cleanup(cleanup, "natsSubscription_AutoUnsubscribe failed");
  
  s = natsSubscription_NextMsg(&msg, sub, 10000);
    
  if(s == NATS_TIMEOUT)
    error_cleanup(cleanup, "natsSubscription_NextMsg timed out");

  if(s != NATS_OK)
    error_cleanup(cleanup, "natsSubscription_NextMsg failed");
  
  const char *msg_subj = natsMsg_GetSubject(msg);
  const char *msg_data = natsMsg_GetData(msg);
  const char *msg_rply = natsMsg_GetReply(msg);
  
  printf("got message subj:%s data:%s reply:%s\n", msg_subj, msg_data, msg_rply);
  
cleanup:
  natsMsg_Destroy(msg);
  natsSubscription_Destroy(sub);
  natsConnection_Destroy(conn);
  natsOptions_Destroy(opts);
  nats_Close();
  return 0;
}
