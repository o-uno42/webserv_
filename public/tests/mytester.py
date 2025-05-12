import threading
import requests

def send_request():
    response = requests.get("http://127.0.0.1:3000/")
    print(f"Response: {response.status_code}")

threads = []
for _ in range(500):  # Adjust the number of clients
    thread = threading.Thread(target=send_request)
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()
