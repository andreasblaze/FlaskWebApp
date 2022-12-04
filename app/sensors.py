from flask import Flask, request, render_template
from flask_login import login_required, current_user

app = Flask(__name__)

sensor_names = {"LI":"light", "HU":"humidity", "ST":"soil_temp",
                "AT":"air_temp", "SM":"soil_moisture"}

@app.route("/sensors")
@login_required
def index():
    return render_template("sensors.html", 
                            user = current_user,
                            graph_code = "<i>put graph here</i>")

@app.route("/write", methods = ['POST'])
def write():
    user = current_user(request)
    d = parse_line(request.data.decode("UTF-8"), user_id = current_user.id)
    print(d, flush=True)
    return {'result': "OK"}, 200

def parse_line(line, user):
    data = {"device" : line[:2],
            "sensor_name" : sensor_names.get(line[2:4], "unkown"),
            "value" : line[4:],
            "user" : current_user}
    return data