#include <string.h>

#include "ieee802154.h"

bool libtock_ieee802154_driver_exists(void) {
  return driver_exists(DRIVER_NUM_IEEE802154);
}

// Temporary buffer used for some commands where the system call interface
// parameters / return codes are not enough te contain the required data.
uint8_t BUF_CFG[27];

int libtock_ieee802154_down(void) {
  // Currently unsupported: there is no way to implement this with the existing
  // radio interface.
  return RETURNCODE_ENOSUPPORT;
}

returncode_t libtock_ieee802154_is_up(bool* status) {
  returncode_t ret = libtock_ieee802154_command_status();
  if (ret == RETURNCODE_EOFF) {
    *status = false;
  } else if (ret == RETURNCODE_SUCCESS) {
    *status = true;
  }

  return ret;
}

returncode_t libtock_ieee802154_set_address_short(uint16_t addr_short) {
  return libtock_ieee802154_command_set_address_short(addr_short);
}

returncode_t libtock_ieee802154_set_address_long(uint8_t* addr_long) {
  return libtock_ieee802154_command_set_address_long_u64(*((uint64_t*) addr_long));
}

returncode_t libtock_ieee802154_set_pan(uint16_t pan) {
  return libtock_ieee802154_command_set_pan(pan);
}

returncode_t libtock_ieee802154_set_channel(uint8_t channel) {
  return libtock_ieee802154_command_set_channel(channel);
}

returncode_t libtock_ieee802154_set_power(uint32_t power) {
  // Cast the signed uint8_t to an uint8_t before zero-padding it.
  return libtock_ieee802154_command_set_power(power);
}

returncode_t libtock_ieee802154_radio_on(void) {
  return libtock_ieee802154_command_radio_on();
}

returncode_t libtock_ieee802154_radio_off(void) {
  return libtock_ieee802154_command_radio_off();
}

returncode_t libtock_ieee802154_config_commit(void) {
  return libtock_ieee802154_command_config_commit();
}

returncode_t libtock_ieee802154_get_address_short(uint16_t* addr) {
  if (!addr) return RETURNCODE_EINVAL;

  uint32_t addr_ret;  // Command returncode holds u32 return value.
  returncode_t ret = libtock_ieee802154_command_get_address_short(&addr_ret);

  // Driver adds 1 to make the value positive.
  *addr = addr_ret - 1;

  return ret;
}

returncode_t libtock_ieee802154_get_address_long(uint8_t* addr_long) {
  return libtock_ieee802154_command_get_address_long_u64((uint64_t*) addr_long);
}

returncode_t libtock_ieee802154_get_pan(uint16_t* pan) {
  if (!pan) return RETURNCODE_EINVAL;

  uint32_t pan_ret;  // Command returncode holds u32 return value.
  returncode_t ret = libtock_ieee802154_command_get_pan(&pan_ret);

  // Driver adds 1 to make the value positive.
  *pan = pan_ret - 1;

  return ret;
}

returncode_t libtock_ieee802154_get_channel(uint8_t* channel) {
  if (!channel) return RETURNCODE_EINVAL;

  uint32_t channel_ret;  // Command returncode holds u32 return value.
  returncode_t ret = libtock_ieee802154_command_get_channel(&channel_ret);

  // Driver adds 1 to make the value positive.
  *channel = channel_ret - 1;
  return ret;
}

returncode_t libtock_ieee802154_get_power(uint32_t* power) {
  if (!power) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_command_get_power(power);

  // Driver adds 1 to the power after casting it to unsigned, so this works
  *power -= 1;

  return ret;
}

returncode_t libtock_ieee802154_max_neighbors(uint32_t* neighbors) {
  if (!neighbors) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_command_get_max_neighbors(neighbors);

  // Driver adds 1 to the power after casting it to unsigned, so this works
  *neighbors -= 1;

  return ret;
}

returncode_t libtock_ieee802154_num_neighbors(uint32_t* num_neighbors) {
  if (!num_neighbors) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_command_get_number_neighbors(num_neighbors);

  // Driver adds 1 to the power after casting it to unsigned, so this works
  *num_neighbors -= 1;

  return ret;
}

returncode_t libtock_ieee802154_get_neighbor_address_short(uint32_t index, uint16_t* addr) {
  if (!addr) return RETURNCODE_EINVAL;

  uint32_t addr_ret;  // Command returncode holds u32 return value.
  returncode_t ret = libtock_ieee802154_command_get_neighbor_address_short(index, &addr_ret);

  // Driver adds 1 to ensure it is positive.
  *addr = addr_ret - 1;

  return ret;
}

