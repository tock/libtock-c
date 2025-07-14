#pragma once

#include "../tock.h"

/* IEEE 802.15.4 system call interface */
// The libtock-c IEEE 802.15.4 driver consists of a set of system calls
// and data types to interface with the kernel. To receive, the user process
// must provide a ring buffer to the kernel of the type `libtock_ieee802154_rxbuf`.
// Upon receiving a frame, the kernel will write the frame into the ring buffer
// and schedule an upcall that is to be be handled by the userprocess. Before
// reading the frame, the userprocess must call `reset_ring_buffer` to
// unallow the ring buffer and unsubscribe upcalls (so as to clear pending upcalls).
// The userprocess can then read the frame from the ring buffer and process it.
// Given the non-deterministic nature of upcalls, the userprocess must carefully
// handle receiving upcalls. There exists a risk of dropping 15.4 packets while
// reading from the ring buffer (as the ring buffer is unallowed while reading).
// This can be handled by utilizing two ring buffers and alternating which
// belongs to the kernel and which is being read from. An example of this can be
// found in libtock-c/libopenthread/platform/system.c. Alternatively,
// the user can also utilize a single ring buffer if dropped frames may be permissible
// (see libtock-c/examples/tests/ieee802154/radio_rx/main.c for an example of this).

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*libtock_ieee802154_callback_send_done)(returncode_t, bool);
typedef void (*libtock_ieee802154_callback_recv_done)(int, int, int);

// Check for presence of the driver
bool libtock_ieee802154_exists(void);

// Disable the 802.15.4 radio. -- NOT SUPPORTED --
int libtock_ieee802154_down(void);

// Returns true if the 802.15.4 radio is up.
returncode_t libtock_ieee802154_is_up(bool* status);

// IEEE 802.15.4 device configuration interface. After any calls to
// ieee_802154_set_*, libtock_ieee802154_config_commit must be called at least once.
// Otherwise, it is not guaranteed that the configuration will reach the underlying
// MAC device. Also note that this state is shared between multiple processes.

// Sets the short MAC address (16 bits).
// `addr` (in): Short MAC address.
returncode_t libtock_ieee802154_set_address_short(uint16_t addr);
// Sets the long MAC address (64 bits)
// `addr_long` (in): Long MAC address. Must point to 8 bytes of valid memory.
returncode_t libtock_ieee802154_set_address_long(uint8_t* addr_long);
// Sets the 802.15.4 PAN ID (16 bits)
// `pan` (in): PAN ID.
returncode_t libtock_ieee802154_set_pan(uint16_t pan);
// Sets the 802.15.4 channel.
// `channel` (in): 802.15.4 channel. 11 <= channel <= 26.
returncode_t libtock_ieee802154_set_channel(uint8_t channel);
// Sets the 802.15.4 transmission power.
// `power` (in): Transmission power. -17 <= power <= 4.
returncode_t libtock_ieee802154_set_power(uint32_t power);
// Commits any new configuration state to the radio.
returncode_t libtock_ieee802154_config_commit(void);

// Gets the short MAC address. Returns TOCK_SUCCESS if the address was
// successfully written into `addr`.
// `addr` (out): Short MAC address.
returncode_t libtock_ieee802154_get_address_short(uint16_t* addr);
// Gets the long MAC address. Returns TOCK_SUCCESS if the address was
// successfully written into `addr_long`.
// `addr_long` (out): Long MAC address. Must point to 8 bytes of valid memory.
returncode_t libtock_ieee802154_get_address_long(uint8_t* addr_long);
// Gets the 802.15.4 PAN ID. Returns TOCK_SUCCESS if the PAN ID was successfully
// written into `pan`.
// `pan` (out): PAN ID.
returncode_t libtock_ieee802154_get_pan(uint16_t* pan);
// Gets the 802.15.4 channel. Returns TOCK_SUCCESS if the channel was
// successfully written into `channel`.
// `channel` (out): 802.15.4 channel. If successful, `channel` will satisfy 11
// <= channel <= 26.
returncode_t libtock_ieee802154_get_channel(uint8_t* channel);
// Gets the 802.15.4 transmission power. Returns TOCK_SUCCESS if the power
// was successfully written into `power`.
// `power` (out): Transmission power. If successful, `power` will satisfy
// -17 <= power <= 4.
returncode_t libtock_ieee802154_get_power(uint32_t* power);

