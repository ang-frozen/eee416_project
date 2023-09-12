import serial
import time
import re
import vonage
import csv

fault = 1
# Your Vonage API credentials
api_key = '0cc4f045'
api_secret = 'Mj0OhoWbSG2YC3TS'
# Create a Vonage client
client = vonage.Client(key=api_key, secret=api_secret)
to_phone_number = '+8801748502555'  # Replace with the recipient's phone number
message = 'Accident detected, here is the address!'

# Send an SMS
def send_sms(to_phone_number, message):
    try:
        response = client.sms.send_message({
            'from': 'your_virtual_number',  # Your Vonage virtual number
            'to': to_phone_number,
            'text': message
        })
        print(f"Message sent. Message ID: {response['messages'][0]['message-id']}")
    except Exception as e:
        print(f"Error sending SMS: {str(e)}")
def create_csv_with_columns(csv_file, columns):
    try:
        with open(csv_file, 'w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(columns)
        print(f"CSV file '{csv_file}' created with columns: {columns}")
    except Exception as e:
        print(f"An error occurred: {e}")

def append_to_csv(csv_file, var1, var2, var3, var4, var5):
    try:
        with open(csv_file, 'a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([var1, var2, var3, var4, var5])
        print("Row added successfully.")
    except Exception as e:
        print(f"An error occurred: {e}")
#create csv file
csv_file = 'test2.csv'
columns = ['AccX', 'AccY', 'AccZ', 'longitude', 'Latitude']
create_csv_with_columns(csv_file, columns)
ser = serial.Serial('/dev/ttyUSB0', 9600)  # Change the port as needed
i = 0
msg = ""
try:
    while True:
        data = ser.readline().decode().strip()
        i = i+1
        msg = msg+ " "+ data
        if (i%11 == 0):
            pattern = r' AccX:(-?\d+\.\d+) AccY:(-?\d+\.\d+) AccZ:(-?\d+\.\d+) Roll:(-?\d+\.\d+) Pitch:(-?\d+\.\d+) ACCIDENT:(\d+) ALC:(\d+) cm1:(\d+) cm2:(\d+) Lat:(-?\d+\.\d+) Long:(-?\d+\.\d+)'
            match = re.search(pattern, msg)
            if match:
            # Extract the values using group() method
                AccX = float(match.group(1))
                AccY = float(match.group(2))
                AccZ = float(match.group(3))
                roll = float(match.group(4))
                pitch = float(match.group(5))
                accident = int(match.group(6))
                alc = int(match.group(7))
                cm1 = int(match.group(8))
                cm2 = int(match.group(9))
                lat = float(match.group(10))
                longi = float(match.group(11))
                google_maps_link = f"https://www.google.com/maps?q={lat},{longi}"
                message = 'Accident detected, here is the address! ' + google_maps_link +" "
                print(accident)
                append_to_csv(csv_file, AccX, AccY, AccZ, longi, lat)
                if accident and fault==1:
                    send_sms(to_phone_number, message)
                    fault = 0
                    print("Sent")
                
            #print(msg)
            time.sleep(.0001)
            i = 0
            msg =""
        

except KeyboardInterrupt:
    ser.close()
