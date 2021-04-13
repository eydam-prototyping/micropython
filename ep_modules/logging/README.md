## Logging

A simple logging module for micropython.

```python
>>> import logging
>>> l = logging.Logger()
>>> l
Logger(hostname='linux', appname='micropython')
>>> l.log_debug("Test")
<135> 2021-04-13T17:57:15Z linux micropython 0 0 Test
```

Default format is `protocol23format`. Other options are `standard` (the only one at the moment).

Each logger has at least one LogHandler, that specifies, what to do with the message. Default option is `LogToConsole`. You can also log to file:

```python
>>> import logging
>>> h = logging.LogHandler(dest=logging.LogToFile)
>>> l = logging.Logger()
>>> l.addHandler(h)
>>> l.log_info("Test")
<134> 2021-04-13T18:03:04Z linux micropython 0 0 Test
```

The default filename is `log.txt` but can be changed in constructor:

```python
>>> h = logging.LogHandler(dest=logging.LogToFile, filename="test.log")
```

Third option is `LogToStream`. Here you can specify a socket:

```python
>>> import logging
>>> import socket
>>> s = socket.socket()
>>> addr_info = socket.getaddrinfo("192.168.178.100", 514)
>>> s.connect(addr_info[0][-1])
>>> h = logging.LogHandler(dest=logging.LogToStream, stream=s)
>>> l = logging.Logger()
>>> l.addHandler(h)
```

Each handler has a formatter, witch sets the log format (`protocol23format` or `standard`). If your bash supports colored output, you can also highlight your Logs:

```python
>>> import logging
>>> l = logging.Logger()
>>> l.handlers[0].formatter.format="standard"
>>> l.handlers[0].formatter.colored = True
>>> l.log_info("Test")
[2021-04-13T18:14:56Z] INFO       linux micropython 0 0 Test
```
(Colored output does not work in markdown)

Currently tested in unix and esp32 port.