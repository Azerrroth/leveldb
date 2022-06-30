#include <assert.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "leveldb/db.h"

std::string dec2hex(int i)  //将int转成16进制字符串
{
  std::stringstream ioss;  //定义字符串流
  std::string s_temp;      //存放转化后字符
  ioss << std::setiosflags(std::ios::uppercase) << std::hex
       << i;  //以十六制(大写)形式输出

  ioss >> s_temp;
  return s_temp;
}

class ReDB : public leveldb::DB {
  std::fstream* real_value_file = nullptr;

 public:
  ReDB() = default;

  ReDB(const ReDB&) = delete;

  void OpenFile(const std::string& name) {
    real_value_file =
        new std::fstream(name, std::ios::in | std::ios::app | std::ios::binary);
    if (!real_value_file->is_open()) {
      std::cerr << "Cannot open file " << name << std::endl;
      exit(1);
    }
  }

  void CloseFile() {
    if (real_value_file) {
      real_value_file->close();
      delete real_value_file;
      real_value_file = nullptr;
    }
  }

  ~ReDB() = default;

  // Offset 48 bits, Length 16 bits
  // Value: [offset][length]
  // 6 bytes address & 2 bytes length
  leveldb::Status MyPut(const leveldb::WriteOptions& options,
                        const leveldb::Slice& key,
                        const leveldb::Slice& value) {
    char save_buf[19];
    real_value_file->seekg(0, std::ios::end);
    std::streampos size = real_value_file->tellg();
    long offset = size;
    std::cout << "文件大小为：" << offset << " 字节" << std::endl;

    int length = value.size();
    printf("[Key] %10s [Value] %s. [Length] %03x\n", key.ToString().c_str(),
           value.ToString().c_str(), length);
    sprintf(save_buf, "%012lx%04x", offset, length);
    real_value_file->write(value.data(), length);

    printf("写入成功\n%s\n", save_buf);
    std::string str = save_buf;
    return leveldb::DB::Put(options, key, "0x" + str);
  }

  leveldb::Status MyGet(const leveldb::ReadOptions& options,
                        const leveldb::Slice& key, std::string* value) {
    auto status = Get(leveldb::ReadOptions(), key, value);
    assert(status.ok());
    std::cout << "[Key] " << key.ToString() << " [Value] " << *value
              << std::endl;

    std::string str = value->substr(2);
    std::string offset_str = str.substr(0, 12);
    std::string length_str = str.substr(12, 4);

    unsigned int offset = std::stoul(offset_str, nullptr, 16);
    unsigned int length = std::stoul(length_str, nullptr, 16);

    std::cout << "[Offset] " << offset << " [Length] " << length << std::endl;

    real_value_file->seekg(offset, std::ios::beg);
    char* buf = new char[length];
    real_value_file->read(buf, length);
    std::string str_value(buf, length);
    delete[] buf;
    *value = str_value;

    return status;
  }

  leveldb::Status Delete(const leveldb::WriteOptions& options,
                         const leveldb::Slice& key);
};
