// MtProtoKit ⇄ libteleproto3 SSL ABI bridge (OpenSSL → BoringSSL).
//
// libteleproto3's iOS .a is compiled against OpenSSL 3.x headers, where
// SSL_set_tlsext_host_name() (TLS SNI) expands to the legacy SSL_ctrl() macro.
// Telegram-iOS links that .a against BoringSSL, which deliberately removed
// SSL_ctrl in favour of dedicated setter functions (see boringssl PORTING.md:
// "When porting code which uses SSL_CTX_ctrl or SSL_ctrl, use the replacement").
//
// teleproto3 makes exactly ONE SSL_ctrl-class call in its whole source tree —
// the SNI setter at lib/src/t3_client_stream.c:331 — so this minimal shim
// forwards that single command to BoringSSL's real SSL_set_tlsext_host_name()
// and is provably complete for teleproto3's usage. Any other command is a
// no-op returning 0 (teleproto3 issues none). The symbol is force-loaded
// (alwayslink) so link order cannot drop it before libteleproto3.a needs it.

#include <openssl/ssl.h>

// OpenSSL's SSL_CTRL_SET_TLSEXT_HOSTNAME command value. BoringSSL does not
// define this constant, so it is pinned here with its canonical OpenSSL value.
#define T3_SSL_CTRL_SET_TLSEXT_HOSTNAME 55

long SSL_ctrl(SSL *ssl, int cmd, long larg, void *parg) {
    (void)larg;  // TLSEXT name-type (host_name); implied by the command.
    if (cmd == T3_SSL_CTRL_SET_TLSEXT_HOSTNAME) {
        return (long)SSL_set_tlsext_host_name(ssl, (const char *)parg);
    }
    return 0;
}
