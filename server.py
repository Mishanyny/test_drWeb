from flask import Flask, request, jsonify, send_from_directory, abort
import os
from werkzeug.utils import secure_filename
import hashlib


app = Flask(__name__)


@app.before_request
def check_store_path():
    if not os.path.exists('store'):  
        os.makedirs('store')


@app.route('/upload/', methods = ['GET', 'POST'])
def upload():

    if request.method == 'POST':
        file = request.files['files']
        if file:
            filename = hashlib.md5(bytes(file.filename, 'utf-8')).hexdigest()
            path = os.path.join('store', filename[0:2])
            print(path)
            if os.path.exists(path):
                file.save(os.path.join(path, filename))
            else:
                os.makedirs(path)
                file.save(os.path.join(path, filename))
            return jsonify({'success':True, 'hash':filename})


@app.route('/download/<hash_file>')
def download(hash_file):

    path = os.path.join('store', hash_file[0:2])
    fi = os.path.join(path, hash_file)
    if os.path.exists(fi):
        return send_from_directory(path, hash_file)
    else:
        abort(404)


@app.route('/delete/<hash_file>')
def delete(hash_file):

    path = os.path.join('store', hash_file[0:2])
    fi = os.path.join(path, hash_file)
    if os.path.exists(fi):
        os.remove(fi)
        return jsonify({'success':True})
    else:
        abort(404)
    


if __name__ == "__main__":
    app.run(debug = True, port = 8080, host = '0.0.0.0')

