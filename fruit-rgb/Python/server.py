import socket 

HOST = '10.0.0.20'
PORT = 5001
BUFFER_SIZE = 1024

class Server:
    def __init__(self):
        self.host = HOST
        self.port = PORT
        self.buffer = BUFFER_SIZE
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((self.host,self.port))
        self.conn = None
        self.addr = None
        self.messages = []

    def up(self):
        self.server.listen(100)
        print('Server up')
    

    def connect(self):
        self.conn, self.addr = self.server.accept()
        print(f'Connected to: {self.addr}')
        
    
    def clientthread(self):
            while True:
                message = self.conn.recv(self.buffer)
                if message:
                    if not len(message) > 14:
                        self.messages.append(message) 
                    #if len(self.messages) > 20:
                       #self.conn.close()
                else:
                    break
            #self.conn.close()
            

    def handlebyte(self):
        cleaned_strings = [msg.decode('utf-8').strip() for msg in self.messages]
        self.messages = cleaned_strings

    def close():
        server.shutdown(socket.SHUT_RDWR)
        server.close()
        print ("closed")

if __name__ == '__main__':
    server = Server()
    server.up()
    while True:
        server.connect()
        server.clientthread()
        server.messages[-1]
    




