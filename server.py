
import socket

sock = socket.socket()
sock.bind(('', 9876))
sock.listen(1)
conn, addr = sock.accept()

print('connected:', addr)

while True:
    data = conn.recv(1024)
    if not data:
        break
    print(data)

conn.close()
