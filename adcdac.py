#!/usr/bin/python
# https://github.com/technoboy10/partly-cloudy
#  read adc  update dac
# streaming SSE has extra data: at front
# could use json?

import re
import requests
class Bit:
    def __init__(self, auth_token, device_id):
        self.token = auth_token
        self.id = device_id
        self.headers = {"Authorization": "Bearer " + self.token}
    
    def output(self, pct, dur):
        r = requests.post("https://api-http.littlebitscloud.cc/devices/" + self.id + "/output", data={"percent": pct, "duration_ms": dur}, headers=self.headers)
        return r.text

    def input(self):
        r = requests.get("https://api-http.littlebitscloud.cc/v3/devices/" + self.id + "/input", headers=self.headers, stream=True)
        for line in r.iter_lines():
			#if line: print line
			yield line   # just fetch one line
        return

auth = "whatever"
id = "whatever0"
bit = Bit(auth,id)
while(1):
	r=bit.input()
	m = re.match(".*percent.:(\d+)",r.next())
	if m: bit.output(m.group(1),3000)
