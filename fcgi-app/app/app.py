from datetime import datetime
from flask import Flask
import time
app = Flask(__name__)

@app.route('/')
def hello_world():
    time.sleep(5)
    return 'Hello Asia!'

@app.route('/the-time')
def the_time():
    cur_time = str(datetime.now())
    return cur_time + ' is the current time!  ...YEAH!'

if __name__ == '__main__':
    app.run()