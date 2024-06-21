from flask import Flask, request, jsonify

app = Flask(__name__)

data = []

@app.route('/sensor/data', methods=['POST'])
def post():
    distance = request.form.get("distance")
    print(f"Received distance: {distance}")
    if distance is not None:
        subData = {"distance": distance}
        data.append(subData)
        return 'Data berhasil disimpan di server', 200
    else:
        return 'Gagal menerima data', 400

@app.route('/sensor/data', methods=['GET'])
def get():
    return jsonify(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
