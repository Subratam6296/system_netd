/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _RESOLVER_CONTROLLER_H_
#define _RESOLVER_CONTROLLER_H_

#include <list>
#include <vector>

#include "Dns64Configuration.h"
#include "netdutils/InternetAddresses.h"

struct __res_params;

namespace android {
namespace net {

class DumpWriter;
struct ResolverStats;

class ResolverController {
  public:
    ResolverController(const NetworkController& netCtrl)
        : mDns64Configuration(netCtrl, std::bind(&ResolverController::sendNat64PrefixEvent, this,
                                                 std::placeholders::_1)) {}

    ~ResolverController() {}

    // TODO: delete this function
    int setDnsServers(unsigned netId, const char* searchDomains, const char** servers,
            int numservers, const __res_params* params);

    int clearDnsServers(unsigned netid);

    int flushDnsCache(unsigned netid);

    int getDnsInfo(unsigned netId, std::vector<std::string>* servers,
            std::vector<std::string>* domains, __res_params* params,
            std::vector<android::net::ResolverStats>* stats);

    int getPrefix64(unsigned netId, netdutils::IPPrefix* prefix);

    // Binder specific functions, which convert between the binder int/string arrays and the
    // actual data structures, and call setDnsServer() / getDnsInfo() for the actual processing.
    int setResolverConfiguration(int32_t netId, const std::vector<std::string>& servers,
            const std::vector<std::string>& domains, const std::vector<int32_t>& params,
            const std::string& tlsName, const std::vector<std::string>& tlsServers,
            const std::set<std::vector<uint8_t>>& tlsFingerprints);

    int getResolverInfo(int32_t netId, std::vector<std::string>* servers,
                        std::vector<std::string>* domains, std::vector<std::string>* tlsServers,
                        std::vector<int32_t>* params, std::vector<int32_t>* stats);

    void sendNat64PrefixEvent(const net::Dns64Configuration::Nat64PrefixInfo& args);

    void dump(DumpWriter& dw, unsigned netId);

  private:
    Dns64Configuration mDns64Configuration;
};
}  // namespace net
}  // namespace android

#endif /* _RESOLVER_CONTROLLER_H_ */
