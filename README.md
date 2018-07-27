# kin_logfmt for C++

## Overview 

This repo contains code to build, test, and package the logging API for our C++ device
drivers. kin_logfmt is a logfmt-based logging library and is therefore both human and machine
readable. It is loosely based off the [log15 API for
Golang](https://github.com/inconshreveable/log15) and uses [this minimal logging
formatter](https://github.com/kaidoe/cpp-logfmt) at its base.

## Usage

Make sure to include the package header files and the kin_logfmt namespace using
```
#include <kin_logfmt/kin_logfmt.h>

using namespace kin_logfmt;
```

In your `main.cpp` file, define your family of loggers with a log level and a file stream:
```
KinLogfmt logfmt(INFO_, new FakeFileStream()); // If no file stream is passed in, std::cerr will be used as default
```

Then, define new loggers for your modules like so:
```
Logger *logger = logfmt.newLogger("my_module",
                                  "example", "context"); // additional context can be added in kv pairs
```

You can then log your outputs as follows:
```
logger->INFO("Hello %s, I am %s and I like %d\'s and %d\'s", "human", "a robot", 1, 0,
             LOGFMT_KEY::tag, "robot_greeting",
             LOGFMT_KEY::client, "readme_doc",
             "my_number", 1234);
logger->FATAL("I have started my hostile takeover of the human race."
              "uh_oh", force_bool(true));
```
which will log
```
level="INFO" module="my_module" timestamp="2018-07-25T18:55:06.713135Z" message="Hello human, I am a robot and I like 1's and 0's" tag="robot_greeting" client="readme_doc" example="context" my_number=1234
level="FATAL" module="my_module" timestamp="2018-07-25T18:55:06.713137Z" message="I have started my hostile takeover of the human race." example="context" uh_oh=true
```

Notice the function force_bool() wrapped around our bool literal. This
is needed to ensure proper type deduction and is a C++ quirk I had to
learn the hard way. [Read about it
here.](https://stackoverflow.com/questions/13268608/boostvariant-why-is-const-char-converted-to-bool)

The predefined LOGFMT_KEYs are as follows, and should be used wherever possible to standardize across logs:
```
level,      // metadata only, do not call
module,     // metadata only, do not call
timestamp,  // metadata only, do not call
msg,        // automatically configured, do not call
tag,
request_id,
client,
```

Cleanup is managed within the KinLogfmt object, so don't call delete on your loggers.

## Developing

To develop on this repo, I usually use `build_and_action.sh ./exec.sh` to build the Docker build
container, install the libraries & executables, and kick me into the container for further
testing. If they change during development, headers, libraries & executables can be rebuilt and
reinstalled using
```
cd /logfmt/kin_logfmt/build      // or /logfmt/cpp-logfmt/build
make install
```
Then, tests can be run with
```
/output/usr/bin/test_logfmt_util // tests the barebones logfmt data structure (cpp-logfmt)
/output/usr/bin/logger_test      // tests the kin_logfmt wrapper
```

## Packaging

Packaging is easy, just run `build_and_action.sh ./create_package.sh` and upload the built package
to Gemfury.

Hopefully this will ease our logging workflow and make them more compliant with third-party consumers.

### That's it! Happy logging :)

