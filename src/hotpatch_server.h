#ifndef _H_HOTPATCH_SERVER
#define _H_HOTPATCH_SERVER

#include <map>
#include <string>

namespace hotpatch {

typedef void (*void_function_type)();

class HotpatchServer {

private:
    std::thread socket_server_thread;
    bool should_stop = false;
    std::map<std::string, void*> registered_variables;

    // TODO: Support multiple handers for more libraries
    void* dl_handler;
public:
    HotpatchServer();

    ~HotpatchServer();

    void init();

    void close();

    void register_variable(std::string key, void *p_value);


    void* register_function(std::string func_name, void* p_function);



};

} // End of namespace

#endif