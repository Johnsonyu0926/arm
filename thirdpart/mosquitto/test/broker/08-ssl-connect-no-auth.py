#!/usr/bin/env python3

# Test whether a valid CONNECT results in the correct CONNACK packet using an SSL connection.

if sys.version < '2.7':
    print("WARNING: SSL not supported on Python 2.6")
    exit(0)


def write_config(filename, port1, port2):
    with open(filename, 'w') as f:
        f.write("port %d\n" % (port2))
        f.write("allow_anonymous true\n")
        f.write("\n")
        f.write("listener %d\n" % (port1))
        f.write("allow_anonymous true\n")
        f.write("cafile ../ssl/all-ca.crt\n")
        f.write("certfile ../ssl/server.crt\n")
        f.write("keyfile ../ssl/server.key\n")


(port1, port2) = mosq_test.get_port(2)
conf_file = os.path.basename(__file__).replace('.py', '.conf')
write_config(conf_file, port1, port2)

rc = 1
keepalive = 10
connect_packet = mosq_test.gen_connect("connect-success-test", keepalive=keepalive)
connack_packet = mosq_test.gen_connack(rc=0)

broker = mosq_test.start_broker(filename=os.path.basename(__file__), port=port2, use_conf=True)

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH, cafile="../ssl/test-root-ca.crt")
    ssock = context.wrap_socket(sock, server_hostname="localhost")
    ssock.settimeout(20)
    ssock.connect(("localhost", port1))

    mosq_test.do_send_receive(ssock, connect_packet, connack_packet, "connack")

    rc = 0

    ssock.close()
except mosq_test.TestError:
    pass
finally:
    os.remove(conf_file)
    broker.terminate()
    broker.wait()
    (stdo, stde) = broker.communicate()
    if rc:
        print(stde.decode('utf-8'))

exit(rc)
