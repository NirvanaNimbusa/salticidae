#include "salticidae/config.h"
#ifdef SALTICIDAE_CBINDINGS
#include "salticidae/network.h"

using namespace salticidae;

extern "C" {

msgnetwork_config_t *msgnetwork_config_new() {
    return new msgnetwork_config_t();
}

void msgnetwork_config_free(const msgnetwork_config_t *self) { delete self; }

msgnetwork_t *msgnetwork_new(const eventcontext_t *ec, const msgnetwork_config_t *config) {
    return new msgnetwork_t(*ec, *config);
}

void msgnetwork_free(const msgnetwork_t *self) { delete self; }

bool msgnetwork_send_msg(msgnetwork_t *self,
                        const msg_t *msg, const msgnetwork_conn_t *conn) {
    return self->send_msg(*msg, *conn);
}

msgnetwork_conn_t *msgnetwork_connect(msgnetwork_t *self, const netaddr_t *addr) {
    return new msgnetwork_t::conn_t(self->connect(*addr));
}

void msgnetwork_listen(msgnetwork_t *self, const netaddr_t *listen_addr) {
    self->listen(*listen_addr);
}

void msgnetwork_reg_handler(msgnetwork_t *self,
                            _opcode_t opcode,
                            msgnetwork_msg_callback_t cb) {
    self->set_handler(opcode,
        [cb](const msgnetwork_t::Msg &msg, const msgnetwork_t::conn_t &conn) {
            cb(&msg, &conn);
        });
}

void msgnetwork_reg_conn_handler(msgnetwork_t *self, msgnetwork_conn_callback_t cb) {
    self->reg_conn_handler([cb](const ConnPool::conn_t &_conn, bool connected) {
        auto conn = salticidae::static_pointer_cast<msgnetwork_t::Conn>(_conn);
        cb(&conn, connected);
    });
}

void msgnetwork_start(msgnetwork_t *self) { self->start(); }

msgnetwork_t *msgnetwork_conn_get_net(const msgnetwork_conn_t *conn) {
    return (*conn)->get_net();
}

msgnetwork_conn_mode_t msgnetwork_conn_get_mode(const msgnetwork_conn_t *conn) {
    return (msgnetwork_conn_mode_t)(*conn)->get_mode();
}

netaddr_t *msgnetwork_conn_get_addr(const msgnetwork_conn_t *conn) {
    return new netaddr_t((*conn)->get_addr());
}

}

#endif
