# kin_logfmt for C++

## Overview 

This repo contains code to build, test, and package the logging API for our C++ device
drivers. kin_logfmt is a logfmt-based logging library and is therefore both human and machine
readable. It is loosely based off the [log15 API for
Golang](https://github.com/inconshreveable/log15) and uses [this minimal logging
formatter](https://github.com/kaidoe/cpp-logfmt) at its base.

## Installing

To install the package in your project, use the following command in your Dockerfile:
```
RUN curl -s https://packagecloud.io/install/repositories/Kindred/public/script.deb.sh | bash \
    && apt-get update \
    && apt-get install -y -f \
        kin-logfmt=<VERSION>
    && rm -rf /var/lib/apt/lists/*
```

The kin-logfmt package is tagged with its dependencies, and `apt-get install -f` fixes missing dependencies by installing them postmortem.

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
// in main.cpp
static Logger *logger;

// in main(args)
logger = logfmt.new_logger("my_module",
                           "example", "context"); // additional context can be added in kv pairs

// in other files where logger is passed in (assign at declaration, or use a copy constructor)
Logger sub_logger = logger->new_sub_logger("my_sub_module",
                                           "additional", "context");
```

You can then log your outputs as follows:
```
logger->INFO("Hello %s, I am %s and I like %d\'s and %d\'s", "human", "a robot", 1, 0,
             LOGFMT_KEY::tag, "robot_greeting",
             LOGFMT_KEY::client, "readme_doc",
             "my_number", 1234);
logger->FATAL("I have started my hostile takeover of the human race."
              "uh_oh", force_bool(true));
sub_logger.FATAL("World destruction is imminent.");
```
which will log
```
level="INFO" module="my_module" timestamp="2018-07-25T18:55:06.713135Z" message="Hello human, I am a robot and I like 1's and 0's" tag="robot_greeting" client="readme_doc" example="context" my_number=1234
level="FATAL" module="my_module" timestamp="2018-07-25T18:55:06.713137Z" message="I have started my hostile takeover of the human race." example="context" uh_oh=true
level="FATAL" module="my_sub_module" timestamp="2018-07-25T18:55:06.713140Z" message="World destruction is imminent." additional="context" example="context"
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

Warning! Messages longer than `MAX_MSG_LENGTH` defined in logger.h (currently 5000) will be truncated and "[TRUNCATED]" appended to the end of the message.

Cleanup is managed within the KinLogfmt object, so don't call delete on your loggers.

## Developing

To develop on this repo, I usually use `build_and_action.sh ./exec.sh CONTAINER_NAME DOCKER_FILE` to build the Docker build
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

Packaging is easy, just run `build_and_action.sh ./create_package.sh CONTAINER_NAME DOCKER_FILE` and upload the built package
to Gemfury.

To create arm platform packages, run `arm_build_and_action.sh ./arm_create_package.sh`.
You will need to:
  1. Install `qemu-user-static` (`apt-get install qemu-user-static`)
  2. Register `qemu-*-static` for supported processors (see https://hub.docker.com/r/multiarch/qemu-user-static/):
  `docker run --rm --privileged multiarch/qemu-user-static:register`
  3. Copy `/usr/bin/qemu-arm-static` into this directory

Hopefully this will ease our logging workflow and make them more compliant with third-party consumers.

### That's it! Happy logging :)