returncode_t libtock_ieee802154_get_neighbor_address_long(uint32_t index, uint8_t* addr_long) {
  if (!addr_long) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) addr_long, 8);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_ieee802154_command_get_neighbor_address_long(index);

  // unallow the rw buffer from the kernel so that other libtock functions
  // can utilize and modify the buffer
  libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);

  return ret;
}

returncode_t libtock_ieee802154_get_neighbor(uint32_t  index,
                                             uint16_t* addr_short,
                                             uint8_t*  addr_long) {
  returncode_t ret = libtock_ieee802154_get_neighbor_address_short(index, addr_short);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtock_ieee802154_get_neighbor_address_long(index, addr_long);
}

returncode_t libtock_ieee802154_add_neighbor(uint16_t addr_short, uint8_t* addr_long, uint32_t* index) {
  if (!addr_long) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) addr_long, 8);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_ieee802154_command_add_neighbor(addr_short, index);

  // unallow the rw buffer from the kernel so that other libtock functions
  // can utilize and modify the buffer
  libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);

  if (ret != RETURNCODE_SUCCESS) return ret;

  // Driver adds 1 to ensure it is positive.
  *index -= 1;

  return ret;
}

returncode_t libtock_ieee802154_remove_neighbor(uint32_t index) {
  return libtock_ieee802154_command_remove_neighbor(index);
}

returncode_t libtock_ieee802154_max_keys(uint32_t* max_keys) {
  returncode_t ret = libtock_ieee802154_command_get_max_keys(max_keys);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Driver adds 1 to ensure it is positive.
  *max_keys -= 1;

  return ret;
}

returncode_t libtock_ieee802154_num_keys(uint32_t* key_count) {
  returncode_t ret = libtock_ieee802154_command_get_num_keys(key_count);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Driver adds 1 to ensure it is positive.
  *key_count -= 1;

  return ret;
}

returncode_t libtock_ieee802154_get_key_security_level(uint32_t index, security_level_t* level) {
  if (!level) return RETURNCODE_EINVAL;

  uint32_t ret_level;
  returncode_t ret = libtock_ieee802154_command_get_key_security_level(index, &ret_level);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Driver adds 1 to ensure it is positive.
  ret_level -= 1;
  *level     = (security_level_t) ret_level;

  return ret;
}

returncode_t libtock_ieee802154_key_id_bytes(key_id_mode_t key_id_mode) {
  switch (key_id_mode) {
    default:
    case KEY_ID_IMPLICIT:
      return 0;
    case KEY_ID_INDEX:
      return 1;
    case KEY_ID_SRC_4_INDEX:
      return 5;
    case KEY_ID_SRC_8_INDEX:
      return 9;
  }
}

returncode_t libtock_ieee802154_get_key_id(uint32_t       index,
                                           key_id_mode_t* key_id_mode,
                                           uint8_t*       key_id) {
  if (!key_id_mode || !key_id) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) BUF_CFG, 10);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_ieee802154_command_get_key_id(index);

  // unallow the rw buffer from the kernel prior to accessing the buffer.
  libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);

  if (ret == RETURNCODE_SUCCESS) {
    *key_id_mode = (key_id_mode_t) (BUF_CFG[0]);
    memcpy(key_id, BUF_CFG + 1, libtock_ieee802154_key_id_bytes(*key_id_mode));
  }
  return ret;
}

returncode_t libtock_ieee802154_get_key(uint32_t index, uint8_t* key) {
  if (!key) return RETURNCODE_EINVAL;

  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) BUF_CFG, 16);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_ieee802154_command_get_key(index);

  // unallow the rw buffer from the kernel prior to accessing the buffer.
  libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);

  if (ret == RETURNCODE_SUCCESS) {
    memcpy(key, BUF_CFG, 16);
  }

  return ret;
}

returncode_t libtock_ieee802154_get_key_desc(uint32_t          index,
                                             security_level_t* level,
                                             key_id_mode_t*    key_id_mode,
                                             uint8_t*          key_id,
                                             uint8_t*          key) {
  returncode_t ret = libtock_ieee802154_get_key_security_level(index, level);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_ieee802154_get_key_id(index, key_id_mode, key_id);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtock_ieee802154_get_key(index, key);
}

