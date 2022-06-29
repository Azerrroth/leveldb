#include <assert.h>
#include <iostream>
#include <string>

#include "leveldb/db.h"

class ReDB : public leveldb::DB {
 public:
  ReDB() = default;

  ReDB(const ReDB&) = delete;

  leveldb::Status Put(const leveldb::WriteOptions& options,
                      const leveldb::Slice& key, const leveldb::Slice& value) {
    // offset 48 bit, length 16 bit
    long length = value.size();
    std::cout << "length: " << length << std::endl;
    return leveldb::DB::Put(options, key, value);
  }

  leveldb::Status Get(const leveldb::ReadOptions& options,
                      const leveldb::Slice& key, std::string* value);

  leveldb::Status Delete(const leveldb::WriteOptions& options,
                         const leveldb::Slice& key);
};
