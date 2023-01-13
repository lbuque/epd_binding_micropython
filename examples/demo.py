import epd
import logo
from time import sleep
import random

epd47 = epd.EPD47()

while True:
    print("display")
    epd47.power(True)
    sleep(0.1)
    x = random.randint(0, 960-200)
    y = random.randint(0, 540-200)
    epd47.bitmap(logo.BITMAP, x, y, logo.WIDTH, logo.HEIGHT)
    epd47.power(False)
    sleep(1)