// Turn the radio off.
returncode_t libtock_ieee802154_radio_on(void);

// Turn the radio on.
returncode_t libtock_ieee802154_radio_off(void);

// IEEE 802.15.4 neighbor list management. The list of known neighbors is
// implemented as a variable-sized (up to a maximum of
// `libtock_ieee802154_max_neighbors()`) list of (short address, long address) pairs.
// List indices are maintained in the range [0, `libtock_ieee802154_max_neighbors()` -
// 1] and are stable between calls to `libtock_ieee802154_remove_neighbor()`.

// Retrieves the maximum number of neighbors supported.
returncode_t libtock_ieee802154_max_neighbors(uint32_t* neighbors);
// Retrieves the current number of neighbors.
returncode_t libtock_ieee802154_num_neighbors(uint32_t* neighbors);
// Retrieves the short address of the neighbor at index `index` into `addr`.
// If successful, returns TOCK_SUCCESS.
// `index` (in): Index in neighbor list.
// `addr` (out): Short address of neighbor at `index`.
returncode_t libtock_ieee802154_get_neighbor_address_short(uint32_t index, uint16_t* addr);
// Retrieves the long address of the neighbor at index `index` into `addr_long`.
// If successful, returns TOCK_SUCCESS.
// `index` (in): Index in neighbor list.
// `addr_long` (out): Long address of neighbor at `index`. Must point to 8
// bytes of valid memory.
returncode_t libtock_ieee802154_get_neighbor_address_long(uint32_t index, uint8_t* addr_long);
// Retrieves the neighbor at index `index` into `addr` and `addr_long`.
// If successful, returns TOCK_SUCCESS.
// `index` (in): Index in neighbor list.
// `addr` (out): Short address of neighbor at `index`.
// `addr_long` (out): Long address of neighbor at `index`. Must point to 8
// bytes of valid memory.
returncode_t libtock_ieee802154_get_neighbor(uint32_t  index,
                                             uint16_t* addr_short,
                                             uint8_t*  addr_long);
// Adds a new neighbor to the neighbor list.
// If successful, returns TOCK_SUCCESS and writes the list index of the new neighbor
// or existing neighbor with matching addresses into `index`.
// `addr` (in): Short address of new neighbor.
// `addr_long` (in): Long address of new neighbor. Must point to 8 bytes of valid memory.
// `index` (out): New index in neighbor list. Can be NULL if the index is not needed.
returncode_t libtock_ieee802154_add_neighbor(uint16_t addr_short, uint8_t* addr_long, uint32_t* index);
// Removes the neighbor at `index`. If successful, returns TOCK_SUCCESS,
// otherwise TOCK_EINVAL.
returncode_t libtock_ieee802154_remove_neighbor(uint32_t index);

// IEEE 802.15.4 key list management. The list of known keys is implemented as
// a variable-sized (up to a maximum of `libtock_ieee802154_max_keys()`) list of
// (security level, key id, key) tuples.  List indices are maintained in the
// range [0, `libtock_ieee802154_max_keys()` - 1] and are stable between calls to
// `libtock_ieee802154_remove_key()`.

// Enum for representing IEEE 802.15.4 security levels in C.
typedef enum {
  SEC_LEVEL_NONE      = 0,
  SEC_LEVEL_MIC32     = 1,
  SEC_LEVEL_MIC64     = 2,
  SEC_LEVEL_MIC128    = 3,
  SEC_LEVEL_ENCMIC32  = 5,
  SEC_LEVEL_ENCMIC64  = 6,
  SEC_LEVEL_ENCMIC128 = 7,
} security_level_t;

// Enum for representing IEEE 802.15.4 key ID modes in C.
typedef enum {
  KEY_ID_IMPLICIT    = 0,
  KEY_ID_INDEX       = 1,
  KEY_ID_SRC_4_INDEX = 2,
  KEY_ID_SRC_8_INDEX = 3,
} key_id_mode_t;

