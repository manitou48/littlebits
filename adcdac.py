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
        self.headers = {"Authorization": "Bearer " + self.token, "Accept": "application/vnd.littlebits.v2+json"}
    
    def output(self, pct, dur):
        r = requests.post("https://api-http.littlebitscloud.cc/devices/" + self.id + "/output", data={"percent": pct, "duration_ms": dur}, headers=self.headers)
        return r.text

    def input(self):
        r = requests.get("https://api-http.littlebitscloud.cc/devices/" + self.id + "/input", headers=self.headers, stream=True)
        for line in r.iter_lines():
			#if line: print line
			return line   # just fetch one line
        return

auth = "ec77a8cae80d0699ba4082b23ec96ad72b97b814f2e5f3ed08fbaf69b4fac8dc"
id = "243c200bffd5"
bit = Bit(auth,id)
while(1):
	r=bit.input()
	m = re.match(".*percent.:(\d+)",r)
	if m: bit.output(m.group(1),2000)