returncode_t libtock_ieee802154_add_key(security_level_t level,
                                        key_id_mode_t    key_id_mode,
                                        uint8_t*         key_id,
                                        uint8_t*         key,
                                        uint32_t*        index) {
  if (!key) return RETURNCODE_EINVAL;

  BUF_CFG[0] = level;
  BUF_CFG[1] = key_id_mode;
  int bytes = libtock_ieee802154_key_id_bytes(key_id_mode);
  if (bytes > 0) {
    memcpy(BUF_CFG + 2, key_id, bytes);
  }
  memcpy(BUF_CFG + 2 + 9, key, 16);

  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) BUF_CFG, 27);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_ieee802154_command_add_key(index);

  // unallow the rw buffer from the kernel so that other libtock functions
  // can utilize and modify the buffer.
  libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);

  // Driver adds 1 to ensure it is positive.
  *index -= 1;

  return ret;
}

returncode_t libtock_ieee802154_remove_key(uint32_t index) {
  return libtock_ieee802154_command_remove_key(index);
}

// Internal callback for transmission
static void tx_done_upcall(int                          status,
                           int                          acked,
                           __attribute__ ((unused)) int unused2,
                           void*                        opaque) {
  libtock_ieee802154_callback_send_done cb = (libtock_ieee802154_callback_send_done) opaque;
  cb(tock_status_to_returncode(status), acked);
}