// Retrieves the maximum number of keys supported.
returncode_t libtock_ieee802154_max_keys(uint32_t* keys);
// Retrieves the current number of keys.
returncode_t libtock_ieee802154_num_keys(uint32_t* keys);
// Retrieves the security level of the key at index `index` into `level`.
// If successful, returns TOCK_SUCCESS.
// `index` (in): Index in key list.
// `level` (out): Security level of key at `index`. Will not be SEC_LEVEL_NONE.
returncode_t libtock_ieee802154_get_key_security_level(uint32_t index, security_level_t* level);
// Retrieves the key id of the key at index `index` into `key_id_mode`
// and `key_id`. If successful, returns TOCK_SUCCESS.
// `index` (in): Index in key list.
// `key_id_mode` (out): Key ID mode of key at `index`.
// `key_id` (out): Optional data depending on the value of `key_id_mode`.
// Must point to 9 bytes of valid memory.
//    - KEY_ID_IMPLICIT: This parameter is meaningless.
//    - KEY_ID_INDEX: The key index (1 byte) will be written to `key_id`.
//    - KEY_ID_SRC_4_INDEX: The key source (4 bytes) and index (1 byte) will
//      be written to `key_id`.
//    - KEY_ID_SRC_8_INDEX: The key source (8 bytes) and index (1 byte) will
//      be written to `key_id`.
returncode_t libtock_ieee802154_get_key_id(uint32_t       index,
                                           key_id_mode_t* key_id_mode,
                                           uint8_t*       key_id);
// Returns the number of bytes that will be taken up by a key id with the given
// `key_id_mode`. Returns either 0, 1, 5, or 9. If the key ID mode is invalid,
// returns 0.  See `libtock_ieee802154_get_key_id()` for details.
returncode_t libtock_ieee802154_key_id_bytes(key_id_mode_t key_id_mode);
// Retrieves the key at index `index` into `key`.
// If successful, returns TOCK_SUCCESS.
// `index` (in): Index in key list.
// `key` (out): Key. Must point to 16 bytes of valid memory.
returncode_t libtock_ieee802154_get_key(uint32_t index, uint8_t* key);
// Retrieves the key at index `index` along with all the accompanying information.
// If successful, returns TOCK_SUCCESS.
// `index` (in): Index in key list.
// `level` (out): Security level of key at `index`. Will not be SEC_LEVEL_NONE.
// `key_id_mode` (out): Key ID mode of key at `index`.
// `key_id` (out): Optional data depending on the value of `key_id_mode`.
// Must point to 9 bytes of valid memory. See `libtock_ieee802154_get_key_id()` for details.
// `key` (out): Key. Must point to 16 bytes of valid memory.
returncode_t libtock_ieee802154_get_key_desc(uint32_t          index,
                                             security_level_t* level,
                                             key_id_mode_t*    key_id_mode,
                                             uint8_t*          key_id,
                                             uint8_t*          key);
// Adds a new key into the list of keys, if space remains.
// If successful, returns TOCK_SUCCESS and writes the list index of the new key
// or existing key with matching addresses into `index`. If an existing key
// has the same security level and key ID, returns TOCK_EINVAL.
// `level` (in): Security level. Must not be SEC_LEVEL_NONE.
// `key_id_mode` (in): Key ID mode.
// `key_id` (in): Optional data depending on the value of `key_id_mode`.
//    - KEY_ID_IMPLICIT: This parameter is meaningless and can be `NULL`.
//    - KEY_ID_INDEX: The key index (1 byte). Must point to 1 byte of valid memory.
//    - KEY_ID_SRC_4_INDEX: The key source (4 bytes) and index (1 byte). Must point to
//      5 bytes of valid memory.
//    - KEY_ID_SRC_8_INDEX: The key source (8 bytes) and index (1 byte). Must point to
//      9 bytes of valid memory.
// `key` (in): The key. Must point to 16 bytes of valid memory.
returncode_t libtock_ieee802154_add_key(security_level_t level,
                                        key_id_mode_t    key_id_mode,
                                        uint8_t*         key_id,
                                        uint8_t*         key,
                                        uint32_t*        index);
// Removes the key at `index`. If successful, returns TOCK_SUCCESS,
// otherwise TOCK_EINVAL.
returncode_t libtock_ieee802154_remove_key(uint32_t index);

