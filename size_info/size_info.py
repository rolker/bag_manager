#!/usr/bin/env python3

import rosbag
import sys

from io import BytesIO

for b in sys.argv[1:]:
  bag = rosbag.Bag(b)
  data = {}
  for topic, msg, ts in bag.read_messages():
    if not topic in data:
      data[topic] = []
    buffer = BytesIO()
    msg.serialize(buffer)
    data[topic].append(buffer.__sizeof__())

  grand_total = 0
  for topic in data:
    bytes = 0
    for i in data[topic]:
      bytes += i
    print (bytes,'\t', topic)
    grand_total += bytes
  print(grand_total)
  