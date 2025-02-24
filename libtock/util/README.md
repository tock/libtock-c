# `libtock/util`

This directory contains utilities related to abstractions provided by
the Tock kernel. Currently, the following utilities exist:

- Streaming Process Slice:
  [`streaming_process_slice.h`](./streaming_process_slice.h)

  A contract over read-write allows for losslessly streaming data from the Tock
  kernel to a userspace process.

  Applications like ADC sampling or network stacks require the kernel to provide
  a process with a continuous, lossless stream of data from a source that is not
  rate-controlled by the process. This utility implements the userspace-side of
  a simple protocol to achieve this goal, without requiring kernel-side
  buffering and by utilizing the atomic swap semantics of Tock’s allow system
  call. For more information on this contract, see
  <https://docs.tockos.org/kernel/utilities/streaming_process_slice/struct.streamingprocessslice>
