import os
import sqlite3
from flask import Flask, request, g, redirect, url_for, render_template, session
import secrets

app = Flask(__name__)
app.secret_key = secrets.token_hex(16)
DATABASE = './challenge.db'

# -------------------------------
# AUTO-CREATE DB
# -------------------------------
def init_db():
    if os.path.exists(DATABASE):
        os.remove(DATABASE)

    conn = sqlite3.connect(DATABASE)
    c = conn.cursor()

    c.execute('''CREATE TABLE users (
        id INTEGER PRIMARY KEY,
        username TEXT,
        password TEXT
    )''')

    # password is the "flag"
    c.execute(
    "INSERT INTO users (username, password) VALUES (?, ?)",
    ("admin", "STOUTCTF{0qyyzWCuwL23ulfAz9PqCA1qRBji0cCe}")
    )

    conn.commit()
    conn.close()
    print("[+] Database created with admin password as flag.")


# -------------------------------
# DB CONNECTION HANDLING
# -------------------------------
def get_db():
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
    return db


@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()


# -------------------------------
# LOGIN ROUTE
# -------------------------------
@app.route('/', methods=['GET', 'POST'])
def login():
    msg = ''
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        # VULNERABLE SQL
        query = f"SELECT * FROM users WHERE username = '{username}' AND password = '{password}'"
        cur = get_db().cursor()
        cur.execute(query)
        user = cur.fetchone()

        if user:
            # generate an auth token only given to legit/logically-bypassed logins
            session["auth_token"] = secrets.token_hex(16)
            return redirect(url_for('dashboard'))

        else:
            msg = 'Invalid credentials'

    return render_template('login.html', msg=msg)


# -------------------------------
# DASHBOARD ROUTE
# -------------------------------
@app.route('/dashboard')
def dashboard():
    token = session.get("auth_token")
    if not token:
        return redirect(url_for('login'))

    FLAG = os.environ.get("FLAG")
    return render_template('dashboard.html', FLAG=FLAG)



# -------------------------------
# ENTRY POINT
# -------------------------------
if __name__ == '__main__':
    init_db()
    app.run(host='0.0.0.0', port=5000)
