#pragma once

#include <libtock/net/ieee802154.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_ieee802154_exists(void);

// Sends an IEEE 802.15.4 frame synchronously. The desired key must first be
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
returncode_t libtocksync_ieee802154_send(uint16_t         addr,
                                         security_level_t level,
                                         key_id_mode_t    key_id_mode,
                                         uint8_t*         key_id,
                                         const uint8_t*   payload,
                                         uint8_t          len);

// Sends an IEEE 802.15.4 frame synchronously. This is an alternative and contemporary
// to the `libtock_ieee802154_send` function. This provides an interface for userprocesses to
// form a frame (including headers, security, CRC etc) entirely in the userprocess.
// `libtock_ieee802154_send_raw` then takes this formed frame buffer and passes the frame
// to the 15.4 capsule which sends the buffer (without altering the frame).
// `payload` (in): Buffer containing the desired frame payload.
// `len` (in): Length of frame payload.
returncode_t libtocksync_ieee802154_send_raw(const uint8_t* payload,
                                             uint8_t        len);

// Waits synchronously for an IEEE 802.15.4 frame.
// `frame` (in): Buffer in which to put the full IEEE 802.15.4 frame data. Note
//   that the data written might include more than just the IEEE 802.15.4 frame itself.
//   Use `libtock_ieee802154_frame_get_*` to interact with the resulting frame.
returncode_t libtocksync_ieee802154_receive(const libtock_ieee802154_rxbuf* frame);

// Synchronously enable the 802.15.4 radio. Returns once the radio is fully
// initialized.
returncode_t libtocksync_ieee802154_up(void);

#ifdef __cplusplus
}
#endif
