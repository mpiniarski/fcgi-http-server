from datetime import datetime
from flask import Flask, send_from_directory
import string
import random
import time
import os.path
app = Flask(__name__)

@app.route('/')
def hello_world():
    time.sleep(5)
    return 'Hello Asia!'

@app.route('/the-time')
def the_time():
    cur_time = str(datetime.now())
    return cur_time + ' is the current time!  ...YEAH!'

@app.route('/long/<int:size>')
def image(size):
    return ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(size))



if __name__ == '__main__':
    app.run()