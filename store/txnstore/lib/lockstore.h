// -*- mode: c++; c-file-style: "k&r"; c-basic-offset: 4 -*-
/***********************************************************************
 *
 * store/txnstore/lib/lockstore.h:
 *    Single-node Key-value store with support for 2PC locking-based
 *    transactions using S2PL
 *
 * Copyright 2013-2015 Irene Zhang <iyzhang@cs.washington.edu>
 *                     Naveen Kr. Sharma <naveenks@cs.washington.edu>
 *                     Dan R. K. Ports  <drkp@cs.washington.edu>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************/

#ifndef _LOCK_STORE_H_
#define _LOCK_STORE_H_

#include "lib/assert.h"
#include "lib/message.h"
#include "store/common/backend/kvstore.h"
#include "txnstore.h"
#include "store/common/transaction.h"
#include "store/common/backend/lockserver.h"
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <list>

namespace txnstore {

class LockStore : public TxnStore
{
public:
    LockStore();
    ~LockStore();

    // Overriding from TxnStore.
    int Get(uint64_t id, const std::string &key,
        std::pair<Timestamp, std::string> &value);
    int Get(uint64_t id, const std::string &key, const Timestamp &timestamp,
        std::pair<Timestamp, std::string> &value);
    int Prepare(uint64_t id, const Transaction &txn);
    void Commit(uint64_t id, uint64_t timestamp);
    void Abort(uint64_t id, const Transaction &txn);
    void Load(const std::string &key, const std::string &value,
        const Timestamp &timestamp);

private:
    // Data store.
    KVStore store;

    // Locks manager.
    LockServer locks;

    std::map<uint64_t, Transaction> prepared;

    void dropLocks(uint64_t id, const Transaction &txn);
    bool getLocks(uint64_t id, const Transaction &txn);
};

} // namespace txnstore

#endif /* _LOCK_STORE_H_ */