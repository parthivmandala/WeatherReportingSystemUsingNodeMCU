from flask import Flask, request
import json
from bot import WeatherMonitering
bot1 = WeatherMonitering()

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello World'


@app.route('/values',methods=['POST'])
async def values():
    values = request.data.decode('utf-8')
    # values = json.loads(values)
    await bot1.send_message(chat_id=5604090678,message=values)
    print(values)
    return 'success'


if __name__ == '__main__':
    app.run(host='192.168.198.195',debug=True,port=5000)
