#!/usr/bin/env python3

import cgi
import os

class CGIHandler:
    def __init__(self, script_path): #__init__ : costruttore in python. è automaticamente chiamato quando crei un oggetto CGIHandler("..")
        self.script_path = script_path
    
    def execute(self):
        form = cgi.FieldStorage()
    
    #Populate env variables
    env = os.environ.copy()


# Stampiamo gli header della risposta HTTP
print("Content-Type: text/html")
print()  # Riga vuota necessaria

# Ottenere i parametri GET/POST
form = cgi.FieldStorage()

# Esempio: ottenere il valore di un parametro "name"
name = form.getvalue("name", "Utente Anonimo")

# Generare la risposta HTML
print(f"""
<html>
<head><title>CGI Python</title></head>
<body>
    <h1>Ciao, {name}!</h1>
</body>
</html>
""")
