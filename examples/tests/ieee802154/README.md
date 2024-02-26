This is one of five  applications to test 802.15.4 packet reception
and transmission. The five apps are:

radio_ack: Sends packets, using a printf to signal whether they were
           acknowledged. Also receives packets.
radio_rx: Receives packets only.
radio_rxtx: Sends and receives packets.
radio_tx: Sends packets only.
radio_tx_raw: Send packets fully formed by userprocess. This example sends an ACK packet. For forming headers, use the generic `ieee802154_send(..)` method. 