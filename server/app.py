from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/api/metrics')
def readMetrics():
    since = int(request.args.get('since'))
    items = []
    with open('/workspaces/inspekku/build/collector/memory.db', 'r') as file:
        for line in file.readlines():
            timestamp, metric_name, value = line.strip().split()
            print(timestamp)
            if int(timestamp) >= since:
                items.append({"timestamp": int(timestamp), "metric": metric_name, "value": value})
    
    return jsonify(items)

if __name__ == '__main__':
    app.run(debug=True)
