from socket import *

serverPort = 12020
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('10.132.20.3', serverPort))
serverSocket.listen(1)

print("The server is ready to receive")

while True:
    connectionSocket, addr = serverSocket.accept()
    unsorted_list = connectionSocket.recv(1024).decode()
    sorted_list = sorted(list(map(lambda str: int(str), str.split(unsorted_list))))
    connectionSocket.send(
        ('The sorted list is: ' + (''.join(map(lambda s: s + ' ', map(str, sorted_list)))))
            .encode())
    connectionSocket.close()
