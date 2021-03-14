from bleak import BleakScanner
from bleak import BleakClient
import asyncio
import time

address = "64:69:4E:7B:5E:0B"
MODEL_NBR_UUID = "00002a24-0000-1000-8000-00805f9b34fb"

char_suffix = "-0000-1000-8000-00805f9b34fb"
read_char = "0000ffe1" + char_suffix

def receive(sender: int, data: bytearray):
    print("[" + str(sender) + "] Received: " + data.decode('utf-8'))

async def scan():
    devices = await BleakScanner.discover()
    for d in devices:
        print(d.name)

client = BleakClient(address)

async def setup():
    await client.connect()
    await client.start_notify(read_char, receive)

    message = "Hello World\n"
    print("Sending '" + message + "' to the device")
    await client.write_gatt_char(read_char, message.encode('utf-8'))

loop = asyncio.get_event_loop()
loop.run_until_complete(setup())

while (loop.run_until_complete(client.is_connected())):
    time.sleep(0.5)

# loop.run_until_complete(scan())