#import <MtProtoKit/MT3CallShim.h>
#import "t3_shim_socks5.h"

@implementation MT3CallShim {
    t3_shim_t *_shim;
    NSString  *_shimUsername;
    NSString  *_shimPassword;
    uint16_t   _localPort;
}

- (nullable instancetype)initWithHost:(NSString *)host
                                 port:(uint16_t)port
                                 path:(NSString *)path
                            secretHex:(NSString *)secretHex {
    self = [super init];
    if (!self) return nil;

    t3_shim_t *shim = NULL;
    t3_result_t rc = t3_shim_open(
        host.UTF8String,
        port,
        path.UTF8String,
        secretHex.UTF8String,
        0,
        &shim
    );
    if (rc != T3_OK || shim == NULL) {
        return nil;
    }
    _shim      = shim;
    _localPort = t3_shim_local_port(shim);

    char userBuf[T3_SHIM_CRED_BUFLEN];
    char passBuf[T3_SHIM_CRED_BUFLEN];
    t3_shim_get_credentials(shim, userBuf, sizeof(userBuf), passBuf, sizeof(passBuf));
    _shimUsername = [NSString stringWithUTF8String:userBuf];
    _shimPassword = [NSString stringWithUTF8String:passBuf];

    return self;
}

- (uint16_t)localPort    { return _localPort;    }
- (NSString *)shimUsername { return _shimUsername; }
- (NSString *)shimPassword { return _shimPassword; }

- (void)close {
    if (_shim != NULL) {
        t3_shim_close(_shim);
        _shim = NULL;
    }
}

- (void)dealloc {
    [self close];
}

@end