returncode_t libtock_ieee802154_send(uint32_t                              addr,
                                     security_level_t                      level,
                                     key_id_mode_t                         key_id_mode,
                                     uint8_t*                              key_id,
                                     const uint8_t*                        payload,
                                     uint8_t                               len,
                                     libtock_ieee802154_callback_send_done cb) {
  // Setup parameters in CFG
  BUF_CFG[0] = level;
  BUF_CFG[1] = key_id_mode;
  int bytes = libtock_ieee802154_key_id_bytes(key_id_mode);
  if (bytes > 0) {
    memcpy(BUF_CFG + 2, key_id, bytes);
  }

  // Allow CFG buffer to the kernel
  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) BUF_CFG, 11);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Allow payload buffer to the kernel
  ret = libtock_ieee802154_set_readonly_allow((void*) payload, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Subscribe the provided callback using the internal tx_done_upcall.
  ret = libtock_ieee802154_set_upcall_frame_transmitted(tx_done_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Issue the send command and wait for the transmission to be done.
  ret = libtock_ieee802154_command_send(addr);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return ret;
}

returncode_t libtock_ieee802154_send_raw(
  const uint8_t*                        payload,
  uint8_t                               len,
  libtock_ieee802154_callback_send_done cb) {
  // Setup parameters in ALLOW_RO_TX.
  returncode_t ret = libtock_ieee802154_set_readonly_allow((void*) payload, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Subscribe the provided callback using the internal tx_done_upcall.
  ret = libtock_ieee802154_set_upcall_frame_transmitted(tx_done_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Issue the send command
  return libtock_ieee802154_command_send_raw();
}

// Internal callback for receive
static void rx_done_upcall(int   pans,
                           int   dst_addr,
                           int   src_addr,
                           void* opaque) {
  libtock_ieee802154_callback_recv_done cb = (libtock_ieee802154_callback_recv_done) opaque;
  cb(pans, dst_addr, src_addr);
}

returncode_t libtock_ieee802154_receive(const libtock_ieee802154_rxbuf*       frame,
                                        libtock_ieee802154_callback_recv_done cb) {
  // Provide the ring buffer to the kernel
  returncode_t ret = libtock_ieee802154_set_readwrite_allow_rx((uint8_t*) frame, libtock_ieee802154_RING_BUFFER_LEN);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Subscribe the provided callback using the internal rx_done_upcall.
  return libtock_ieee802154_set_upcall_frame_received(rx_done_upcall, cb);
}

returncode_t libtock_reset_ring_buf(const libtock_ieee802154_rxbuf* frame, subscribe_upcall callback, void* ud) {
  returncode_t ret = libtock_ieee802154_set_readwrite_allow_rx((uint8_t*) frame,
                                                               (frame) ? libtock_ieee802154_RING_BUFFER_LEN : 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtock_ieee802154_set_upcall_frame_received(callback, ud);
}

uint8_t* libtock_ieee802154_read_next_frame(const libtock_ieee802154_rxbuf* frame) {
  if (!frame) return NULL;

  uint8_t* rx_buf = (uint8_t*) frame;
  int read_index  = rx_buf[0];
  int write_index = rx_buf[1];
  if (read_index == write_index) {
    return NULL;
  }
  rx_buf[0]++;
  if (rx_buf[0] >= libtock_ieee802154_MAX_RING_BUF_FRAMES) {
    rx_buf[0] = 0;
  }
  return &rx_buf[libtock_ieee802154_RING_BUF_META_LEN + (read_index * libtock_ieee802154_FRAME_LEN)];
}

int libtock_ieee802154_frame_get_length(const uint8_t* frame) {
  if (!frame) return 0;
  // data_offset + data_len - 2 header bytes
  return frame[0] + frame[1] - 2;
}

int libtock_ieee802154_frame_get_payload_offset(const uint8_t* frame) {
  if (!frame) return 0;
  return frame[0] + libtock_ieee802154_FRAME_META_LEN;
}

int libtock_ieee802154_frame_get_payload_length(const uint8_t* frame) {
  if (!frame) return 0;
  return frame[1];
}

// Utility function to determine if the source and destination PAN and
// addresses are present depending on the frame control field. This is used
// only internally as a header parsing subroutine. Supports only 2003, 2006 or
// 2015 frame versions. Returns false if the addressing mode combination is
// invalid or the frame version is not supported. All out-parameters must be
// provided.
//
// If the source pan is dropped, that means that it is the same as the
// destination pan, which must be present.
static bool libtock_ieee802154_get_addressing(uint16_t     frame_control,
                                              bool*        dst_pan_present,
                                              addr_mode_t* dst_mode,
                                              bool*        src_pan_present,
                                              bool*        src_pan_dropped,
                                              addr_mode_t* src_mode) {
  if (!dst_pan_present || !dst_mode || !src_pan_present || !src_pan_dropped ||
      !src_mode) {
    return false;
  }

  typedef enum {
    VERSION_2003 = 0x0,
    VERSION_2006 = 0x1,
    VERSION_2015 = 0x2,
  } version_t;

  // Fields that determine if the PANs are present
  version_t version = (version_t) ((frame_control >> 12) & 0x3);
  *dst_mode = (addr_mode_t) ((frame_control >> 10) & 0x3);
  *src_mode = (addr_mode_t) ((frame_control >> 14) & 0x3);
  bool pan_id_compression = (frame_control >> 6) & 0x1;
  bool dst_present        = dst_mode != ADDR_NONE;
  bool src_present        = src_mode != ADDR_NONE;

  // The flags that we are trying to determine
  *src_pan_dropped = false;

  // IEEE 802.15.4: Section 7.2.1.5 determines whether the PANs are present
  // depending on the pan ID compression field and the addressing modes.
  if (version == VERSION_2015) {
    if (dst_present) {
      if (src_present) {
        *src_pan_dropped = pan_id_compression;
        *dst_pan_present = true;
        *src_pan_present = !pan_id_compression;
      } else {
        *dst_pan_present = !pan_id_compression;
        *src_pan_present = false;
      }
    } else {
      if (src_present) {
        *dst_pan_present = false;
        *src_pan_present = !pan_id_compression;
      } else {
        *dst_pan_present = pan_id_compression;
        *src_pan_present = false;
      }
    }
  } else if (version == VERSION_2003 || version == VERSION_2006) {
    *src_pan_dropped = pan_id_compression;
    *dst_pan_present = dst_present;
    *src_pan_present = src_present && !src_pan_dropped;
  } else {
    return false;
  }

  // Check validity of addressing modes
  if (*src_pan_dropped && !*dst_pan_present) {
    return 0xff;
  }

  return true;
}

// Utility function to obtain the frame control field from a frame
static void libtock_ieee802154_get_frame_control(const uint8_t* frame, uint16_t* frame_control) {
  if (!frame || !frame_control) return;
  *frame_control = ((uint16_t) frame[libtock_ieee802154_FRAME_META_LEN]) |
                   (((uint16_t) frame[libtock_ieee802154_FRAME_META_LEN + 1]) << 8);
}

// Utility function to obtain the address offset from a frame
static void libtock_ieee802154_get_addr_offset(const uint8_t* frame, uint16_t* addr_offset, uint16_t* frame_control,
                                               const uint16_t* SEQ_SUPPRESSED) {
  if (!frame || !addr_offset || !frame_control || !SEQ_SUPPRESSED) return;
  *addr_offset = ((*frame_control & *SEQ_SUPPRESSED) ? 2 : 3) + libtock_ieee802154_FRAME_META_LEN;
}

addr_mode_t libtock_ieee802154_frame_get_dst_addr(__attribute__ ((unused)) const uint8_t* frame,
                                                  __attribute__ ((unused)) uint16_t*      short_addr,
                                                  __attribute__ ((unused)) uint8_t*       long_addr) {
  if (!frame) return ADDR_NONE;

  uint16_t frame_control;
  libtock_ieee802154_get_frame_control(frame, &frame_control);

  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!libtock_ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                         &src_pan_present, &src_pan_dropped, &src_mode)) {
    return ADDR_NONE;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  uint16_t addr_offset;
  libtock_ieee802154_get_addr_offset(frame, &addr_offset, &frame_control, &SEQ_SUPPRESSED);

  if (dst_pan_present) addr_offset += 2;

  if (dst_mode == ADDR_SHORT && short_addr) {
    *short_addr = ((uint16_t) frame[addr_offset]) |
                  (((uint16_t) frame[addr_offset + 1]) << 8);
  }
  if (dst_mode == ADDR_LONG && long_addr) {
    int i;
    for (i = 0; i < 8; i++) {
      long_addr[i] = frame[addr_offset + 7 - i];
    }
  }

  return dst_mode;
}

addr_mode_t libtock_ieee802154_frame_get_src_addr(__attribute__ ((unused)) const uint8_t* frame,
                                                  __attribute__ ((unused)) uint16_t*      short_addr,
                                                  __attribute__ ((unused)) uint8_t*       long_addr) {
  if (!frame) return ADDR_NONE;

  uint16_t frame_control;
  libtock_ieee802154_get_frame_control(frame, &frame_control);

  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!libtock_ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                         &src_pan_present, &src_pan_dropped, &src_mode)) {
    return ADDR_NONE;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  uint16_t addr_offset;
  libtock_ieee802154_get_addr_offset(frame, &addr_offset, &frame_control, &SEQ_SUPPRESSED);

  if (dst_pan_present) addr_offset += 2;
  if (dst_mode == ADDR_SHORT) {
    addr_offset += 2;
  } else if (dst_mode == ADDR_LONG) {
    addr_offset += 8;
  }
  if (src_pan_present) addr_offset += 2;

  if (src_mode == ADDR_SHORT && short_addr) {
    *short_addr = ((uint16_t) frame[addr_offset]) |
                  (((uint16_t) frame[addr_offset + 1]) << 8);
  }
  if (src_mode == ADDR_LONG && long_addr) {
    int i;
    for (i = 0; i < 8; i++) {
      long_addr[i] = frame[addr_offset + 7 - i];
    }
  }

  return src_mode;
}

bool libtock_ieee802154_frame_get_dst_pan(__attribute__ ((unused)) const uint8_t* frame,
                                          __attribute__ ((unused)) uint16_t*      pan) {
  if (!frame) return false;

  uint16_t frame_control;
  libtock_ieee802154_get_frame_control(frame, &frame_control);

  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!libtock_ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                         &src_pan_present, &src_pan_dropped, &src_mode)) {
    return false;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  uint16_t addr_offset;
  libtock_ieee802154_get_addr_offset(frame, &addr_offset, &frame_control, &SEQ_SUPPRESSED);

  if (dst_pan_present && pan) {
    *pan = ((uint16_t) frame[addr_offset]) |
           (((uint16_t) frame[addr_offset + 1]) << 8);
  }

  return dst_pan_present;
}

bool libtock_ieee802154_frame_get_src_pan(__attribute__ ((unused)) const uint8_t* frame,
                                          __attribute__ ((unused)) uint16_t*      pan) {
  if (!frame) return false;

  uint16_t frame_control;
  libtock_ieee802154_get_frame_control(frame, &frame_control);

  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!libtock_ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                         &src_pan_present, &src_pan_dropped, &src_mode)) {
    return false;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  uint16_t addr_offset;
  libtock_ieee802154_get_addr_offset(frame, &addr_offset, &frame_control, &SEQ_SUPPRESSED);

  if (src_pan_dropped) {
    // We can assume that the destination pan is present.
    if (pan) {
      *pan = ((uint16_t) frame[addr_offset]) |
             (((uint16_t) frame[addr_offset + 1]) << 8);
    }
  } else {
    if (dst_pan_present) addr_offset += 2;
    if (dst_mode == ADDR_SHORT) {
      addr_offset += 2;
    } else if (dst_mode == ADDR_LONG) {
      addr_offset += 8;
    }

    if (src_pan_present && pan) {
      *pan = ((uint16_t) frame[addr_offset]) |
             (((uint16_t) frame[addr_offset + 1]) << 8);
    }
  }

  return src_pan_present || src_pan_dropped;
}
