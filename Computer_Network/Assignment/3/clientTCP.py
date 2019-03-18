from socket import *

serverName = "10.132.20.3"
serverPort = 12020
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))

sentence = "1 17 8 233"
clientSocket.send(sentence.encode())

modifiedSentence = clientSocket.recv(1024)
print("From Server:", modifiedSentence.decode())

clientSocket.close()
