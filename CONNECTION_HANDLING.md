# ngircd Connection Handling on ELKS

## Question: Does ngircd fork for each connection or use select()?

## Answer: **select() based, NO forking per connection**

ngircd uses an **event-driven, single-process model** with `select()` (or better alternatives when available).

## Architecture Overview

### 1. I/O Multiplexing (Event Loop)

ngircd supports multiple I/O methods in order of preference:
1. **epoll** (Linux, most efficient)
2. **kqueue** (BSD)
3. **devpoll** (Solaris)
4. **poll** (POSIX)
5. **select** (fallback, most portable)

**On ELKS:** Uses `select()` because:
- ELKS doesn't have epoll/kqueue/devpoll
- poll() may not be fully implemented
- select() is the POSIX standard fallback

### 2. Connection Handling Flow

```
1. Server socket listens on port 6667
2. select() monitors all file descriptors (listening socket + client connections)
3. When select() returns:
   - If listening socket is ready → accept() new connection
   - If client socket is ready → read/write data
4. All handled in SINGLE process, NO fork per connection
```

### 3. Code Evidence

From `src/ngircd/conn.c` line 1409:
```c
new_sock = accept(Sock, (struct sockaddr *)&new_addr,
                  (socklen_t *)&new_sock_len);
```

From `src/ngircd/io.c` line 136:
```c
static int io_dispatch_select(struct timeval *tv)
{
    fd_set readers_tmp;
    fd_set writers_tmp;
    // ... select() on all file descriptors
}
```

**No fork()** called for regular connections!

### 4. Why This Design?

#### Advantages for ELKS:
✅ **Memory efficient**: One process handles all connections
✅ **No fork overhead**: fork() is expensive on ELKS
✅ **Simpler resource management**: All connections share same heap/stack
✅ **Low process count**: Important on constrained systems

#### Trade-offs:
⚠️ **Single-threaded**: One blocking connection affects all
⚠️ **select() limitations**: Max FD_SETSIZE file descriptors (usually 1024)
⚠️ **Stack sharing**: Large buffers affect all connections (hence our stack fix!)

## How It Fits in ELKS Memory

### Memory Layout with 10 Connections:
```
Stack: 8KB
  └─ Shared by all connections
  └─ READBUFFER_LEN (2048 bytes) reused per read

Heap: 12KB  
  └─ Client structures (malloc'd)
  └─ Channel lists
  └─ Message buffers

Static Data: ~44KB
  └─ Global arrays (connections, channels, etc)
  
Total: ~64KB (fits in ELKS data segment!)
```

### If it forked per connection:
```
10 connections × 64KB each = 640KB!
ELKS can't handle this!
```

## Comparison: Fork vs Select

### Fork Model (what ngircd DOESN'T do):
```c
// Hypothetical (NOT used):
new_sock = accept(...);
if (fork() == 0) {
    // Child process handles ONE client
    while (1) {
        read(new_sock, ...);
        // Process commands
    }
    exit(0);
}
// Parent continues accepting
```

**Memory**: 10 clients = 10 processes × 64KB = 640KB
**ELKS**: ❌ Would exhaust memory immediately!

### Select Model (what ngircd DOES):
```c
// Simplified:
while (1) {
    FD_ZERO(&readfds);
    FD_SET(listen_sock, &readfds);
    for (each client)
        FD_SET(client_sock, &readfds);
    
    select(maxfd, &readfds, ...);
    
    if (FD_ISSET(listen_sock, &readfds))
        new_sock = accept(...);  // New client
    
    for (each client)
        if (FD_ISSET(client_sock, &readfds))
            read_and_process(client_sock);  // Handle data
}
```

**Memory**: 1 process for ALL clients = 64KB total
**ELKS**: ✅ Works perfectly!

## Special Case: DNS/Ident Resolution

ngircd **does fork** for DNS/ident lookups (optional):
- Disabled on ELKS (NGIRCD_DISABLE_RESOLVER)
- Would only fork briefly for hostname resolution
- Main connection handling remains single-process

## Configuration Impact

From `ngircd.conf`:
```ini
[Limits]
MaxConnections = 10      # Max simultaneous connections
MaxUsers = 100           # Max registered users
```

All 10 connections handled by **ONE process** using select()!

## Summary

**ngircd on ELKS:**
- ✅ Uses `select()` to multiplex I/O across all connections
- ✅ Single process handles all clients
- ✅ NO fork per connection
- ✅ Memory efficient (~64KB for server + all clients)
- ✅ Perfect for ELKS's limited resources

**Why it fits in ELKS despite "small":**
- Event-driven = share one stack
- No fork = no per-process overhead  
- Careful buffer sizing = fits in 64KB data segment

The single-process, select()-based design is **exactly why** ngircd can run on ELKS!