// IEEE 802.15.4 transmission and reception functions.
// Transmission is sequenced across multiple processes, but received frames are exposed to all
// processes.

// Sends an IEEE 802.15.4 frame asynchronously. The desired key must first be
// added to the key list.  It is then looked up with the security level and key
// ID provided. Returns TOCK_SUCCESS or TOCK_ENOACK on successful transmission,
// depending on whether or not an ACK was received.
// `addr` (in): Destination short MAC address.
// `level` (in): Security level desired. Can be SEC_LEVEL_NONE, in which case
//   `key_id_mode` is meaningless and can be set to 0.
// `key_id_mode` (in): The key ID mode, if `level` is not SEC_LEVEL_NONE.
// `key_id` (in): Optional data depending on the value of `key_id_mode` if
//   `level` is not SEC_LEVEL_NONE. See `libtock_ieee802154_add_key`.
// `payload` (in): Buffer containing the desired frame payload. Must point to
//   `len` bytes of valid memory.
// `len` (in): Length of frame payload.
// `cb` (in): Callback to call when the frame is sent. The callback will receive two arguments:
//   - `result`: The result of the send operation (returncode).
//   - `acked`: Whether the frame was acknowledged by the receiver.
returncode_t libtock_ieee802154_send(uint32_t                              addr,
                                     security_level_t                      level,
                                     key_id_mode_t                         key_id_mode,
                                     uint8_t*                              key_id,
                                     const uint8_t*                        payload,
                                     uint8_t                               len,
                                     libtock_ieee802154_callback_send_done cb);

// Sends an IEEE 802.15.4 frame asynchronously. This is an alternative and contemporary
// to the `libtock_ieee802154_send` function. This provides an interface for userprocesses to
// form a frame (including headers, security, CRC etc) entirely in the userprocess.
// `libtock_ieee802154_send_raw` then takes this formed frame buffer and passes the frame
// to the 15.4 capsule which sends the buffer (without altering the frame).
// `payload` (in): Buffer containing the desired frame payload.
// `len` (in): Length of frame payload.
// `cb` (in): Callback to call when the frame is sent. The callback will receive two arguments:
//   - `result`: The result of the send operation (returncode).
//   - `acked`: Whether the frame was acknowledged by the receiver.
returncode_t libtock_ieee802154_send_raw(const uint8_t*                        payload,
                                         uint8_t                               len,
                                         libtock_ieee802154_callback_send_done cb);

// Maximum size required of a buffer to contain the IEEE 802.15.4 frame data
// passed to userspace from the kernel. Consists of 3 extra bytes followed by
// the whole IEEE 802.15.4 MTU, which is 127 bytes. The packet is of the following
// form: | data_offset | data_len | mic_len | 15.4 frame |
#define libtock_ieee802154_FRAME_META_LEN 3
#define libtock_ieee802154_FRAME_LEN (libtock_ieee802154_FRAME_META_LEN + 127)

// Size of the ring buffer expected by the kernel. The ring buffer is of the following
// form: | read index | write index | frame 1 | frame 2 | ... | frame n |.
// Dependent on the application, the number of frames contained in the ring buffer
// can be increased or decreased by changing the value of libtock_ieee802154_MAX_RING_BUF_FRAMES.
#define libtock_ieee802154_RING_BUF_META_LEN 2
#define libtock_ieee802154_MAX_RING_BUF_FRAMES 3
#define libtock_ieee802154_RING_BUFFER_LEN (libtock_ieee802154_RING_BUF_META_LEN + \
                                            libtock_ieee802154_MAX_RING_BUF_FRAMES * libtock_ieee802154_FRAME_LEN)

// Type for the 15.4 ring buffer.
typedef uint8_t libtock_ieee802154_rxbuf[libtock_ieee802154_RING_BUFFER_LEN];

// Waits asynchronously for an IEEE 802.15.4 frame. Only waits for one frame.
// To receive more, subscribe to this event again after processing one.
// `callback` (in): Callback to call when a frame is received.
// `frame` (in): Buffer in which to put the full IEEE 802.15.4 frame data. See
//   `libtock_ieee802154_receive_sync` for more details.
//
// The callback will receive three arguments containing information about the header
// of the received frame:
// `pans`: ((destination PAN ID if present else 0) << 16) | (source PANID if present else 0)
// `dst_addr`: (addressing mode << 16) | (short address if address is short else 0)
// `src_addr`: (addressing mode << 16) | (short address if address is short else 0)
returncode_t libtock_ieee802154_receive(
  const libtock_ieee802154_rxbuf*       frame,
  libtock_ieee802154_callback_recv_done cb);

