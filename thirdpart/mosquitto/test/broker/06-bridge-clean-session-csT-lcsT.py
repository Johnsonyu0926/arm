#!/usr/bin/env python3
# Test whether a broker handles cleansession and local_cleansession correctly on bridges

(port_a_listen, port_b_listen) = mosq_test.get_port(2)
subprocess.run(['./06-bridge-clean-session-core.py', str(port_a_listen), str(port_b_listen), "True", "True"])
