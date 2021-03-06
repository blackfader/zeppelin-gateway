#ifndef ZGW_S3_REST_H
#define ZGW_S3_REST_H

#include "pink/include/http_conn.h"

#include "src/s3_cmds/zgw_s3_command.h"

// Every HTTP connection has its own handles
class ZgwHTTPHandles : public pink::HTTPHandles {
 public:
  ZgwHTTPHandles()
      : cmd_(nullptr) {
    cmd_table_ = new S3CmdTable;
    InitCmdTable(cmd_table_);
  }
  virtual ~ZgwHTTPHandles() {
    DestroyCmdTable(cmd_table_);
    delete cmd_table_;
  }

  virtual bool HandleRequest(const pink::HTTPRequest* req) override;
  virtual void HandleBodyData(const char* data, size_t data_size) override;
  virtual void PrepareResponse(pink::HTTPResponse* resp) override;
  virtual int WriteResponseBody(char* buf, size_t max_size) override;

  void HandleConnClosed();

 private:
  S3CmdTable* cmd_table_;
  S3Cmd* cmd_;
  S3Cmd* SelectS3CmdBy(const pink::HTTPRequest* req);
};

class ZgwConnFactory : public pink::ConnFactory {
 public:
  // Deleted in HTTPConn deconstructor
  virtual pink::PinkConn* NewPinkConn(int connfd,
                                      const std::string& ip_port,
                                      pink::ServerThread* server_thread,
                                      void* worker_specific_data) const {
    auto zgw_handles = std::make_shared<ZgwHTTPHandles>();
    return new pink::HTTPConn(connfd, ip_port, server_thread,
                              zgw_handles, worker_specific_data);
  }
};

#endif
