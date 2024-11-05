// Filename: server_config.cpp
// Score: 95

using namespace std;

class ServerConfig {
public:
    string get_server_reason_string() const;
    void set_server_reason_string(const string& reason);

    string get_server_response_topic() const;
    void set_server_response_topic(const string& topic);

    string get_server_correlation_data() const;
    void set_server_correlation_data(const string& data);

    vector<int> get_server_subscription_identifiers() const;
    void set_server_subscription_identifiers(const vector<int>& ids);

    int get_server_keep_alive_interval() const;
    void set_server_keep_alive_interval(int interval);

    int get_server_receive_maximum() const;
    void set_server_receive_maximum(int max);

    int get_server_topic_alias_maximum() const;
    void set_server_topic_alias_maximum(int max);

    bool get_server_request_problem_information() const;
    void set_server_request_problem_information(bool request);

    bool get_server_request_response_information() const;
    void set_server_request_response_information(bool request);

    properties get_server_user_properties() const;
    void set_server_user_properties(const properties& props);

    string get_server_authentication_method() const;
    void set_server_authentication_method(const string& method);

    string get_server_authentication_data() const;
    void set_server_authentication_data(const string& data);
};

// By GST @Date