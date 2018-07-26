#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include <string>
#include <deque>
#include <boost/thread/mutex.hpp>

namespace kin_logfmt {

  class FileStream {
    public:

    virtual ~FileStream() {}
    virtual void write(const std::string &out) = 0;

    boost::mutex writers_mtx_;
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

    int get_messages_size();
    std::string pop_message();

    private:

    std::deque<std::string> messages_;
  };

}; // namespace kin_logfmt

#endif // FILE_STREAM_H
