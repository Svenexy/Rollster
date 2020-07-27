String array_status[] = {"Not Connected",
                         "Connected",
                         "Running"
                        };

void nextionStatusText (int status) {
  Serial3.print("status.txt=\"Status: " + array_status[status] + "\"");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}

void nextionCounter (int counter) {
  Serial3.print("counter.val=");
  Serial3.print(counter);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}

void nextionSetNumber (String Object, int Number) {
  Serial3.print(Object + ".val=");
  Serial3.print(Number);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}
