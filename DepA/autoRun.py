import os
import sys
argc = len(sys.argv)
argv = sys.argv

cmd = './build-artifacts/Linux/bin/DepA'

hostname = [127, 0, 1, 1]
port = 50100
id = 0x20202020

gds_hostname = [0, 0, 0, 0]
gds_port = 50000

if argc > 1:
    port = port + int(argv[1])

    gds_port += int(argv[1])

    id += int(argv[1])
    hostname[3] += int(argv[1])

    # if address bytes overflows
    if(hostname[3] > 255):
        hostname[2] += 1
        hostname[3] = 0
    if(hostname[2] > 255):
        hostname[1] += 1
        hostname[2] = 0

    hostname_string = str(hostname[0]) + '.' + str(hostname[1]) + '.' + str(hostname[2]) + '.' + str(hostname[3])
    gds_hostname_string = str(gds_hostname[0]) + '.' + str(gds_hostname[1]) + '.' + str(gds_hostname[2]) + '.' + str(gds_hostname[3])
   

    cmd += ' -a ' + gds_hostname_string + ' -p ' + str(gds_port) + ' -j ' + hostname_string + ' -g ' + str(port) + ' -d ' + str(id)

    print('Python script running: ' + cmd)
    os.system(cmd) 