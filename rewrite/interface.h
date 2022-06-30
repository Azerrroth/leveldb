#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

#include "leveldb/db.h"

class ReDB : public leveldb::DB {
  std::ifstream read_value_file;
  std::ofstream write_value_file;

 public:
  ReDB() = default;

  ReDB(const ReDB&) = delete;

  void OpenFile(const std::string& name) {
    write_value_file.open(name.c_str(), std::ios::out);
    if (!write_value_file.is_open()) {
      std::cerr << "Cannot open file " << name << std::endl;
      exit(1);
    }
  }

  ~ReDB() { write_value_file.close(); }

  // Offset 48 bits, Length 16 bits
  // Value: [offset][length]
  // 6 bytes address & 2 bytes length
  leveldb::Status MyPut(const leveldb::WriteOptions& options,
                        const leveldb::Slice& key, const leveldb::Slice& value,
                        const std::string& path) {
    // value_file.seekg(0, std::ios::end);
    // std::streampos size = value_file.tellg();
    // std::cout << "文件大小为：" << size << " 字节" << std::endl;

    long offset = 0;
    int length = value.size();
    printf("[Key] %s [Value] %s. [Length] %d\n", key.ToString().c_str(),
           value.ToString().c_str(), length);

    return leveldb::DB::Put(options, key, "0x" + std::to_string(length));
  }

  leveldb::Status MyGet(const leveldb::ReadOptions& options,
                        const leveldb::Slice& key, std::string* value);

  leveldb::Status Delete(const leveldb::WriteOptions& options,
                         const leveldb::Slice& key);
};
