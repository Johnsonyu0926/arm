#!/usr/bin/env python3

# Test whether a client sends a correct SUBSCRIBE to a topic with QoS 0.

# The client should connect to port 1888 with keepalive=60, clean session set,
# and client id subscribe-qos0-test
# The test will send a CONNACK message to the client with rc=0. Upon receiving
# the CONNACK and verifying that rc=0, the client should send a SUBSCRIBE
# message to subscribe to topic "qos0/test" with QoS=0. If rc!=0, the client
# should exit with an error.
# Upon receiving the correct SUBSCRIBE message, the test will reply with a
# SUBACK message with the accepted QoS set to 0. On receiving the SUBACK
# message, the client should send a DISCONNECT message.

port = mosq_test.get_lib_port()

rc = 1
keepalive = 60
connect_packet = mosq_test.gen_connect("subscribe-qos0-test", keepalive=keepalive)
connack_packet = mosq_test.gen_connack(rc=0)

disconnect_packet = mosq_test.gen_disconnect()

mid = 1
subscribe_packet = mosq_test.gen_subscribe(mid, "qos0/test", 0)
suback_packet = mosq_test.gen_suback(mid, 0)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.settimeout(10)
sock.bind(('', port))
sock.listen(5)

client_args = sys.argv[1:]
env = dict(os.environ)
env['LD_LIBRARY_PATH'] = '../../lib:../../lib/cpp'
try:
    pp = env['PYTHONPATH']
except KeyError:
    pp = ''
env['PYTHONPATH'] = '../../lib/python:' + pp
client = mosq_test.start_client(filename=sys.argv[1].replace('/', '-'), cmd=client_args, env=env, port=port)

try:
    (conn, address) = sock.accept()
    conn.settimeout(10)

    mosq_test.do_receive_send(conn, connect_packet, connack_packet, "connect")
    mosq_test.do_receive_send(conn, subscribe_packet, suback_packet, "subscribe")
    mosq_test.expect_packet(conn, "disconnect", disconnect_packet)
    rc = 0

    conn.close()
except mosq_test.TestError:
    pass
finally:
    client.terminate()
    client.wait()
    sock.close()

exit(rc)
