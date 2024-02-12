import mysql.connector
from server import Server
from rgb import RGB
import pandas as pd

conn = mysql.connector.connect( # connect to MySQL DB
    user = 'app_usr',
    password = '7@Rvopcpwc',
    host = 'localhost',
    database = 'sys'
)

pool = conn.cursor() # set DB cursor

def process_rgb(value): # round RGB values up or down
    rgb_list = value.split(',')
    rounded_rgb = [str(round(int(x) / 10) * 10) for x in rgb_list]
    rounded_rgb = ', '.join(map(str, rounded_rgb))
    return rounded_rgb

def calculate_rgb_average(series): # gets the RGB value average (NOT USED)
    avg_r, avg_g, avg_b = 0, 0, 0
    total_items = len(series)

    for item in series:
        r, g, b = map(int, item.split(','))
        avg_r += r
        avg_g += g
        avg_b += b

    avg_r /= total_items
    avg_g /= total_items
    avg_b /= total_items

    return [int(avg_r), int(avg_g), int(avg_b)]


def main():
    server = Server() #start socket server
    server.up()

    try:
        while True:      
            server.connect() # connect and listen to client
            server.clientthread() # receive messages and append to list
            server.handlebyte() # formats data from byte

            res = pd.Series(server.messages) # pass list to series-type column
            res = res.apply(process_rgb)
            print(res)

            mode = res.mode()[0] # find the RGB value with most occurences
            mode = mode.split(',')

            result = [int(i) for i in mode]
            #result = calculate_rgb_average(res)

            rgb = RGB(result[0], result[1], result[2]) # create instance of RGB class
            rgb.evaluate_banana()
            data = rgb.dictgrade()
            print(data)

            # upload result to MySQL DB
            result = pool.execute('''INSERT INTO SensorGrades (r, g, b, maturity_level, char_level, moment)
                                    VALUES (%s, %s, %s, %s, %s, %s)''', 
                                (data['R'], 
                                data['G'], 
                                data['B'], 
                                data['Grade'], 
                                data['Maturity'], 
                                data['Moment']))
            conn.commit()
            server.messages = []

    except KeyboardInterrupt: # handel interrupts
        print("Closing the program...")
        server.server.close()
        conn.close()    
        print("Program closed.")

main()