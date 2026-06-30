#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// ObjC wrapper around the libteleproto3 SOCKS5 shim (t3_shim_socks5).
/// One instance per 1-to-1 call; -close when the call ends (or on -dealloc).
@interface MT3CallShim : NSObject

/// Opens a SOCKS5 listener that tunnels through Type3 (HTTP-stream).
/// @param host       Type3 server hostname (e.g. "arctic-breeze.my.id")
/// @param port       Type3 server TCP port (typically 443)
/// @param path       HTTP path starting with '/' (e.g. "/api/v1/data")
/// @param secretHex  Full Type3 secret as lower-case hex: "ff<32-hex-key><domain>"
/// @return           Opened shim or nil if the listener could not be bound.
- (nullable instancetype)initWithHost:(NSString *)host
                                 port:(uint16_t)port
                                 path:(NSString *)path
                            secretHex:(NSString *)secretHex;

/// Localhost port the SOCKS5 listener is bound on.
@property (nonatomic, readonly) uint16_t localPort;

/// Auto-generated SOCKS5 username (32 hex chars).
@property (nonatomic, readonly) NSString *shimUsername;

/// Auto-generated SOCKS5 password (32 hex chars).
@property (nonatomic, readonly) NSString *shimPassword;

/// Stops the shim and releases all resources. Safe to call multiple times.
- (void)close;

@end

NS_ASSUME_NONNULL_END
