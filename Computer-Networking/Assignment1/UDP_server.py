from socket import *
severPort = 12000
severSocket = socket(AF_INET,SOCK_DGRAM)
severSocket.bind(('',severPort))
print("The Server is ready to receive")
while True :
    message, clientAddress = severSocket.recvfrom(2048)
    modifiedMessage = message.decode().upper()
    severSocket.sendto(modifiedMessage.encode(),clientAddress)
    print("receive: ",message.decode())
    print("send :",modifiedMessage)