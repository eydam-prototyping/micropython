try:
    import logging
except ImportError:
    print("SKIP")
    raise SystemExit

f = logging.LogFormatter()
print(f)

f.format = "standard"
print(f)

f.colored = True
print(f)

h = logging.LogHandler()
print(h)

h = logging.LogHandler(dest=logging.LogToFile)
print(h)
print(h.filename)
try:
    h = logging.LogHandler(dest=logging.LogToStream)
    print("RAISE FAILED")
except:
    print("RAISE VALUE ERROR OK")

import socket
s = socket.socket()
h = logging.LogHandler(dest=logging.LogToStream, stream=s)
print(h)

print(h.formatter)

h = logging.LogHandler(formatter=f)
print(h.formatter)

l = logging.Logger()
print(l)

l.appname = "TestApp"
print(l.appname)
print(l)

l.hostname = "TestHost"
print(l.hostname)
print(l)

h = logging.LogHandler(formatter=f)
l.addHandler(h)
l.handlers[0].formatter.colored=False
l.handlers[1].formatter.colored=False

l.log_debug("test debug")
l.log_info("test info")
l.log_notice("test notice")
l.log_warning("test warning")
l.log_error("test error")
l.log_critical("test critical")
l.log_alarm("test alarm")
l.log_emergency("test emergency")

l.handlers[1].logLevel = 3

l.log_debug("test debug")
l.log_info("test info")
l.log_notice("test notice")
l.log_warning("test warning")
l.log_error("test error")
l.log_critical("test critical")
l.log_alarm("test alarm")
l.log_emergency("test emergency")