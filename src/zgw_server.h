#ifndef ZGW_SERVER_H
#define ZGW_SERVER_H

#include <string>
#include <map>
#include <utility>
#include <pthread.h>

#include <glog/logging.h>
#include "pink/include/server_thread.h"
#include "slash/include/slash_status.h"
#include "slash/include/slash_mutex.h"

#include "src/zgwstore/zgw_store.h"
#include "src/s3_cmds/zgw_s3_command.h"
#include "src/zgw_s3_rest.h"
#include "src/zgw_const.h"
#include "src/zgw_admin_conn.h"

#include "src/zgw_config.h"

using slash::Status;

class ZgwServerHandle : public pink::ServerHandle {
 public:
  virtual int CreateWorkerSpecificData(void** data) const override;
  virtual int DeleteWorkerSpecificData(void* data) const override;
};

class ZgwServer {
 public:
  explicit ZgwServer();
  virtual ~ZgwServer();
  Status Start();

  bool running() const {
    return !should_exit_.load();
  }

  void Exit();

 private:
  // Server related
	std::vector<std::string> zp_meta_ip_ports_;
  std::string ip_;
  std::atomic<bool> should_exit_;

  int worker_num_;

  ZgwServerHandle server_handle_;
  ZgwConnFactory conn_factory_;
  pink::ServerThread* zgw_dispatch_thread_;

  ZgwAdminConnFactory admin_conn_factory_;
  pink::ServerThread* zgw_admin_thread_;
};

#endif
