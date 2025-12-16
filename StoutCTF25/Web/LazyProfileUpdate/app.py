from flask import Flask, render_template, request, redirect
from lxml import etree
import os

app = Flask(__name__)

FLAG = os.environ.get("FLAG")
with open("flag.txt", "w") as f:
  f.write(FLAG)

@app.route('/')
def index():
  return render_template('index.html')

@app.route('/profile')
def profile():
  # Default values
  name = "Unknown"
  bio = "No bio set."
  location = "No location set."

  try:
    with open('profile.xml', 'rb') as f:
      data = f.read()

    # Vulnerable XML parser – XXE enabled
    parser = etree.XMLParser(resolve_entities=True, load_dtd=True, no_network=False)
    doc = etree.fromstring(data, parser=parser)

    name = doc.findtext('name') or name
    bio = doc.findtext('bio') or bio
    location = doc.findtext('location') or location

  except:
    pass  # On any error, fall back to defaults

  return render_template('profile.html', name=name, bio=bio, location=location)


@app.route("/update", methods=["GET", "POST"])
def update():
    if request.method == "POST":
        xml_data = request.form.get("xml", "")

        # Save **exactly** what the attacker posts
        with open("profile.xml", "wb") as f:
            f.write(xml_data.encode())

        return redirect("/profile")

    return render_template("update.html")


if __name__ == "__main__":
  app.run(host="0.0.0.0", port=5000)
