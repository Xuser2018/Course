from socket import *

severName = '192.168.199.109'
severPort = 12000
clientSocket = socket(AF_INET,SOCK_DGRAM)
message = input('input lowercase sentence:')
clientSocket.sendto(message.encode(),(severName,severPort))
modifiedeMessage, serverAddress = clientSocket.recvfrom(2048)
print(modifiedeMessage.decode())
clientSocket.close()