// IEEE 802.15.4 received frame inspection functions. The frames are returned
// to userspace in a particular format that might include more bytes than just
// the raw 802.15.4 frame. In all of the below calls, `frame` is assumed to be
// a non-null pointer to a frame that was received through this userspace driver.

typedef enum {
  ADDR_NONE  = 0,
  ADDR_SHORT = 2,
  ADDR_LONG  = 3,
} addr_mode_t;

// Gets the length of the received frame.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
int libtock_ieee802154_frame_get_length(const uint8_t* frame);
// Gets the offset into `frame` of the data payload in the frame.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
int libtock_ieee802154_frame_get_payload_offset(const uint8_t* frame);
// Gets the length of the data payload in the frame.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
int libtock_ieee802154_frame_get_payload_length(const uint8_t* frame);
// Gets the destination address of the received frame. Returns the addressing
// mode, and if an address is present, writes the address into `short_addr` or
// `long_addr`. If the out parameters are provided as NULL, this just returns
// the addressing mode. Also returns ADDR_NONE if the frame is invalid.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
// `short_addr` (out): The destination address of the frame, if it is short.
// `long_addr` (out): The destination address of the frame, if it is long. Must
// point to 8 bytes of valid memory, if not null.
addr_mode_t libtock_ieee802154_frame_get_dst_addr(const uint8_t* frame,
                                                  uint16_t*      short_addr,
                                                  uint8_t*       long_addr);
// Gets the source address of the received frame. Returns the addressing mode,
// and if an address is present, writes the address into `short_addr` or
// `long_addr`. If the out parameters are provided as NULL, this just returns
// the addressing mode. Also returns ADDR_NONE if the frame is invalid.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
// `short_addr` (out): The source address of the frame, if it is short.
// `long_addr` (out): The source address of the frame, if it is long. Must
// point to 8 bytes of valid memory, if not null.
addr_mode_t libtock_ieee802154_frame_get_src_addr(const uint8_t* frame,
                                                  uint16_t*      short_addr,
                                                  uint8_t*       long_addr);
// Gets the destination PAN ID of the received frame. Returns `true` if it
// is present and writes it into `pan`, otherwise returns `false`.
// Also returns `false` if the frame is invalid in any way.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
// `pan` (out): The destination PAN ID if it is present. Can be set to NULL, in
// which case nothing will be written.
bool libtock_ieee802154_frame_get_dst_pan(const uint8_t* frame,
                                          uint16_t*      pan);
// Gets the source PAN ID of the received frame. Returns `true` if it is
// present and writes it into `pan`, otherwise returns `false`. The source PAN
// ID is considered "present" if it is either included explicitly or is set to
// match the destination PAN ID.
// Also returns `false` if the frame is invalid in any way.
// `frame` (in): The frame data provided by libtock_ieee802154_receive_*.
// `pan` (out): The source PAN ID if it is present. Can be set to NULL, in
// which case nothing will be written.
bool libtock_ieee802154_frame_get_src_pan(const uint8_t* frame,
                                          uint16_t*      pan);

// Reads the next frame from the ring buffer. If the ring buffer is empty,
// returns NULL. The pointer returned points to the first index of the
// received frame.
uint8_t* libtock_ieee802154_read_next_frame(const libtock_ieee802154_rxbuf* frame);

// Resets the ring buffer shared with the kernel to either be disabled or prepared for the next
// received packet. To disable the ring buffer, the user should pass a NULL value to the `frame`
// and `callback` arguments. To prepare for the next received packets, the caller should pass the
// relevant buffer/callback to the `frame` and  `callback` arguments. Note, this function clears
// all pending RX upcalls.
returncode_t libtock_reset_ring_buf(const libtock_ieee802154_rxbuf* frame, subscribe_upcall callback, void* ud);

#ifdef __cplusplus
}
#endif
