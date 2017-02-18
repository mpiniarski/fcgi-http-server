from datetime import datetime
from flask import Flask, send_from_directory, request
import string
import random
import time
import os.path

app = Flask(__name__)


@app.route('/hello-world')
def hello_world():
    time.sleep(1)
    return 'Hello world. This is awesome FCGI!'


@app.route('/api/the-time')
def the_time():
    cur_time = str(datetime.now())
    return cur_time + ' is the current time!  ...YEAH!'


@app.route('/api/chars/<int:size>')
def image(size):
    return ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(size))

@app.route('/api/post', methods=['POST'])
def post():
    data = request.form['text']
    with open('/srv/http_server/uploaded.txt', 'a') as f:
        f.write(str(data))
    return 'uploaded'

@app.route('/api/file', methods=['POST'])
def post2():
    data = request.files['file']
    return data.filename + ' changed'

if __name__ == '__main__':
    app.run()
