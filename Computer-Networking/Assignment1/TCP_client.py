from socket import *
severName = '192.168.199.109'
severPort = 12001
clientSocket = socket(AF_INET,SOCK_STREAM)
clientSocket.connect((severName,severPort))
sentence = input('Input lowercase sentence:')
clientSocket.send(sentence.encode())
modifiedSentence = clientSocket.recv(1024)
print('From Server:',modifiedSentence.decode())
clientSocket.close()