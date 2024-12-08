#!/usr/bin/env python3

# Test whether a client will is transmitted correctly with a null payload.


def helper(port, proto_ver):
    connect_packet = mosq_test.gen_connect("test-helper", keepalive=60, will_topic="will/null/test",
                                           proto_ver=proto_ver)
    connack_packet = mosq_test.gen_connack(rc=0, proto_ver=proto_ver)
    sock = mosq_test.do_client_connect(connect_packet, connack_packet, port=port)
    sock.close()


def do_test(proto_ver):
    rc = 1
    mid = 53
    keepalive = 60
    connect_packet = mosq_test.gen_connect("will-qos0-test", keepalive=keepalive, proto_ver=proto_ver)
    connack_packet = mosq_test.gen_connack(rc=0, proto_ver=proto_ver)

    subscribe_packet = mosq_test.gen_subscribe(mid, "will/null/test", 0, proto_ver=proto_ver)
    suback_packet = mosq_test.gen_suback(mid, 0, proto_ver=proto_ver)

    publish_packet = mosq_test.gen_publish("will/null/test", qos=0, proto_ver=proto_ver)

    port = mosq_test.get_port()
    broker = mosq_test.start_broker(filename=os.path.basename(__file__), port=port)

    try:
        sock = mosq_test.do_client_connect(connect_packet, connack_packet, timeout=30, port=port)
        mosq_test.do_send_receive(sock, subscribe_packet, suback_packet, "suback")

        helper(port, proto_ver)

        mosq_test.expect_packet(sock, "publish", publish_packet)
        rc = 0

        sock.close()
    except mosq_test.TestError:
        pass
    finally:
        broker.terminate()
        broker.wait()
        (stdo, stde) = broker.communicate()
        if rc:
            print(stde.decode('utf-8'))
            print("proto_ver=%d" % (proto_ver))
            exit(rc)


do_test(proto_ver=4)
do_test(proto_ver=5)
exit(0)
