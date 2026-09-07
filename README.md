<div align="center">
	<h3><em>lib</em>WBTP — Web Binary Transfer Protocol</h3>
</div>

**WBTP** is a drop-in replacement for **HTTP** (but not **HTTPS**). However, readable text was swapped out for packed binary! This allows for quicker transfer speeds and serialization/deserialization, at the loss of direct readability.

Implementations providing a custom networking layer must use **TCP** and make no direct modifications to our request/response packets, to maintain spec compliance. A default TCP port of `6969` should be assumed, though custom ports are allowed.

Each release package provides C17-focused header files along with shared-object libraries for [5 platforms](#supported-platforms-5)! This doesn't mean the library is completely incompatible, though. We suggest testing it yourself and submitting a PR modifying the list of tested C versions below, along with how compatible the library is:
- Standard 17 **(main target)**

---

### Table of Contents
- [WBTP Spec](#wbtp-spec)
- [Supported Platforms (5)](#supported-platforms-5)

---

### WBTP Spec

All numerical values are big-endian (aka network byte order).

**Data Types**
|Label|Description|
|---|---|
|`byte`|Unsigned 8-bit integer|
|`u32`|Unsigned 32-bit/4-byte integer|
|`string`|UTF-8 encoded byte(s)|

A typical **WBTP** request is structured something like this:
```
[packet size : u32]                                    ; Size of everything that follows (in bytes, not including itself)
[type : byte]                                          ; Type of request
[path size : u32][...path : string, max size 512]      ; Size of request path, followed by request path
[params size : u32][...params : string, max size 512]  ; Size of request params, followed by request params
[payload size : u32][...payload : bytes, no max size]  ; Size of payload, followed by payload
```

> Note that params has a standardized value template! This is defined somewhere in the header file located at [include/wbtp.h](./include/wbtp.h) or inside a release ZIP/TAR.

...and a typical **WBTP** response is structured something like this:
```
[packet size : u32]                                    ; Size of everything that follows (in bytes)
[type : byte]                                          ; Type of response
[params size : u32][...params : string, max size 512]  ; Size of response params, followed by response params
[payload size : u32][...payload : bytes, no max size]  ; Size of payload, followed by payload
```

> Note that params has a standardized value template! This is defined somewhere in the header file located at [include/wbtp.h](./include/wbtp.h) or inside a release ZIP/TAR.

---

### Supported Platforms (5)

- Windows (x86_64 + arm64)
- Linux (x86_64 + arm64)
- macOS (arm64)
