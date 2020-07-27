String array_status[] = {"Not Connected",
                         "Connected",
                         "Running"
                        };

void nextionStatusText (int status) {
  NexSerial.print("status.txt=\"Status: " + array_status[status] + "\"");
  NexSerial.write(0xff);
  NexSerial.write(0xff);
  NexSerial.write(0xff);
}

void nextionCounter (int counter) {
  NexSerial.print("counter.val=");
  NexSerial.print(counter);
  NexSerial.write(0xff);
  NexSerial.write(0xff);
  NexSerial.write(0xff);
}

void nextionSetNumber (String Object, int Number) {
  NexSerial.print(Object + ".val=");
  NexSerial.print(Number);
  NexSerial.write(0xff);
  NexSerial.write(0xff);
  NexSerial.write(0xff);
}
