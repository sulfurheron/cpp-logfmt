# cpp-logfmt
A basic writer class for Heroku logfmt messages

## Usage

```cpp
LogFmtMessage logFmt;

logFmt.insert("string", "some text");
logFmt.insert("bool_true", true);
logFmt.insert("bool_false", false);
logFmt.insert("int", 10);
logFmt.insert("float", 12.345f);

logFmt.stringify(); // =  bool_false=false bool_true=true float=12.345000 int=10 string="some text"
```

## Todo

- [ ] Parsing

## Licence
Public Domain, do what you like with it (see LICENSE.md)
