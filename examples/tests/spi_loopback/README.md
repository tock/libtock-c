SPI Loopback Test
=================

Writes a known pattern out over the SPI controller syscall interface and
reads it back in the same transfer, then checks the two match.

This only produces a meaningful result on a board where SPI is configured
in hardware loopback mode (so the read data is expected to equal the write
data), such as the QEMU `qemu_arm_mps2_an385`/`qemu_arm_mps2_an386` boards.
On a board wired to a real, non-looped-back SPI peripheral, a mismatch is
expected and does not indicate a bug.

Prints `SPI PASS` or `SPI FAIL: <reason>` so the result can be checked from
a console transcript.
