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

logFmt.stringify(); // = string="some text" bool_true=true float=12.345000 bool_false=false int=10
```

## Todo

- [ ] Parsing

## Licence
Public Domain, do what you like with it (see LICENSE.md)
