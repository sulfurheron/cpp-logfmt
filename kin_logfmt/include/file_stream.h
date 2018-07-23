#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include <string>
#include <vector>

namespace kin_logfmt {

  class FileStream {
    public:

    virtual ~FileStream() {}
    virtual void write(const std::string &out) = 0;
  };

  class StdErr : public FileStream {
    public:

    ~StdErr() {}
    void write(const std::string &out);
  };

  class FakeFileStream : public FileStream {
    public:

    ~FakeFileStream() {}
    void write(const std::string &out);

    private:

    std::vector<std::string> messages_;
  };

}; // namespace kin_logfmt

#endif // FILE_STREAM_H
